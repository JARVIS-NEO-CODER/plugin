#include "examiner_camera_bridge.hpp"

#include <cstdint>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#include "prism/camera/camera_manager.hpp"
#include "prism/camera/core_camera.hpp"
#include "prism/traffic/objects/traffic_player_vehicle.hpp"

namespace ets2la_plugin::examiner_camera_bridge
{
#pragma pack(push, 1)
struct Vec3 { float x, y, z; };
struct Quat { float x, y, z, w; };
struct Command
{
    uint32_t magic;
    uint32_t version;
    uint32_t active;
    int32_t target_vehicle_id;
    Vec3 position;
    Quat rotation;
    Vec3 look_target;
    float fov;
    uint32_t sequence;
};
#pragma pack(pop)

static constexpr uint32_t MAGIC = 0x31434345;
static constexpr char SHM_NAME[] = "Local\\ETS2LAECExaminerCamera";
static HANDLE mapping = nullptr;
static void* view = nullptr;
static uint32_t last_sequence = 0;
static prism::placement_t saved_placement{};
static float saved_fov = 0.0f;
static bool saved = false;
static std::uintptr_t selected_vehicle = 0;
static camera_mode selected_mode = camera_mode::rear;

static bool ensure_mapping()
{
    if (view != nullptr) return true;
    mapping = OpenFileMappingA(FILE_MAP_READ, FALSE, SHM_NAME);
    if (mapping == nullptr) return false;
    view = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, sizeof(Command));
    if (view == nullptr) { CloseHandle(mapping); mapping = nullptr; return false; }
    return true;
}

static bool read_command(Command& command)
{
    if (!ensure_mapping()) return false;
    Command snapshot{};
    std::memcpy(&snapshot, view, sizeof(snapshot));
    if (snapshot.magic != MAGIC || snapshot.version != 1 || snapshot.sequence == last_sequence) return false;
    command = snapshot;
    last_sequence = snapshot.sequence;
    return true;
}

static void restore(prism::core_camera_u* camera)
{
    if (camera == nullptr || !saved) return;
    camera->placement = saved_placement;
    camera->camera_fov = saved_fov;
    saved = false;
}

void set_player_vehicle(std::uintptr_t vehicle_ptr) { selected_vehicle = vehicle_ptr; }
void clear_player_vehicle() { selected_vehicle = 0; }
void set_camera_mode(camera_mode mode) { selected_mode = mode; }
camera_mode get_camera_mode() { return selected_mode; }

static bool apply_player_camera(prism::core_camera_u* camera)
{
    if (camera == nullptr || selected_vehicle == 0) return false;
    auto* vehicle = reinterpret_cast<prism::traffic_player_vehicle_t*>(selected_vehicle);
    if (vehicle == nullptr) return false;

    if (!saved) {
        saved_placement = camera->placement;
        saved_fov = camera->camera_fov;
        saved = true;
    }

    const auto base = vehicle->placement;
    prism::float3_t offset{};
    switch (selected_mode)
    {
        case camera_mode::cabin:
            offset = prism::float3_t{0.0f, 2.2f, 0.4f};
            camera->placement.pos = base.to_global_position() + offset.rotate(base.rot);
            camera->placement.rot = base.rot;
            camera->camera_fov = 70.0f;
            break;
        case camera_mode::rear:
            offset = prism::float3_t{-2.5f, 3.0f, -9.0f};
            camera->placement.pos = base.to_global_position() + offset.rotate(base.rot);
            camera->placement.rot = base.rot;
            camera->camera_fov = 65.0f;
            break;
        case camera_mode::free:
            offset = prism::float3_t{7.0f, 5.0f, -10.0f};
            camera->placement.pos = base.to_global_position() + offset.rotate(base.rot);
            camera->placement.rot = base.rot;
            camera->camera_fov = 70.0f;
            break;
        case camera_mode::overhead:
            offset = prism::float3_t{0.0f, 16.0f, -2.0f};
            camera->placement.pos = base.to_global_position() + offset.rotate(base.rot);
            camera->placement.rot = base.rot;
            camera->camera_fov = 75.0f;
            break;
    }
    return true;
}

void tick()
{
    auto* manager = prism::camera_manager_u::get();
    if (manager == nullptr || manager->current_camera >= manager->cameras.size) return;
    auto* camera = manager->cameras[manager->current_camera];
    if (camera == nullptr) return;

    if (selected_vehicle != 0) {
        apply_player_camera(camera);
        return;
    }

    Command command{};
    if (!read_command(command)) return;
    if (command.active == 0) { restore(camera); return; }

    if (!saved) { saved_placement = camera->placement; saved_fov = camera->camera_fov; saved = true; }
    camera->placement.pos.x = command.position.x;
    camera->placement.pos.y = command.position.y;
    camera->placement.pos.z = command.position.z;
    camera->placement.rot.x = command.rotation.x;
    camera->placement.rot.y = command.rotation.y;
    camera->placement.rot.z = command.rotation.z;
    camera->placement.rot.w = command.rotation.w;
    camera->camera_fov = command.fov;
}

void shutdown()
{
    selected_vehicle = 0;
    if (view != nullptr) { UnmapViewOfFile(view); view = nullptr; }
    if (mapping != nullptr) { CloseHandle(mapping); mapping = nullptr; }
    saved = false;
    last_sequence = 0;
    selected_mode = camera_mode::rear;
}
}
#else
namespace ets2la_plugin::examiner_camera_bridge
{
void tick() {}
void shutdown() {}
void set_player_vehicle(std::uintptr_t) {}
void clear_player_vehicle() {}
void set_camera_mode(camera_mode) {}
camera_mode get_camera_mode() { return camera_mode::rear; }
}
#endif
