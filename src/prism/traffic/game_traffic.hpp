#pragma once

#include "./traffic.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    class game_traffic_u : public traffic_u // Size: 0x05F8
    {
    public:
        char pad_05D8[ 32 ]; //0x05D8 (0x20)

        static uint64_t instance_ptr_address;

        static bool scan_patterns();
        static game_traffic_u* get();
    };
    static_assert( sizeof( game_traffic_u ) == 0x5F8 );
}

#pragma pack( pop )
