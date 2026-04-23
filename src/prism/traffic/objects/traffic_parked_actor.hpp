#pragma once

#include "prism/traffic/objects/traffic_actor.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    // Size: 0x00B0
    class traffic_parked_actor_t : public /* [0x80] @ 0x00 */ traffic_actor_t
    {
    public:
        class parked_model_base_u* parked_model_base;  // 0x0080 (0x08)
        class traffic_parked_trailer_t* slave_trailer; // 0x0088 (0x08)
        char pad_0090[ 24 ];                           // 0x0090 (0x18)
        class traffic_vehicle_u* traffic_vehicle;      // 0x00A8 (0x08)
    };
    static_assert( sizeof( traffic_parked_actor_t ) == 0xB0 );
}

#pragma pack( pop )
