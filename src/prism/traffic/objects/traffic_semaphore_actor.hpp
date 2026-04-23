#pragma once

#include "prism/sound/sound_player_semaphore.hpp"
#include "prism/traffic/objects/traffic_actor.hpp"
#include "prism/traffic/traffic_model_shared.hpp"
#include "prism/traffic/traffic_physics_shared.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{

    // Size: 0x01E0
    class traffic_semaphore_actor_t : public /* [0x80] @ 0x00 */ traffic_actor_t,
                                      public /* [0x20] @ 0x80 */ traffic_model_t,
                                      public /* [0x28] @ 0xA0 */ traffic_physics_object_t
    {
        class collision_group_t;

    public:
        class semaphore_model_u* semaphore_model;                       // 0x00C8 (0x08)
        char pad_00D0[ 16 ];                                            // 0x00D0 (0x10)
        class game_traffic_model_effects_t* game_traffic_model_effects; // 0x00E0 (0x08)
        class model_object_u* model_object;                             // 0x00E8 (0x08)
        char pad_00F0[ 24 ];                                            // 0x00F0 (0x18)
        class traffic_light_t* traffic_rule;                            // 0x0108 (0x08)
        uint32_t state;               // 0x0110 (0x04) 2 = opening; 3 = open; 0 = closing; 1 = closed
        char pad_0114[ 12 ];          // 0x0114 (0x0c)
        float time_to_open;           // 0x0120 (0x04)
        float open_time_remaining;    // 0x0124 (0x04) used for double animation
        char pad_0128[ 8 ];           // 0x0128 (0x08)
        float time_to_close;          // 0x0130 (0x04)
        float close_time_remaining;   // 0x0134 (0x04) used for double animation
        char pad_0138[ 6 ];           // 0x0138 (0x06)
        uint8_t animation_mode;       // 0x013E (0x01)
        uint8_t coll_mode;            // 0x013F (0x01)
        char pad_0140[ 4 ];           // 0x0140 (0x04)
        float animation_time_elapsed; // 0x0144 (0x04) used for single animation
        uint8_t N00007222;            // 0x0148 (0x01)
        uint8_t N0000723A;            // 0x0149 (0x01)
        char pad_014A[ 6 ];           // 0x014A (0x06)
        array_dyn_t< class traffic_semaphore_actor_t::collision_group_t > collision_groups; // 0x0150 (0x20)
        array_dyn_t< class physics_shape_t* > physics_shapes;                               // 0x0170 (0x20)
        char pad_0190[ 8 ];                                                                 // 0x0190 (0x08)
        sound_player_semaphore_t sound_player_semaphore;                                    // 0x0198 (0x48);
    };
#if defined( _WIN32 )
    static_assert( sizeof( traffic_semaphore_actor_t ) == 0x1D8 );
#else
    static_assert( sizeof( traffic_semaphore_actor_t ) == 0x1E0 );
#endif
}

#pragma pack( pop )
