#pragma once

#include "prism/collections/array_dyn.hpp"
#include "prism/traffic/traffic_object.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    // Size: 0x0010
    class traffic_rule_t : public /* [0x10] @ 0x00 */ traffic_object_t
    {
    public:
    };
    static_assert( sizeof( traffic_rule_t ) == 0x10 );

    // Size: 0x0048
    class traffic_item_rule_source_t
    {
    public:
        array_dyn_t< class traffic_item_rule_link_t* > N00007A8F; // 0x0008 (0x20)
        array_dyn_t< class traffic_item_rule_link_t* > N00007AA3; // 0x0028 (0x20)

        virtual void destructor();
    };
    static_assert( sizeof( traffic_item_rule_source_t ) == 0x48 );

    // Size: 0x0038
    class traffic_item_rule_t : public /* [0x10] @ 0x00 */ traffic_rule_t,
                                public /* [0x28] @ 0x10 */ traffic_item_object_t
    {
    public:
    };
    static_assert( sizeof( traffic_item_rule_t ) == 0x38 );

    // Size: 0x0038
    class traffic_item_rule_link_t : public /* [0x38] @ 0x00 */ traffic_item_rule_t
    {
    public:
    };
    static_assert( sizeof( traffic_item_rule_link_t ) == 0x38 );

}

#pragma pack( pop )
