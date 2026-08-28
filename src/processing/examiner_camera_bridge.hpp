#pragma once

#include <cstdint>

namespace ets2la_plugin::prism { struct placement_t; }

namespace ets2la_plugin::examiner_camera_bridge
{
    enum class camera_mode : uint32_t
    {
        cabin = 1,
        rear = 2,
        free = 3,
        overhead = 4
    };

    void tick();
    void shutdown();
    void set_player_vehicle(std::uintptr_t vehicle_ptr);
    void clear_player_vehicle();
    void set_camera_mode(camera_mode mode);
    camera_mode get_camera_mode();
}
