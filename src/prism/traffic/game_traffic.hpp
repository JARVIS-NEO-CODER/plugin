#pragma once

#include "./traffic.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    // Size: 0x0658
    class game_traffic_u : public /* [0x638] @ 0x00 */ traffic_u
    {
    public:
        array_dyn_t< class traffic_world_rule_t* > traffic_world_rules_2; // 0x0638 (0x20)

        static uint64_t instance_ptr_address;

        static bool scan_patterns();
        static game_traffic_u* get();
    };
    static_assert( sizeof( game_traffic_u ) == 0x658 );
}

#pragma pack( pop )
