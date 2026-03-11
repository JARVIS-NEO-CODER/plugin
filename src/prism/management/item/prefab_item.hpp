#pragma once

#include "./kdop_item.hpp"
#include "prism/collections/array_dyn.hpp"
#include "prism/token.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    class prefab_item_t : public kdop_item_t // Size: 0x01B0
    {
    public:
        class segment_t* segment;                       //0x0048 (0x08)
        char pad_0050[ 16 ];                            //0x0050 (0x10)
        class node_item_t* nodes[ 6 ];                  //0x0060 (0x30)
        class prefab_model_u* prefab_model;             //0x0090 (0x08)
        token_t prefab_variant;                         //0x0098 (0x08)
        token_t semaphore_profile;                      //0x00A0 (0x08)
        array_dyn_t< token_t > N00006FE5;               //0x00A8 (0x20)
        class terrain_profile_u* terrain_profiles[ 6 ]; //0x00C8 (0x30)
        float terrain_profile_coefs[ 6 ];               //0x00F8 (0x18)
        char pad_0110[ 8 ];                             //0x0110 (0x08)
        uint64_t origin_idx;                            //0x0118 (0x08)
        char pad_0120[ 144 ];                           //0x0120 (0x90)
    };

    static_assert( sizeof( prefab_item_t ) == 0x1B0 );
}

#pragma pack( pop )
