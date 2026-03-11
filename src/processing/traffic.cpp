#include "core.hpp"

#include "prism/traffic/game_traffic.hpp"
#include "prism/traffic/traffic_rules.hpp"
#include "prism/controllers/game_ctrl.hpp"
#include "prism/management/item/kdop_item.hpp"
#include "prism/management/item/prefab_item.hpp"
#include "prism/management/item/segment.hpp"
#include "prism/management/item/semaphore_instance.hpp"

#include "processing/traffic.hpp"

#include <vector>
#include <cmath>
#include <algorithm>

namespace ets2la_plugin
{
    short TrafficProcessor::get_uid_for_vehicle(uintptr_t vehicle_ptr) const
    {
        auto it = vehicle_uids.find(vehicle_ptr);
        if (it != vehicle_uids.end())
        {
            return it->second;
        }
        else
        {
            short new_uid = static_cast<short>(vehicle_uids.size()) + 1;
            vehicle_uids[vehicle_ptr] = new_uid;
            return new_uid;
        }
    }

    std::vector<prism::traffic_ai_vehicle_t*> TrafficProcessor::get_ai_traffic_data() const
    {
        std::vector<prism::traffic_ai_vehicle_t*> vehicles;
        auto* game_traffic = prism::game_traffic_u::get();
        if (game_traffic == nullptr)
            return vehicles;

        for (const auto& ai_vehicle : game_traffic->ai_vehicles)
        {
            vehicles.push_back(ai_vehicle.vehicle);
        }

        return vehicles;
    }

    std::pair<std::vector<prism::game_physics_vehicle_u*>, std::vector<prism::game_trailer_actor_u*>> TrafficProcessor::get_truckersmp_traffic_data() const
    {
        std::vector<prism::game_physics_vehicle_u*> trucks;
        std::vector<prism::game_trailer_actor_u*> trailers;

        auto* game_ctrl = prism::game_ctrl_u::get();
        if (game_ctrl == nullptr)
        {
            return std::make_pair(trucks, trailers);
        }
        
        static prism::unit_descriptor_t *stored_game_trailer_actor_unit_descriptor = nullptr;
        const auto* vehicles_list = game_ctrl->get_some_nearby_non_ai_vehicles_list();
        if (vehicles_list == nullptr)
        {
            return std::make_pair(trucks, trailers);
        }


        auto* node = vehicles_list->begin;
        while(node->item != vehicles_list->empty_item)
        {
            const auto unit_descriptor = node->item->get_unit_descriptor();

            if (stored_game_trailer_actor_unit_descriptor == nullptr)
            {
                // check if the node item is of type 'game_trailer_actor' and store the address of the unit descriptor if it is
                // that way we can just compare the unit descriptor addresses instead of comparing strings every time
                if (strcmp(*unit_descriptor->p_class_name, "game_trailer_actor") == 0)
                {
                    stored_game_trailer_actor_unit_descriptor = unit_descriptor;
                }
            }

            const auto is_trailer = stored_game_trailer_actor_unit_descriptor == unit_descriptor;
            if (is_trailer)
            {
                prism::game_trailer_actor_u* trailer = reinterpret_cast<prism::game_trailer_actor_u *>(node->item);
                trailers.push_back(trailer);
            }
            else
            {
                auto *truck = reinterpret_cast<prism::game_physics_vehicle_u *>(node->item);
                trucks.push_back(truck);
            }

            node = node->next;
        }

        return std::make_pair(trucks, trailers);
    }

    void TrafficProcessor::write_traffic_data(
        const processor_traffic_data_t& traffic_data, 
        CMemoryHandler* memory_handler, 
        scs_value_dplacement_t truck_pos
    ) const
    {
        std::vector<processor_traffic_vehicle_object_t> traffic_objects;

        // Here we sort the vehicles by their distance to the truck. I've not figured
        // out a better way to do this yet, if you can think of a better sorting algorithm
        // then please ping @Tumppi066 on our Discord server or create a PR.

        for (const auto& ai_vehicle : traffic_data.ai_vehicles)
        {
            processor_traffic_vehicle_object_t traffic_object;
            traffic_object.type = 0;
            traffic_object.ai_vehicle = ai_vehicle;

            float dx = (ai_vehicle->placement.cx * 512 + ai_vehicle->placement.pos.x) - truck_pos.position.x;
            float dz = (ai_vehicle->placement.cz * 512 + ai_vehicle->placement.pos.z) - truck_pos.position.z;
            traffic_object.truck_distance = std::sqrt(dx * dx + dz * dz);

            traffic_objects.push_back(traffic_object);
        }

        for (const auto& truck : traffic_data.tmp_vehicles.first)
        {
            processor_traffic_vehicle_object_t traffic_object;
            traffic_object.type = 1;
            traffic_object.tmp_truck = truck;

            prism::placement_t truck_placement;
            truck->get_physics_placement(&truck_placement);

            float dx = (truck_placement.cx * 512 + truck_placement.pos.x) - truck_pos.position.x;
            float dz = (truck_placement.cz * 512 + truck_placement.pos.z) - truck_pos.position.z;
            traffic_object.truck_distance = std::sqrt(dx * dx + dz * dz);

            traffic_objects.push_back(traffic_object);
        }

        for (const auto& trailer : traffic_data.tmp_vehicles.second)
        {
            processor_traffic_vehicle_object_t traffic_object;
            traffic_object.type = 2;

            prism::placement_t trailer_placement;
            trailer->get_physics_placement(&trailer_placement);

            float dx = (trailer_placement.cx * 512 + trailer_placement.pos.x) - truck_pos.position.x;
            float dz = (trailer_placement.cz * 512 + trailer_placement.pos.z) - truck_pos.position.z;
            traffic_object.truck_distance = std::sqrt(dx * dx + dz * dz);

            auto* trailer_obj = trailer;
            while (trailer_obj != nullptr)
            {
                traffic_object.tmp_trailers.push_back(trailer_obj);
                trailer_obj = trailer_obj->get_slave_trailer();
            }

            traffic_objects.push_back(traffic_object);
        }

        
        std::sort(traffic_objects.begin(), traffic_objects.end(), [](const processor_traffic_vehicle_object_t& a, const processor_traffic_vehicle_object_t& b) {
            return a.truck_distance < b.truck_distance;
        });

        // Now we need to convert this data to the format the memory uses. Once again the code
        // for TMP vehicles is convoluted, so if there's a better way to match trailers to their
        // trucks then do the same as above and ping @Tumppi066 on our Discord server or create a PR.
        // (especially figuring out a way to match the trailers to their trucks)
        
        std::array<TrafficVehicleObject, 40> traffic_vehicle_objects = {};
        int count = 0;

        for (const auto& traffic_object : traffic_objects)
        {
            if (count >= 40)
                break;

            TrafficVehicleObject vehicle_object = {};
            vehicle_object.vehicle.id = -1;

            if (traffic_object.type == 0) // ai vehicle
            {
                const auto* ai_vehicle = traffic_object.ai_vehicle;
                vehicle_object.vehicle = TrafficVehicle{
                    ai_vehicle->placement.pos.x + 512 * ai_vehicle->placement.cx,
                    ai_vehicle->placement.pos.y,
                    ai_vehicle->placement.pos.z + 512 * ai_vehicle->placement.cz,
                    ai_vehicle->placement.rot.w,
                    ai_vehicle->placement.rot.x,
                    ai_vehicle->placement.rot.y,
                    ai_vehicle->placement.rot.z,
                    ai_vehicle->physics_data->bounding_box.x, // width
                    ai_vehicle->physics_data->bounding_box.y, // height
                    ai_vehicle->physics_data->bounding_box.z, // length
                    ai_vehicle->physics_data->speed,          // speed
                    ai_vehicle->physics_data->acceleration,   // acceleration
                    0,                                        // trailer_count
                    get_uid_for_vehicle(reinterpret_cast<uintptr_t>(ai_vehicle)), // id
                    false,                                    // is_tmp
                    false                                     // is_trailer
                };

                auto trailer = ai_vehicle->trailer;
                int i = 0;
                while (trailer != nullptr && i < 3)
                {
                    vehicle_object.trailers[i] = TrafficTrailer{
                        trailer->placement.pos.x + 512 * trailer->placement.cx,
                        trailer->placement.pos.y,
                        trailer->placement.pos.z + 512 * trailer->placement.cz,
                        trailer->placement.rot.w,
                        trailer->placement.rot.x,
                        trailer->placement.rot.y,
                        trailer->placement.rot.z,
                        trailer->physics_data->bounding_box.x, // width
                        trailer->physics_data->bounding_box.y, // height
                        trailer->physics_data->bounding_box.z  // length
                    };

                    trailer = trailer->slave_trailer;
                    i++;
                }

                vehicle_object.vehicle.trailer_count = i;
            }
            else if (traffic_object.type == 1) // tmp vehicle
            {
                const auto* tmp_truck = traffic_object.tmp_truck;
                prism::placement_t truck_placement;
                tmp_truck->get_physics_placement(&truck_placement);
                
                vehicle_object.vehicle = TrafficVehicle{
                    truck_placement.pos.x + 512 * truck_placement.cx,
                    truck_placement.pos.y,
                    truck_placement.pos.z + 512 * truck_placement.cz,
                    truck_placement.rot.w,
                    truck_placement.rot.x,
                    truck_placement.rot.y,
                    truck_placement.rot.z,
                    abs(tmp_truck->dimensions.start.x - tmp_truck->dimensions.end.x), // width
                    abs(tmp_truck->dimensions.start.y - tmp_truck->dimensions.end.y), // height
                    abs(tmp_truck->dimensions.start.z - tmp_truck->dimensions.end.z), // length
                    0,                               // speed (null)
                    tmp_truck->linear_acceleration.x + tmp_truck->linear_acceleration.y + tmp_truck->linear_acceleration.z,  // acceleration (null)
                    0,                               // trailer_count
                    get_uid_for_vehicle(reinterpret_cast<uintptr_t>(tmp_truck)),
                    true,                            // is_tmp
                    false                            // is_trailer
                };
            }
            else if (traffic_object.type == 2) // tmp trailer
            {
                const auto* tmp_trailer = traffic_object.tmp_trailers.empty() ? nullptr : traffic_object.tmp_trailers[0];
                if (tmp_trailer == nullptr)
                    continue;

                prism::placement_t trailer_placement;
                tmp_trailer->get_physics_placement(&trailer_placement);

                vehicle_object.vehicle = TrafficVehicle{
                    trailer_placement.pos.x + 512 * trailer_placement.cx,
                    trailer_placement.pos.y,
                    trailer_placement.pos.z + 512 * trailer_placement.cz,
                    trailer_placement.rot.w,
                    trailer_placement.rot.x,
                    trailer_placement.rot.y,
                    trailer_placement.rot.z,
                    abs(tmp_trailer->dimensions.start.x - tmp_trailer->dimensions.end.x), // width
                    abs(tmp_trailer->dimensions.start.y - tmp_trailer->dimensions.end.y), // height
                    abs(tmp_trailer->dimensions.start.z - tmp_trailer->dimensions.end.z), // length
                    0,                               // speed (null)
                    tmp_trailer->linear_acceleration.x + tmp_trailer->linear_acceleration.y + tmp_trailer->linear_acceleration.z,  // acceleration (null)
                    0,                               // trailer_count
                    get_uid_for_vehicle(reinterpret_cast<uintptr_t>(tmp_trailer)),
                    true,                            // is_tmp
                    true                             // is_trailer
                };

                for (size_t i = 1; i < traffic_object.tmp_trailers.size() && i < 4; i++)
                {
                    const auto* trailer = traffic_object.tmp_trailers[i];
                    prism::placement_t trailer_placement;
                    trailer->get_physics_placement(&trailer_placement);

                    vehicle_object.trailers[i-1] = TrafficTrailer{
                        trailer_placement.pos.x + 512 * trailer_placement.cx,
                        trailer_placement.pos.y,
                        trailer_placement.pos.z + 512 * trailer_placement.cz,
                        trailer_placement.rot.w,
                        trailer_placement.rot.x,
                        trailer_placement.rot.y,
                        trailer_placement.rot.z,
                        abs(trailer->dimensions.start.x - trailer->dimensions.end.x), // width
                        abs(trailer->dimensions.start.y - trailer->dimensions.end.y), // height
                        abs(trailer->dimensions.start.z - trailer->dimensions.end.z)  // length
                    };
                }
            }

            if(vehicle_object.vehicle.id == -1)
            {
                continue;
            }

            traffic_vehicle_objects[count] = vehicle_object;
            count++;
        }

        memory_handler->write_traffic_mem(TrafficMemData{
            traffic_vehicle_objects
        });
    }

    std::vector<processor_traffic_object_t> TrafficProcessor::get_traffic_objects_data() const
    {
        std::vector<processor_traffic_object_t> traffic_objects;

        auto* base_ctrl = prism::base_ctrl_u::get();
        if (base_ctrl == nullptr)
            return traffic_objects;

        auto* nearby_kdop_items = base_ctrl->get_nearby_kdop_items();
        if ( nearby_kdop_items == nullptr )
            return traffic_objects;

        // Gather traffic lights and gates from prefabs
        for (const auto *kdop_item : *nearby_kdop_items )
        {
            if (kdop_item->item_type != 4) // 4 = prefab
            {
                if (kdop_item->item_type > 4)
                    break; // items seem to be ordered by item type, so we can stop looping after going through all prefabs
                continue;
            }

            const auto prefab_kdop_item = static_cast<const prism::prefab_item_t *>(kdop_item);
            if (prefab_kdop_item->segment == nullptr)
                continue;

            for (const auto &semaphore_instance : prefab_kdop_item->segment->semaphore_instances)
            {
                if (semaphore_instance.actor == nullptr || semaphore_instance.actor->get_type() != 0x07) // we only want type 0x07 (traffic_semaphore_actor_t)
                {
                    continue;
                }
                auto* semaphore_actor = static_cast<const prism::traffic_semaphore_actor_t*>(semaphore_instance.actor);
                traffic_objects.push_back(processor_traffic_object_t{
                    semaphore_instance.id,
                    0, // distance is calculated in write_traffic_object_data
                    semaphore_actor
                });
            }
        }

        // Gather traffic lights and gates from traffic objects list 
        // (these seem to be mostly gates that are not part of a prefab)
        auto* game_traffic = prism::game_traffic_u::get();
        if (game_traffic != nullptr)
        {
            for (const auto* traffic_object : game_traffic->traffic_objects)
            {
                if (traffic_object->get_type() != 0x07)
                    continue; // we only want type 0x07 (traffic_semaphore_actor_t)

                auto* semaphore_actor = static_cast<const prism::traffic_semaphore_actor_t*>(traffic_object);
                traffic_objects.push_back(processor_traffic_object_t{
                    0, // id is not available for traffic objects that are not part of a prefab
                    0, // distance is calculated in write_traffic_object_data
                    semaphore_actor
                });
            }
        }

        return traffic_objects;
    }

    void TrafficProcessor::write_traffic_objects_data(
        std::vector<processor_traffic_object_t> traffic_objects, 
        CMemoryHandler* memory_handler, 
        scs_value_dplacement_t truck_pos
    ) const
    {
        for (auto traffic_object : traffic_objects)
        {
            float dx = (traffic_object.traffic_object->placement.cx * 512 + traffic_object.traffic_object->placement.pos.x) - truck_pos.position.x;
            float dz = (traffic_object.traffic_object->placement.cz * 512 + traffic_object.traffic_object->placement.pos.z) - truck_pos.position.z;
            traffic_object.distance = std::sqrt(dx * dx + dz * dz);
        }

        std::sort(traffic_objects.begin(), traffic_objects.end(), [](const processor_traffic_object_t& a, const processor_traffic_object_t& b) {
            return a.distance < b.distance;
        });

        // We only want to send the closest 40 traffic objects to the memory
        if (traffic_objects.size() > 40)
        {
            traffic_objects.resize(40);
        }

        std::array<SemaphoreObject, 40> semaphore_objects_data = {};
        int count = 0;
        for (const auto& obj : traffic_objects)
        {
            if (count >= 40)
                break;

            auto* traffic_object = obj.traffic_object;

            SemaphoreObject return_object = {
                traffic_object->placement.pos.x,
                traffic_object->placement.pos.y,
                traffic_object->placement.pos.z,
                traffic_object->placement.cx,
                traffic_object->placement.cz,
                traffic_object->placement.rot.w,
                traffic_object->placement.rot.x,
                traffic_object->placement.rot.y,
                traffic_object->placement.rot.z,
            };
            return_object.id = obj.id;

            if (traffic_object->traffic_rule != nullptr && traffic_object->traffic_rule->get_type() == prism::traffic_light_t::ID)
            {
                const auto* traffic_light = static_cast<const prism::traffic_light_t*>(traffic_object->traffic_rule);
                return_object.type = 1; // 1 = traffic light
                return_object.time_remaining = traffic_light->state_time_remaining;
                // 0 = off, 1 = orange_to_red, 2 = red, 4 = orange_to_green, 8 = green, 32 = sleep (blinking orange)
                return_object.state = traffic_light->state;
            }
            else // gates
            {
                // state // 0 = closing; 1 = closed; 3 = open; 2 = opening
                return_object.type = 2; // 2 = gate
                return_object.state = traffic_object->state;
                if (traffic_object->animation_mode == 2)
                {
                    return_object.time_remaining = traffic_object->open_time_remaining;
                } else {
                    return_object.time_remaining = traffic_object->animation_time_elapsed;
                }
            }

            semaphore_objects_data[count] = return_object;
            count++;
        }

        memory_handler->write_semaphore_mem(SemaphoreMemData{
            semaphore_objects_data
        });
    }
}