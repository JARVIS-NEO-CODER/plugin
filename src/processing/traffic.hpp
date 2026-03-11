#pragma once

#include "prism/traffic/traffic_ai_trailer.hpp"
#include "prism/traffic/traffic_ai_vehicle.hpp"
#include "prism/traffic/traffic_objects.hpp"
#include "prism/vehicles/game_physics_vehicle.hpp"
#include "prism/vehicles/game_trailer_actor.hpp"
#include "prism/management/item/semaphore_instance.hpp"
#include "memory/virtual/memory_handler.hpp"
#include <vector>

namespace ets2la_plugin
{
    struct processor_traffic_data_t
    {
        std::vector<prism::traffic_ai_vehicle_t*> ai_vehicles;
        std::pair<
            std::vector<prism::game_physics_vehicle_u*>, 
            std::vector<prism::game_trailer_actor_u*>
        > tmp_vehicles;
    };

    // Used internally in the TrafficProcessor to store ai and tmp vehicles
    // in a way I can use them easily.
    struct processor_traffic_vehicle_object_t
    {
        short type; // 0 = ai, 1 = tmp, 2 = tmp trailer
        float truck_distance;
        prism::traffic_ai_vehicle_t* ai_vehicle;
        prism::game_physics_vehicle_u* tmp_truck;
        std::vector<prism::game_trailer_actor_u*> tmp_trailers;
    };

    // Used internally in the TrafficProcessor to traffic objects (traffic lights and gates)
    // in a way I can use them easily.
    struct processor_traffic_object_t
    {
        uint32_t id;
        float distance;
        const prism::traffic_semaphore_actor_t* traffic_object;
    };

    class TrafficProcessor
    {
        private:
            mutable std::map<uintptr_t, short> vehicle_uids = {};
            scs_log_t scs_log_;

            template <class... T>
            void info(const char *fmt_s, T &&...args) const
            {
                scs_log_(0, fmt::vformat(std::string("[ets2la_plugin][TrafficProcessor] ") + fmt_s, fmt::make_format_args(args...)).c_str());
            }

        public:
            TrafficProcessor(scs_log_t scs_log) { scs_log_ = scs_log; }
            short get_uid_for_vehicle(uintptr_t vehicle_ptr) const;

            std::vector<prism::traffic_ai_vehicle_t*> get_ai_traffic_data() const;
            std::pair<std::vector<prism::game_physics_vehicle_u*>, std::vector<prism::game_trailer_actor_u*>> get_truckersmp_traffic_data() const;
            void write_traffic_data(const processor_traffic_data_t& traffic_data, CMemoryHandler* memory_handler, scs_value_dplacement_t truck_pos) const;

            std::vector<processor_traffic_object_t> get_traffic_objects_data() const;
            void write_traffic_objects_data(std::vector<processor_traffic_object_t> traffic_objects, CMemoryHandler* memory_handler, scs_value_dplacement_t truck_pos) const;

    };
}