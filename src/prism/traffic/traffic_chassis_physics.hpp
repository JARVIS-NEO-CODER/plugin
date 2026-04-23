#pragma once

#include "prism/collections/array_dyn.hpp"
#include "prism/common.hpp"
#include "prism/traffic/traffic_physics_shared.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{

    // Size: 0x0028
    class traffic_chassis_physics_object_t : public /* [0x28] @ 0x00 */ traffic_physics_object_t
    {
    public:
    };
#if defined( _WIN32 )
    static_assert( sizeof( traffic_chassis_physics_object_t ) == 0x20 );
#else
    static_assert( sizeof( traffic_chassis_physics_object_t ) == 0x28 );
#endif

    // Size: 0x0128
    class traffic_chassis_physics_t : public /* [0x20] @ 0x00 */ traffic_physics_t
    {
        class wheel_t;

    public:
        class physics_actor_t* physics_actor;                        // 0x0020 (0x08)
        array_dyn_t< class physics_shape_t* > physics_shapes;        // 0x0028 (0x20)
        char pad_0048[ 32 ];                                         // 0x0048 (0x20)
        float speed;                                                 // 0x0068 (0x04)
        float acceleration;                                          // 0x006C (0x04)
        float3_t bounding_box;                                       // 0x0070 (0x0c)
        float3_t N0000BBCB;                                          // 0x007C (0x0c)
        array_dyn_t< traffic_chassis_physics_t::wheel_t > N0000BBCC; // 0x0088 (0x20)
        char pad_00A8[ 8 ];                                          // 0x00A8 (0x08)
        array_dyn_t< uint32_t > N0000BBD1;                           // 0x00B0 (0x20)
        float speed2;                                                // 0x00D0 (0x04)
        char pad_00D4[ 84 ];                                         // 0x00D4 (0x54)
    };
    static_assert( sizeof( traffic_chassis_physics_t ) == 0x128 );

}

#pragma pack( pop )
