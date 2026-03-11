#pragma once

#include "prism/collections/array_dyn.hpp"
#include <cstdint>

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    class traffic_rule_t // Size: 0x0068
    {
    public:
        char pad_0008[ 24 ];                                     //0x0008 (0x18)
        array_dyn_t< class traffic_item_rule_link_t > N00006B45; //0x0020 (0x20)
        char pad_0040[ 32 ];                                     //0x0040 (0x20)
        uint32_t state;                                          //0x0060 (0x04)
        char pad_0064[ 4 ];                                      //0x0064 (0x04)

        virtual void destructor();

        // traffic_light = 0x2000, automatic_barrier = 0x3000, crossing_object_barrier = 0x4000 ?
        virtual uint64_t get_type() const;
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
