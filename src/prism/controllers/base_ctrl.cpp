#pragma once
#include "base_ctrl.hpp"

#include "memory/memory_utils.hpp"
#include "patterns.hpp"

namespace ets2la_plugin::prism
{

    uint64_t base_ctrl_u::instance_ptr_address               = 0;
    uint32_t base_ctrl_u::game_actor_offset                  = 0;
    uint32_t base_ctrl_u::some_nearby_kdop_items_offset      = 0;
    uint32_t base_ctrl_u::some_nearby_non_ai_vehicles_offset = 0;

    bool base_ctrl_u::scan_patterns()
    {
        auto addr = memory::get_address_for_pattern( patterns::base_ctrl );

        if ( addr == 0 )
        {
            throw std::runtime_error( "Failed to find base_ctrl pattern" );
        }
        base_ctrl_u::instance_ptr_address = addr + 3 + *reinterpret_cast< int32_t* >( addr + 3 ) + 4;
        base_ctrl_u::game_actor_offset    = *reinterpret_cast< uint32_t* >( addr + 17 );

        addr = memory::get_address_for_pattern( patterns::base_ctrl_u_some_nearby_kdop_items, 3 );

        if ( addr == 0 )
        {
            throw std::runtime_error( "Failed to find base_ctrl::some_nearby_kdop_items pattern" );
        }

        base_ctrl_u::some_nearby_kdop_items_offset = *reinterpret_cast< uint32_t* >( addr );

        const auto tmp_vehicles_addr =
            memory::get_address_for_pattern( patterns::base_ctrl_u_some_nearby_non_ai_vehicles, 8 );

        if ( tmp_vehicles_addr == 0 )
        {
            throw std::runtime_error( "Failed to find TMP vehicle list offset" );
        }
        base_ctrl_u::some_nearby_non_ai_vehicles_offset = *reinterpret_cast< uint32_t* >( tmp_vehicles_addr );

        return true;
    }

    base_ctrl_u* base_ctrl_u::get()
    {
        if ( base_ctrl_u::instance_ptr_address != 0 )
        {
            return *reinterpret_cast< base_ctrl_u** >( base_ctrl_u::instance_ptr_address );
        }
        return nullptr;
    }

    array_dyn_t< class kdop_item_t* >* base_ctrl_u::get_nearby_kdop_items() const
    {
        if ( base_ctrl_u::some_nearby_kdop_items_offset != 0 )
        {
            return reinterpret_cast< array_dyn_t< class kdop_item_t* >* >(
                reinterpret_cast< uint64_t >( this ) + base_ctrl_u::some_nearby_kdop_items_offset
            );
        }
        return nullptr;
    }

    // contains TruckerMP nearby players' trucks and trailers
    // also contains trucks and trailers in menus (background/service center/truck dealer/etc...)
    list_dyn_t< class vehicle_shared_u* >* base_ctrl_u::get_some_nearby_non_ai_vehicles_list() const
    {
        if ( base_ctrl_u::some_nearby_non_ai_vehicles_offset != 0 )
        {
            return reinterpret_cast< list_dyn_t< class vehicle_shared_u* >* >(
                reinterpret_cast< uint64_t >( this ) + base_ctrl_u::some_nearby_non_ai_vehicles_offset
            );
        }

        return nullptr;
    }
}
