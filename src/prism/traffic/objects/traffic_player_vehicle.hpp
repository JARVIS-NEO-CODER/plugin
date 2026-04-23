#pragma once

#include "prism/collections/array_dyn.hpp"
#include "prism/traffic/objects/traffic_item_active_actor.hpp"
#include "prism/unit/unit.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{

    // Size: 0x02A8
    class traffic_player_vehicle_t : public /* [0x1D8] @ 0x00 */ traffic_item_active_actor_t
    {
        class crashed_actor_t;
        class red_light_offence_check_t;
        class navigation_t;

    public:
        traffic_player_base_object_t_templ< unit_t >*
            traffic_player_object;                          // 0x01D8 (0x08) remote_vehicle_base_u|physics_vehicle_u
        class traffic_player_trailer_t* trailer;            // 0x01E0 (0x08)
        float speed;                                        // 0x01E8 (0x04)
        float acceleration;                                 // 0x01EC (0x04)
        char pad_01F0[ 16 ];                                // 0x01F0 (0x10)
        traffic_player_vehicle_t::navigation_t* navigation; // 0x0200 (0x08)
        char pad_0208[ 16 ];                                // 0x0208 (0x10)
        class dangerous_actor_t* dangerous_actor;           // 0x0218 (0x08)
        char pad_0220[ 64 ];                                // 0x0220 (0x40)
        array_dyn_t< class traffic_player_vehicle_t::crashed_actor_t > N0000BA65;           // 0x0260 (0x20)
        array_dyn_t< class traffic_player_vehicle_t::red_light_offence_check_t > N0000BA69; // 0x0280 (0x20)
        char pad_02A0[ 8 ];                                                                 // 0x02A0 (0x08)
    };
    static_assert( sizeof( traffic_player_vehicle_t ) == 0x2A8 );

}

#pragma pack( pop )
