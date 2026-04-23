#pragma once
#include "prism/collections/array_dyn.hpp"
#include "prism/string.hpp"
#include "prism/traffic/objects/traffic_item_active_actor.hpp"
#include "prism/traffic/traffic_ai_task.hpp"
#include "prism/traffic/traffic_vehicle_model.hpp"
#include "prism/traffic/traffic_vehicle_mp.hpp"
#include "prism/traffic/traffic_vehicle_physics.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    // Size: 0x0560
    class traffic_ai_vehicle_t : public /* [0x1D8] @ 0x00 */ traffic_item_active_actor_t,
                                 public /* [0x10] @ 0x1D8 */ traffic_vehicle_model_object_t,
                                 public /* [0x30] @ 0x1E8 */ traffic_vehicle_physics_object_t,
                                 public /* [0x08] @ 0x218 */ traffic_ai_task_handler_t,
                                 public /* [0x1D0] @ 0x220 */ traffic_vehicle_mp_t
    {
    public:
        class traffic_vehicle_u* traffic_vehicle;           // 0x03F0 (0x08)
        class traffic_ai_trailer_t* trailer;                // 0x03F8 (0x08)
        char pad_0400[ 160 ];                               // 0x0400 (0xa0)
        array_dyn_t< class traffic_ai_task_t* > ai_tasks;   // 0x04A0 (0x20)
        char pad_04C0[ 8 ];                                 // 0x04C0 (0x08)
        class traffic_navigation_t* traffic_navigation;     // 0x04C8 (0x08)
        class sound_player_traffic_vehicle_t* sound_player; // 0x04D0 (0x08)
        char pad_04D8[ 16 ];                                // 0x04D8 (0x10)
        string_dyn_t vehicle_id_name;                       // 0x04E8 (0x18)
        string_dyn_t vehicle_type_name;                     // 0x0500 (0x18)
        char pad_0518[ 72 ];                                // 0x0518 (0x48)
    };
#if defined( _WIN32 )
    static_assert( sizeof( traffic_ai_vehicle_t ) == 0x558 );
#else
    static_assert( sizeof( traffic_ai_vehicle_t ) == 0x560 );
#endif

}

#pragma pack( pop )
