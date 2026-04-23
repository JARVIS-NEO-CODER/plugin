#pragma once

#include "prism/traffic/objects/traffic_item_passive_actor.hpp"
#include "prism/traffic/traffic_trailer_model.hpp"
#include "prism/traffic/traffic_trailer_physics.hpp"
#include "prism/traffic/traffic_vehicle_mp.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    // Size: 0x0330 NOTE: different linux vs windows
    class traffic_ai_trailer_t : public /* [0xF0] @ 0x00 */ traffic_item_passive_actor_t,
                                 public /* [0x10] @ 0xF0 */ traffic_trailer_model_object_t,
                                 public /* [0x30] @ 0x100 */ traffic_trailer_physics_object_t,
                                 public /* [0x1D0] @ 0x130 */ traffic_vehicle_mp_t
    {
    public:
        class traffic_trailer_u* traffic_trailer; // 0x0300 (0x08)
        traffic_ai_trailer_t* slave_trailer;      // 0x0308 (0x08)
        char pad_0310[ 32 ];                      // 0x0310 (0x20)
    };
#if defined( _WIN32 )
    static_assert( sizeof( traffic_ai_trailer_t ) == 0x328 );
#else
    static_assert( sizeof( traffic_ai_trailer_t ) == 0x330 );
#endif
}

#pragma pack( pop )
