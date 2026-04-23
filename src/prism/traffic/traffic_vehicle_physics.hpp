#pragma once

#include "prism/traffic/traffic_chassis_physics.hpp"
#include "prism/traffic/traffic_physics_shared.hpp"

namespace ets2la_plugin::prism
{
    // Size: 0x0180
    class traffic_vehicle_physics_t : public /* [0x128] @ 0x00 */ traffic_chassis_physics_t
    {
    public:
        char pad_0128[ 24 ];                                    // 0x0128 (0x18)
        array_dyn_t< float > N0000BBE3;                         // 0x0140 (0x20)
        char pad_0160[ 16 ];                                    // 0x0160 (0x10)
        class traffic_vehicle_u* traffic_vehicle;               // 0x0170 (0x08)
        class accessory_chassis_data_u* accessory_chassis_data; // 0x0178 (0x08)
    };
    static_assert( sizeof( traffic_vehicle_physics_t ) == 0x180 );

    // Size: 0x0030
    class traffic_vehicle_physics_object_t : public /* [0x28] @ 0x00 */ traffic_chassis_physics_object_t
    {
    public:
        traffic_vehicle_physics_t* physics; // 0x0028 (0x08)
    };
#if defined( _WIN32 )
    static_assert( sizeof( traffic_vehicle_physics_object_t ) == 0x28 );
#else
    static_assert( sizeof( traffic_vehicle_physics_object_t ) == 0x30 );
#endif
}
