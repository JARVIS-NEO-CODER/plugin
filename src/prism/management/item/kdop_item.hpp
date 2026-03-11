#pragma once

#include <cstdint>

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    class kdop_item_t // Size: 0x0048
    {
    public:
        uint8_t view_dist;   //0x0008 (0x01)
        char pad_0009[ 1 ];  //0x0009 (0x01)
        uint8_t item_type;   //0x000A (0x01)
        char pad_000B[ 1 ];  //0x000B (0x01)
        float minimums[ 5 ]; //0x000C (0x14)
        float maximums[ 5 ]; //0x0020 (0x14)
        uint32_t flags;      //0x0034 (0x04)
        char pad_0038[ 8 ];  //0x0038 (0x08)
        uint64_t uid;        //0x0040 (0x08)

        virtual void destructor();
    };

    static_assert( sizeof( kdop_item_t ) == 0x48 );
}

#pragma pack( pop )
