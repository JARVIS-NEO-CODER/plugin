#pragma once

#include "prism/collections/array_dyn.hpp"
#include <cstdint>

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    class segment_t // Size: 0x0320
    {
    public:
        char pad_0008[ 32 ];                                           //0x0008 (0x20)
        class kdop_item_t* kdop;                                       //0x0028 (0x08)
        char pad_0030[ 64 ];                                           //0x0030 (0x40)
        array_dyn_t< class semaphore_instance_t > semaphore_instances; //0x0070 (0x20)
        char pad_0090[ 656 ];                                          //0x0090 (0x290)

        virtual void destructor();
    };

    static_assert( sizeof( segment_t ) == 0x320 );
}

#pragma pack( pop )
