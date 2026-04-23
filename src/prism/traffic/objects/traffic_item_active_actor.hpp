#pragma once

#include "prism/collections/list_dyn.hpp"
#include "prism/traffic/objects/traffic_item_actor.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    // Size: 0x01D8
    class traffic_item_active_actor_t : public /* [0xA8] @ 0x00 */ traffic_item_actor_t
    {
        class path_item_t;
        class path_rule_t;

    public:
        char pad_00A8[ 72 ];                                                      // 0x00A8 (0x48)
        list_dyn_t< class traffic_item_active_actor_t::path_item_t* > path_items; // 0x00F0 (0x58)
        char pad_0148[ 56 ];                                                      // 0x0148 (0x38)
        list_dyn_t< class traffic_item_active_actor_t::path_rule_t* > path_rules; // 0x0180 (0x58)
    };
    static_assert( sizeof( traffic_item_active_actor_t ) == 0x1D8 );

}

#pragma pack( pop )
