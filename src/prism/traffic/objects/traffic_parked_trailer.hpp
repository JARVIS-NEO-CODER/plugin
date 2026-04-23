#pragma once

#include "prism/traffic/objects/traffic_parked_actor.hpp"
#include "prism/traffic/traffic_trailer_model.hpp"
#include "prism/traffic/traffic_trailer_physics.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{

    // Size: 0x00E8
    class traffic_parked_trailer_t : public /* [0x10] @ 0x00 */ traffic_trailer_model_object_t,
                                     public /* [0x28] @ 0x10 */ traffic_trailer_physics_object_t,
                                     public /* [0xB0] @ 0x38 */ traffic_parked_actor_t
    {
    public:
    };
#if defined( _WIN32 )
    static_assert( sizeof( traffic_parked_trailer_t ) == 0xE8 );
#else
    static_assert( sizeof( traffic_parked_trailer_t ) == 0xF0 );
#endif

}

#pragma pack( pop )
