#pragma once
#include "prism/collections/array_dyn.hpp"
#include "prism/unit/unit.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    // Size: 0x0638 1.58
    class traffic_u : public /* [0x10] @ 0x00 */ unit_t
    {
        class vehicle_type_info_t;
        class trailer_type_info_t;
        class lane_type_info_t;
        class pending_spawn_t;
        class cached_vehicle_t;
        class item_in_cutscene_t;
        class area_item_info_t;
        class sign_item_info_t;
        class remote_clients_jip_context_t;

        class spawned_vehicle_t // Size: 0x0010
        {
        public:
            class traffic_ai_vehicle_t* vehicle; //0x0000 (0x08)
            char pad_0008[ 8 ];                  //0x0008 (0x08)
        };
        static_assert( sizeof( spawned_vehicle_t ) == 0x10 );

    public:
        char pad_0010[ 8 ];                                                     // 0x0010 (0x08)
        class traffic_data_u* traffic_data;                                     // 0x0018 (0x08)
        class color_array_u* color_array;                                       // 0x0020 (0x08)
        array_dyn_t< class traffic_u::vehicle_type_info_t > truck_type_infos;   // 0x0028 (0x20)
        array_dyn_t< class traffic_u::trailer_type_info_t > trailer_type_infos; // 0x0048 (0x20)
        array_dyn_t< class traffic_u::lane_type_info_t > lane_type_infos;       // 0x0068 (0x20)
        array_dyn_t< class traffic_u::pending_spawn_t* > pending_spawns;        // 0x0088 (0x20)
        array_dyn_t< class traffic_u::cached_vehicle_t > cached_vehicles;       // 0x00A8 (0x20)
        array_dyn_t< class traffic_u::spawned_vehicle_t > spawned_vehicles_1;   // 0x00C8 (0x20)
        array_dyn_t< class traffic_u::spawned_vehicle_t >
            spawned_vehicles_2; // 0x00E8 (0x20) traffic vehicles from other convoy users
        array_dyn_t< class traffic_u::spawned_vehicle_t >
            spawned_vehicles_3; // 0x0108 (0x20) out of range? traffic vehicles from other convoy users
        array_dyn_t< class traffic_ai_vehicle_t* > traffic_ai_vehicles_1;         // 0x0128 (0x20)
        array_dyn_t< class traffic_ai_vehicle_t* > traffic_ai_vehicles_2;         // 0x0148 (0x20)
        char pad_0168[ 40 ];                                                      // 0x0168 (0x28)
        array_dyn_t< class traffic_object_t* > traffic_objects_1;                 // 0x0190 (0x20)
        array_dyn_t< class traffic_player_vehicle_t* > traffic_player_vehicles_1; // 0x01B0 (0x20)
        array_dyn_t< class traffic_player_trailer_t* > traffic_player_trailers_1; // 0x01D0 (0x20)
        array_dyn_t< class traffic_player_vehicle_t* > traffic_player_vehicles_2; // 0x01F0 (0x20)
        array_dyn_t< class traffic_player_trailer_t* > traffic_player_trailers_2; // 0x0210 (0x20)
        array_dyn_t< class traffic_world_rule_t* > traffic_world_rules_1;         // 0x0230 (0x20)
        class traffic_environment_t* traffic_environment;                         // 0x0250 (0x08)
        array_dyn_t< class traffic_segment_t* > traffic_segments_1;               // 0x0258 (0x20)
        array_dyn_t< class traffic_segment_t* > traffic_segments_2;               // 0x0278 (0x20)
        array_dyn_t< class traffic_segment_t* > traffic_segments_3;               // 0x0298 (0x20)
        array_dyn_t< class kdop_item_t* > kdop_items_1;                           // 0x02B8 (0x20)
        array_dyn_t< class kdop_item_t* > kdop_items_2;                           // 0x02D8 (0x20)
        array_dyn_t< class traffic_u::item_in_cutscene_t > items_in_cutscenes;    // 0x02F8 (0x20)
        char pad_0318[ 56 ];                                                      // 0x0318 (0x38)
        array_dyn_t< class traffic_u::area_item_info_t > area_item_infos_1;       // 0x0350 (0x20)
        array_dyn_t< class traffic_u::area_item_info_t > area_item_infos_2;       // 0x0370 (0x20)
        char pad_0390[ 24 ];                                                      // 0x0390 (0x18)
        array_dyn_t< class traffic_u::sign_item_info_t* > sign_item_infos;        // 0x03A8 (0x20)
        array_dyn_t< class traffic_gate_t* > traffic_gates;                       // 0x03C8 (0x20)
        char pad_03E8[ 16 ];                                                      // 0x03E8 (0x10)
        array_dyn_t< class kdop_item_t* > kdop_items_3;                           // 0x03F8 (0x20)
        char pad_0418[ 16 ];                                                      // 0x0418 (0x10)
        array_dyn_t< class kdop_item_t* > kdop_items_4;                           // 0x0428 (0x20)
        array_dyn_t< class traffic_origin_t* > traffic_origins;                   // 0x0448 (0x20)
        char pad_0468[ 16 ];                                                      // 0x0468 (0x10)
        array_dyn_t< class traffic_spawn_point_t* > traffic_spawn_points;         // 0x0478 (0x20)
        char pad_0498[ 24 ];                                                      // 0x0498 (0x18)
        class city_data_u* city_data;                                             // 0x04B0 (0x08)
        array_dyn_t< uint32_t > N00001DA9;                                        // 0x04B8 (0x20)
        char pad_04D8[ 8 ];                                                       // 0x04D8 (0x08)
        array_dyn_t< class traffic_object_t* > traffic_objects_2;                 // 0x04E0 (0x20)
        array_dyn_t< class traffic_object_t* > traffic_objects_3;                 // 0x0500 (0x20)
        char pad_0520[ 120 ];                                                     // 0x0520 (0x78)
        array_dyn_t< class traffic_u::remote_clients_jip_context_t > N00001DC5;   // 0x0598 (0x20)
        char pad_05B8[ 128 ];                                                     // 0x05B8 (0x80)
    };
    static_assert( sizeof( traffic_u ) == 0x638 );

}

#pragma pack( pop )
