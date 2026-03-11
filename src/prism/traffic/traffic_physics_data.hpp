#pragma once
#include "prism/common.hpp"

#pragma pack(push, 1)

namespace ets2la_plugin::prism
{
    class traffic_core_vehicle_physics_data // Size: 0x0050
    {
    public:
        char pad_0008[ 24 ]; //0x0008 (0x18)
        class physics_actor_t* physics_actor; //0x0020 (0x08)
        char pad_0028[ 40 ]; //0x0028 (0x28)

        virtual void destructor();
    };

    static_assert(sizeof( traffic_core_vehicle_physics_data ) == 0x50);

    class traffic_dummy_vehicle_physics_data : public traffic_core_vehicle_physics_data // Size: 0x0108
    {
    public:
        char pad_0050[ 24 ]; //0x0050 (0x18)
        float speed; //0x0068 (0x04)
        float acceleration; //0x006C (0x04)
        float3_t bounding_box; //0x0070 (0x0c) (width, height, length)
        char pad_007C[ 140 ]; //0x007C (0x8c)
    };

    static_assert(sizeof( traffic_dummy_vehicle_physics_data ) == 0x108);
}

#pragma pack(pop)
