#pragma once

#include "prism/collections/array_dyn.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    // Size: 0x0048
    class sound_player_semaphore_t
    {
    public:
        char pad_0008[ 32 ];                                      // 0x0008 (0x20)
        array_dyn_t< class sound_semaphore_item_t* > sound_items; // 0x0028 (0x20)

        virtual void destructor();
    };
    static_assert( sizeof( sound_player_semaphore_t ) == 0x48 );
}

#pragma pack( pop )
