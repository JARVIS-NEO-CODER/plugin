#pragma once
#include "game_traffic.hpp"

#include "memory/memory_utils.hpp"
#include "patterns.hpp"

namespace ets2la_plugin::prism
{
    uint64_t game_traffic_u::instance_ptr_address = 0;

    bool game_traffic_u::scan_patterns()
    {
        const auto addr = memory::get_address_for_pattern( patterns::game_traffic, 6 );

        if ( addr == 0 )
        {
            return false;
        }
        game_traffic_u::instance_ptr_address = addr + *reinterpret_cast< int32_t* >( addr ) + 4;

        return true;
    }

    game_traffic_u* game_traffic_u::get()
    {
        if ( game_traffic_u::instance_ptr_address != 0 )
        {
            return *reinterpret_cast< game_traffic_u** >( game_traffic_u::instance_ptr_address );
        }
        return nullptr;
    }
}
