#pragma once
#include "prism/token.hpp"
#include "prism/collections/array_dyn.hpp"
#include "prism/vehicles/core_vehicle.hpp"
#include <prism/common.hpp>

#pragma pack(push, 1)

namespace ets2la_plugin::prism
{
	class traffic_vehicle_u : public core_vehicle_u // Size: 0x0390
	{
	public:
		class array_dyn_t<float> lod_dist; //0x0160 (0x20)
		char pad_0180[16]; //0x0180 (0x10)
		class array_dyn_t<string_dyn_t> driver_model; //0x0190 (0x20)
		class array_dyn_t<float3_t> color; //0x01B0 (0x20)
		class array_dyn_t<string_dyn_t> trailer_chains; //0x01D0 (0x20)
		float max_speed; //0x01F0 (0x04)
		float engine_power; //0x01F4 (0x04)
		float deceleration_limit; //0x01F8 (0x04)
		float virtual_rear_wheels_offset; //0x01FC (0x04)
		float displacement_limit; //0x0200 (0x04)
		bool allow_parked; //0x0204 (0x01)
		char pad_0205[3]; //0x0205 (0x03)
		float spawn_ratio; //0x0208 (0x04)
		char pad_020C[4]; //0x020C (0x04)
		class array_dyn_t<traffic_vehicle_u*> variant; //0x0210 (0x20)
		class traffic_vehicle_u* base; //0x0230 (0x08)
		bool prefer_variant; //0x0238 (0x01)
		char pad_0239[7]; //0x0239 (0x07)
		class array_dyn_t<class traffic_spawn_conditions_u*> spawn_condition; //0x0240 (0x20)
		class array_dyn_t<token_t> tags; //0x0260 (0x20)
		token_t license_plate_type; //0x0280 (0x08)
		class array_dyn_t<string_dyn_t> sound_move; //0x0288 (0x20)
		class array_dyn_t<string_dyn_t> sound_horn; //0x02A8 (0x20)
		uint32_t sound_gears; //0x02C8 (0x04)
		float safe_distance; //0x02CC (0x04)
		float suspension_coef; //0x02D0 (0x04)
		float headlights_pitch; //0x02D4 (0x04)
		float headlights_offset; //0x02D8 (0x04)
		float3_t headlights_diffuse; //0x02DC (0x0c)
		float headlights_intensity_min; //0x02E8 (0x04)
		float headlights_intensity_max; //0x02EC (0x04)
		float2_t headlights_fadeout_range; //0x02F0 (0x08)
		float headlights_bias; //0x02F8 (0x04)
		float headlights_range; //0x02FC (0x04)
		float headlights_angle; //0x0300 (0x04)
		float headlights_aspect; //0x0304 (0x04)
		class string_dyn_t headlights_mask; //0x0308 (0x18)
		char pad_0320[8]; //0x0320 (0x08)
		class traffic_vehicle_type_u* traffic_vehicle_type; //0x0328 (0x08)
		class array_dyn_t<traffic_vehicle_u*> city_variant; //0x0330 (0x20)
		class array_dyn_t<string_dyn_t> attachable_trailer; //0x0350 (0x20)
		class array_dyn_t<uint32_t> trailer_count; //0x0370 (0x20)
	};
	static_assert(sizeof(traffic_vehicle_u) == 0x390);

}

#pragma pack(pop)
