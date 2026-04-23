#pragma once

namespace ets2la_plugin::patterns
{

    namespace base_ctrl
    {
        inline constexpr auto pattern           = "48 8b 05 ? ? ? ? 48 8b ? 48 8b 49 ? 48 8b 80";
        inline constexpr auto offset_instance   = 3;
        inline constexpr auto offset_game_actor = 17;

        namespace nearby_non_ai_vehicles
        {
            inline constexpr auto pattern = "f0 0f c1 42 ? 48 8d b9 ? ? ? ? 48 8b 07";
            inline constexpr auto offset  = 8;
        }

        namespace nearby_kdop_items
        {
            inline constexpr auto pattern = "49 8d 8e ? ? ? ? 48 89 45 ? e8 ? ? ? ? 49 8b 86";
            inline constexpr auto offset  = 3;
        }

    }

    namespace game_ctrl
    {
        namespace gps_manager
        {
            inline constexpr auto pattern = "48 8d b7 ? ? ? ? 0f 57 c9";
            inline constexpr auto offset  = 3;
        }
    }

    namespace camera_manager
    {
        inline constexpr auto pattern         = "48 8b 05 ? ? ? ? 48 8b fa 48 85 c0 0f";
        inline constexpr auto offset_instance = 3;
    }

    namespace game_traffic
    {
        inline constexpr auto pattern         = "48 8b d9 48 8b 0d ? ? ? ? 48 85 c9 74 ? 48 8b 83 ? ? ? ? 48 85 c0";
        inline constexpr auto offset_instance = 6;
    }

    namespace vehicle_shared
    {
        namespace steering
        {
            inline constexpr auto pattern = "eb ? f3 0f 10 ? ? ? ? ? f3 0f 5c ? ? ? ? ? 0f 2f d3";
            inline constexpr auto offset  = 6;
        }
    }

    namespace game_actor
    {
        namespace throttle_input
        {
            inline constexpr auto pattern               = "48 b8 3F 7C FD CC 88 01 00 00 48 8d 8f";
            inline constexpr auto offset                = 13;
            inline constexpr auto offset_value_modifier = 0;
        }
        namespace brake_input
        {
            inline constexpr auto pattern               = "48 b8 C7 91 B7 6C 4E 3A 00 00 48 8d 8f";
            inline constexpr auto offset                = 13;
            inline constexpr auto offset_value_modifier = 0;
        }
    }

    namespace game_physics_vehicle
    {
        namespace trailer
        {
            inline constexpr auto pattern = "4c 8b 71 ? 49 8b 8e ? ? ? ? 48 85 c9";
            inline constexpr auto offset  = 7;
        }
    }

    namespace game_trailer_actor
    {
        namespace slave_trailer
        {
            inline constexpr auto pattern = "48 8b 86 ? ? ? ? 48 85 c0 74 ? 48 8b fe 48 8b f0";
            inline constexpr auto offset  = 3;
        }
    }

}
