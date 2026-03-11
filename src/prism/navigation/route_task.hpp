#pragma once

#include "prism/collections/array_dyn.hpp"
#include "prism/common.hpp"
#include "prism/thread/task.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    class route_task_node_t // Size: 0x0018
    {
    public:
        uint32_t unk0;           // 0x0000 (0x04)
        uint32_t unk1;           // 0x0004 (0x04)
        class node_item_t* node; // 0x0008 (0x08)
        uint32_t unk2;           // 0x0010 (0x04)
        uint32_t unk3;           // 0x0014 (0x04)
    };
    static_assert( sizeof( route_task_node_t ) == 0x18 );

    class route_item_t // Size: 0x0030
    {
    public:
        uint32_t type;                 // 0x0000 (0x04) 0 = ptr is node_item_t; 1 = ptr is prefab_item_t
        uint32_t node_id;              // 0x0004 (0x04)
        class node_item_t* item_ptr;   // 0x0008 (0x08) node_item_t or prefab_item_t
        char pad_0010[ 8 ];            // 0x0010 (0x08)
        float distance_from_previous;  // 0x0018 (0x04) meters
        float time_from_previous;      // 0x001C (0x04) seconds
        uint16_t N00007113;            // 0x0020 (0x02)
        uint16_t N000070E6;            // 0x0022 (0x02)
        float total_distance_till_end; // 0x0024 (0x04) meters
        float total_time_till_end;     // 0x0028 (0x04) seconds
        char pad_002C[ 4 ];            // 0x002C (0x04)
    };
    static_assert( sizeof( route_item_t ) == 0x30 );

    class physical_route_item_t // Size: 0x0020
    {
    public:
        class node_item_t* node;       // 0x0000 (0x08)
        char pad_0008[ 4 ];            // 0x0008 (0x04)
        float distance_from_previous;  // 0x000C (0x04) meters
        float time_from_previous;      // 0x0010 (0x04) seconds
        float total_distance_till_end; // 0x0014 (0x04) meters
        float total_time_till_end;     // 0x0018 (0x04) seconds
        char pad_001C[ 4 ];            // 0x001C (0x04)
    };
    static_assert( sizeof( physical_route_item_t ) == 0x20 );

    // Size: 0x0138 1.58
    class route_task_t : public /* [0x38] @ 0x00 */ thread::task_t
    {
    public:
        char pad_0038[ 24 ];                                        // 0x0038 (0x18)
        array_dyn_t< physical_route_item_t > physical_route_items;  // 0x0050 (0x20) node to node
        array_dyn_t< route_item_t > route_items;                    // 0x0070 (0x20)
        char pad_0090[ 40 ];                                        // 0x0090 (0x28)
        array_dyn_t< class private_route_task_target_t > N000071A4; // 0x00B8 (0x20)
        char pad_00D8[ 24 ];                                        // 0x00D8 (0x18)
        array_dyn_t< uint32_t > N00007011;                          // 0x00F0 (0x20)
        char pad_0110[ 8 ];                                         // 0x0110 (0x08)
        array_dyn_t< uint64_t > N00007013;                          // 0x0118 (0x20)
    };
    static_assert( sizeof( route_task_t ) == 0x138 );

}

#pragma pack( pop )
