#pragma once
#include "prism/common.hpp"
#include "./traffic_physics_data.hpp"

#pragma pack(push, 1)

namespace ets2la_plugin::prism
{
	class traffic_trailer_physics_data : public traffic_dummy_vehicle_physics_data // Size: 0x01D8
	{
	public:
		char pad_0108[88]; //0x0108 (0x58)
		class traffic_trailer_u* traffic_trailer; //0x0160 (0x08)
		class accessory_chassis_data_u* accessory_chassis_data; //0x0168 (0x08)
		char pad_0170[104]; //0x0170 (0x68)
	};
	static_assert(sizeof(traffic_trailer_physics_data) == 0x1D8);

	class traffic_ai_trailer_t // Size: 0x0328
	{
	public:
		char pad_0000[40]; //0x0000 (0x28)
		class placement_t placement; //0x0028 (0x20)
		char pad_0048[216]; //0x0048 (0xd8)
		class traffic_trailer_physics_data* physics_data; //0x0120 (0x08)
		char pad_0128[464]; //0x0128 (0x1d0)
		class traffic_trailer_u* traffic_trailer; //0x02F8 (0x08)
		class traffic_ai_trailer_t* slave_trailer; //0x0300 (0x08)
		char pad_0308[32]; //0x0308 (0x20)
	};
	static_assert(sizeof(traffic_ai_trailer_t) == 0x328);
}

#pragma pack(pop)
