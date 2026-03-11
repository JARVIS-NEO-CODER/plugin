#pragma once
#include "prism/collections/array_dyn.hpp"
#include "prism/unit/unit.hpp"
#include "prism/common.hpp"
#include "prism/token.hpp"
#include "./core_camera.hpp"

#pragma pack(push, 1)

namespace ets2la_plugin::prism
{
    class core_camera_u : public unit_t // Size: 0x0290
    {
    public:
        char pad_0010[ 4 ]; //0x0010 (0x04)
        float N0000CA51; //0x0014 (0x04)
        char pad_0018[ 4 ]; //0x0018 (0x04)
        float mouse_sensitivity; //0x001C (0x04)
        float camera_fov; //0x0020 (0x04) FOV that is listed in the F4 interior menu
        float near_plane; //0x0024 (0x04)
        float far_plane; //0x0028 (0x04)
        char pad_002C[ 12 ]; //0x002C (0x0c)
        float h_fov; //0x0038 (0x04) horizontal FOV
        float v_fov; //0x003C (0x04) vertical FOV
        class placement_t placement; //0x0040 (0x20)
        float4x4_t projection_matrix; //0x0060 (0x40)
        char pad_00A0[ 76 ]; //0x00A0 (0x4c)
        float shake_anim_step; //0x00EC (0x04)
        float shake_anim_scale_min; //0x00F0 (0x04)
        float shake_anim_scale_max; //0x00F4 (0x04)
        class array_dyn_t< float3_t > shake_anim; //0x00F8 (0x20)
        char pad_0118[ 12 ]; //0x0118 (0x0c)
        float3_t hand_shake_limit; //0x0124 (0x0c)
        char pad_0130[ 8 ]; //0x0130 (0x08)
        float hand_shake_speed; //0x0138 (0x04)
        char pad_013C[ 20 ]; //0x013C (0x14)
        token_t N00002261; //0x0150 (0x08)
        char pad_0158[ 40 ]; //0x0158 (0x28)
        token_t N00002267; //0x0180 (0x08)
        char pad_0188[ 40 ]; //0x0188 (0x28)
        token_t N0000226D; //0x01B0 (0x08)
        char pad_01B8[ 40 ]; //0x01B8 (0x28)
        token_t N00002273; //0x01E0 (0x08)
        char pad_01E8[ 32 ]; //0x01E8 (0x20)
        class input_object_def_t* inp_def; //0x0208 (0x08)
        token_t N00002279; //0x0210 (0x08)
        char pad_0218[ 40 ]; //0x0218 (0x28)
        token_t N0000227F; //0x0240 (0x08)
        char pad_0248[ 40 ]; //0x0248 (0x28)
        token_t N00002285; //0x0270 (0x08)
        char pad_0278[ 24 ]; //0x0278 (0x18)
    };

    static_assert(sizeof( core_camera_u ) == 0x290);
}

#pragma pack(pop)
