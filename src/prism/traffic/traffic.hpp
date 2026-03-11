#pragma once
#include "prism/collections/array_dyn.hpp"
#include "prism/unit/unit.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    class traffic_u : public unit_t // Size: 0x05D8
    {
    public:
        char pad_0010[ 8 ];                                          //0x0010 (0x08)
        class traffic_data_u* traffic_data;                          //0x0018 (0x08)
        class color_array_u* color_array;                            //0x0020 (0x08)
        array_dyn_t< class vehicle_type_info_t > vehicle_type_infos; //0x0028 (0x20)
        char pad_0048[ 128 ];                                        //0x0048 (0x80)
        array_dyn_t< class spawned_vehicle_t > ai_vehicles;          //0x00C8 (0x20)
        char pad_00E8[ 168 ];                                        //0x00E8 (0xa8)
        array_dyn_t< class traffic_object_t* > traffic_objects;      //0x0190 (0x20)
        char pad_01B0[ 200 ];                                        //0x01B0 (0xc8)
        array_dyn_t< class traffic_segment_t* > traffic_segments;    //0x0278 (0x20)
        char pad_0298[ 304 ];                                        //0x0298 (0x130)
        array_dyn_t< class traffic_gate_t* > traffic_gates;          //0x03C8 (0x20)
        char pad_03E8[ 200 ];                                        //0x03E8 (0xc8)
        class city_data_u* city_data;                                //0x04B0 (0x08)
        char pad_04B8[ 288 ];                                        //0x04B8 (0x120)
    };
    static_assert( sizeof( traffic_u ) == 0x5D8 );

}

#pragma pack( pop )
