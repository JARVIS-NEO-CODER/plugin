#pragma once

#include <cstdint>

namespace ets2la_plugin::prism { struct placement_t; }

namespace ets2la_plugin::examiner_camera_bridge
{
    void tick();
    void shutdown();

    // Select a live Convoy player vehicle by its in-process pointer.
    // The bridge resolves the pointer on each tick so the camera follows the
    // selected vehicle as it moves.
    void set_player_vehicle(std::uintptr_t vehicle_ptr);
    void clear_player_vehicle();
}
