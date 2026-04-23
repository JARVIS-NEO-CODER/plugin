#pragma once

#include "prism/traffic/traffic_chassis_physics.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{

    // Size: 0x0200
    class traffic_trailer_physics_t : public /* [0x128] @ 0x00 */ traffic_chassis_physics_t
    {
    public:
        char pad_0128[ 8 ];                                                   // 0x0128 (0x08)
        array_dyn_t< float > N0000BCA6;                                       // 0x0130 (0x20)
        char pad_0150[ 72 ];                                                  // 0x0150 (0x48)
        array_dyn_t< class physics_shape_t* > physics_shapes;                 // 0x0198 (0x20)
        array_dyn_t< class physics_joint_t* > physics_joints;                 // 0x01B8 (0x20)
        array_dyn_t< class physics_actor_dynamic_t* > physics_actors_dynamic; // 0x01D8 (0x20)
        char pad_01F8[ 8 ];                                                   // 0x01F8 (0x08)
    };
    static_assert( sizeof( traffic_trailer_physics_t ) == 0x200 );

    // Size: 0x0030
    class traffic_trailer_physics_object_t : public /* [0x28] @ 0x00 */ traffic_chassis_physics_object_t
    {
    public:
        traffic_trailer_physics_t* physics; // 0x0028 (0x08)
    };
#if defined( _WIN32 )
    static_assert( sizeof( traffic_trailer_physics_object_t ) == 0x28 );
#else
    static_assert( sizeof( traffic_trailer_physics_object_t ) == 0x30 );
#endif

}

#pragma pack( pop )
