#pragma once
#include "./route_source.hpp"
#include "prism/collections/array_dyn.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    class gps_manager_t // Size: 0x0590
    {
    public:
        simple_route_source_t simple_route_source;              // 0x0008 (0x170)
        char pad_0178[ 64 ];                                    // 0x0178 (0x40)
        array_dyn_t< class route_task_node_t > start_point;     // 0x01B8 (0x20) not 100% sure but looks possible
        array_dyn_t< class route_task_node_t > waypoints;       // 0x01D8 (0x20)
        array_dyn_t< class route_task_node_t > avoid_waypoints; // 0x01F8 (0x20)
        char pad_0218[ 4 ];                                     // 0x0218 (0x04)
        float trip_distance;                                    // 0x021C (0x04) meters
        float trip_time;                                        // 0x0220 (0x04) seconds
        float next_checkpoint_distance;                         // 0x0224 (0x04) meters
        float next_checkpoint_time;                             // 0x0228 (0x04) seconds
        char pad_022C[ 868 ];                                   // 0x022C (0x364)

        virtual void destructor();

        static gps_manager_t* get();
    };

    static_assert( sizeof( gps_manager_t ) == 0x590 );
}

#pragma pack( pop )
