#pragma once

#include "./traffic_semaphore.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    // Size: 0x00A8
    class traffic_light_t : public /* [0x68] @ 0x00 */ traffic_semaphore_t
    {
    public:
        char pad_0068[ 56 ];        // 0x0068 (0x38)
        float state_time_remaining; // 0x00A0 (0x04)
        char pad_00A4[ 4 ];         // 0x00A4 (0x04)

        struct ELightState
        {
            enum
            {
                OFF             = 0,
                ORANGE_TO_RED   = 1 << 0,
                RED             = 1 << 1,
                ORANGE_TO_GREEN = 1 << 2,
                GREEN           = 1 << 3,
                SLEEP           = 1 << 5,
            };
        };
    };
    static_assert( sizeof( traffic_light_t ) == 0xA8 );
}

#pragma pack( pop )
