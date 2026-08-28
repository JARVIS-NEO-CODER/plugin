#include "examiner_camera_bridge.hpp"
#include <cstdint>
#include <cstring>
#ifdef _WIN32
#include <windows.h>
#include "prism/common.hpp"
#include "prism/camera/camera_manager.hpp"
#include "prism/camera/core_camera.hpp"
#include "prism/traffic/objects/traffic_player_vehicle.hpp"
namespace ets2la_plugin::examiner_camera_bridge {
#pragma pack(push, 1)
struct Vec3 { float x,y,z; }; struct Quat { float x,y,z,w; };
struct Command { uint32_t magic,version,active; int32_t target_vehicle_id; Vec3 position; Quat rotation; Vec3 look_target; float fov; uint32_t sequence; };
#pragma pack(pop)
static constexpr uint32_t MAGIC=0x31434345; static constexpr char SHM_NAME[]="Local\\ETS2LAECExaminerCamera";
static HANDLE mapping=nullptr; static void* view=nullptr; static uint32_t last_sequence=0; static prism::placement_t saved_placement{}; static float saved_fov=0; static bool saved=false; static std::uintptr_t selected_vehicle=0; static camera_mode selected_mode=camera_mode::rear;
static bool ensure_mapping(){if(view)return true; mapping=OpenFileMappingA(FILE_MAP_READ,FALSE,SHM_NAME); if(!mapping)return false; view=MapViewOfFile(mapping,FILE_MAP_READ,0,0,sizeof(Command)); if(!view){CloseHandle(mapping);mapping=nullptr;return false;}return true;}
static bool read_command(Command& c){if(!ensure_mapping())return false; Command s{};std::memcpy(&s,view,sizeof s);if(s.magic!=MAGIC||s.version!=1||s.sequence==last_sequence)return false;c=s;last_sequence=s.sequence;return true;}
static void restore(prism::core_camera_u* camera){if(!camera||!saved)return;camera->placement=saved_placement;camera->camera_fov=saved_fov;saved=false;}
void set_player_vehicle(std::uintptr_t p){selected_vehicle=p;} void clear_player_vehicle(){selected_vehicle=0;} void set_camera_mode(camera_mode m){selected_mode=m;} camera_mode get_camera_mode(){return selected_mode;}
static bool apply_player_camera(prism::core_camera_u* camera){if(!camera||!selected_vehicle)return false;auto* v=reinterpret_cast<prism::traffic_player_vehicle_t*>(selected_vehicle);if(!v)return false;if(!saved){saved_placement=camera->placement;saved_fov=camera->camera_fov;saved=true;}const auto base=v->placement;ets2la_plugin::float3_t offset{};switch(selected_mode){case camera_mode::cabin:offset={0,2.2f,0.4f};break;case camera_mode::rear:offset={-2.5f,3,-9};break;case camera_mode::free:offset={7,5,-10};break;case camera_mode::overhead:offset={0,16,-2};break;}camera->placement.pos=base.to_global_position()+offset.rotate(base.rot);camera->placement.rot=base.rot;camera->camera_fov=selected_mode==camera_mode::overhead?75.f:70.f;return true;}
void tick(){auto* manager=prism::camera_manager_u::get();if(!manager||manager->current_camera>=manager->cameras.size)return;auto* camera=manager->cameras[manager->current_camera];if(!camera)return;if(selected_vehicle){apply_player_camera(camera);return;}Command c{};if(!read_command(c))return;if(!c.active){restore(camera);return;}if(!saved){saved_placement=camera->placement;saved_fov=camera->camera_fov;saved=true;}camera->placement.pos.x=c.position.x;camera->placement.pos.y=c.position.y;camera->placement.pos.z=c.position.z;camera->placement.rot.x=c.rotation.x;camera->placement.rot.y=c.rotation.y;camera->placement.rot.z=c.rotation.z;camera->placement.rot.w=c.rotation.w;camera->camera_fov=c.fov;}
void shutdown(){selected_vehicle=0;if(view){UnmapViewOfFile(view);view=nullptr;}if(mapping){CloseHandle(mapping);mapping=nullptr;}saved=false;last_sequence=0;selected_mode=camera_mode::rear;}
}
#else
namespace ets2la_plugin::examiner_camera_bridge { void tick(){} void shutdown(){} void set_player_vehicle(std::uintptr_t){} void clear_player_vehicle(){} void set_camera_mode(camera_mode){} camera_mode get_camera_mode(){return camera_mode::rear;} }
#endif
