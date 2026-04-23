#pragma once

#include "prism/traffic/traffic_obstacle.hpp"
#include "prism/traffic/traffic_rule.hpp"

#include <cstdint>

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    // Size: 0x0068
    class traffic_semaphore_t : public /* [0x10] @ 0x00 */ traffic_rule_t,
                                public /* [0x08] @ 0x10 */ traffic_obstacle_t,
                                public /* [0x48] @ 0x18 */ traffic_item_rule_source_t
    {
    public:
        uint32_t state;     // 0x0060 (0x04)
        char pad_0064[ 4 ]; // 0x0064 (0x04)
    };
    static_assert( sizeof( traffic_semaphore_t ) == 0x68 );
}

#pragma pack( pop )
