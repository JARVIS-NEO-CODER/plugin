#pragma once
#include "./traffic_physics_data.hpp"
#include "./traffic_vehicle.hpp"
#include "prism/common.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    class traffic_vehicle_physics_data : public traffic_dummy_vehicle_physics_data // Size: 0x0158
    {
    public:
        char pad_0108[ 80 ]; //0x0108 (0x50)
    };
    static_assert( sizeof( traffic_vehicle_physics_data ) == 0x158 );
#if defined( _WIN32 )

    class traffic_ai_vehicle_t // Size: 0x0550
    {
    public:
        char pad_0000[ 40 ];                        //0x0000 (0x28)
        placement_t placement;                      //0x0028 (0x20)
        aabox_t aabox;                              //0x0048 (0x18)
        char pad_0060[ 424 ];                       //0x0060 (0x1a8)
        traffic_vehicle_physics_data* physics_data; //0x0208 (0x08)
        char pad_0210[ 472 ];                       //0x0210 (0x1d8)
        class traffic_vehicle_u* traffic_vehicle;   //0x03E8 (0x08)
        class traffic_ai_trailer_t* trailer;        //0x03F0 (0x08)
        char pad_03F8[ 344 ];                       //0x03F8 (0x158)
    };
    static_assert( sizeof( traffic_ai_vehicle_t ) == 0x550 );

#else

    class traffic_ai_vehicle_t // Size: 0x0558
    {
    public:
        char pad_0000[ 40 ];                        //0x0000 (0x28)
        placement_t placement;                      //0x0028 (0x20)
        aabox_t aabox;                              //0x0048 (0x18)
        char pad_0060[ 432 ];                       //0x0060 (0x1b0)
        traffic_vehicle_physics_data* physics_data; //0x0210 (0x08)
        char pad_0218[ 472 ];                       //0x0218 (0x1d8)
        class traffic_vehicle_u* traffic_vehicle;   //0x03F0 (0x08)
        class traffic_ai_trailer_t* trailer;        //0x03F8 (0x08)
        char pad_0400[ 344 ];                       //0x0400 (0x158)
    };
    static_assert( sizeof( traffic_ai_vehicle_t ) == 0x558 );

#endif

    class spawned_vehicle_t // Size: 0x0010
    {
    public:
        traffic_ai_vehicle_t* vehicle; //0x0000 (0x08)
        char pad_0008[ 8 ];            //0x0008 (0x08)
    };
    static_assert( sizeof( spawned_vehicle_t ) == 0x10 );
}

#pragma pack( pop )
