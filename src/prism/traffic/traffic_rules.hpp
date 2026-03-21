#pragma once

#include "prism/collections/array_dyn.hpp"
#include <cstdint>

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    // Size: 0x0068
    class traffic_rule_t : public /* [0x10] @ 0x00 */ traffic_object_t
    {
    public:
        char pad_0010[ 16 ];                                     //0x0010 (0x10)
        array_dyn_t< class traffic_item_rule_link_t > N00006B45; //0x0020 (0x20)
        char pad_0040[ 32 ];                                     //0x0040 (0x20)
        uint32_t state;                                          //0x0060 (0x04)
        char pad_0064[ 4 ];                                      //0x0064 (0x04)
    };
    static_assert( sizeof( traffic_rule_t ) == 0x68 );

    class traffic_light_t : public traffic_rule_t // Size: 0x00A8
    {
    public:
        char pad_0068[ 56 ];        //0x0068 (0x38)
        float state_time_remaining; //0x00A0 (0x04)
        char pad_00A4[ 4 ];         //0x00A4 (0x04)

        static constexpr uint64_t ID = 0x2000;
    };
    static_assert( sizeof( traffic_light_t ) == 0xA8 );
}

#pragma pack( pop )
