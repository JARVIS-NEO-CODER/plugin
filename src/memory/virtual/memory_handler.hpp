#pragma once

#include "fmt/core.h"
#include "scssdk_telemetry.h"
#include <array>

namespace ets2la_plugin
{
    struct PluginStateData;
    struct InputMemData;
    struct CameraMemData;
    struct TrafficMemData;
    struct ParkedVehiclesMemData;
    struct SemaphoreMemData;
    struct RouteMemData;

    class CMemoryHandler
    {
    private:

        void* state_mmap           = nullptr;
        void* input_mmap           = nullptr;
        void* camera_mmap          = nullptr;
        void* traffic_mmap         = nullptr;
        void* parked_vehicles_mmap = nullptr;
        void* semaphore_mmap       = nullptr;
        void* route_mmap           = nullptr;

        const wchar_t* state_mem_name           = L"ETS2LAPluginStatus";
        const wchar_t* input_mem_name           = L"ETS2LAPluginInput";
        const wchar_t* camera_mem_name          = L"ETS2LACameraProps";
        const wchar_t* traffic_mem_name         = L"ETS2LATraffic";
        const wchar_t* parked_vehicles_mem_name = L"ETS2LAParkedVehicles";
        const wchar_t* semaphore_mem_name       = L"ETS2LASemaphore";
        const wchar_t* route_mem_name           = L"ETS2LARoute";

        bool is_running_under_wine = false;

        scs_log_t scs_log_;

        template <class... T>
        void info(const char *fmt_s, T &&...args) const
        {
            scs_log_(0, fmt::vformat(std::string("[ets2la_plugin][CMemoryHandler] ") + fmt_s, fmt::make_format_args(args...)).c_str());
        }

        template <class... T>
        void error(const char *fmt_s, T &&...args) const
        {
            scs_log_(2, fmt::vformat(std::string("[ets2la_plugin] ") + fmt_s, fmt::make_format_args(args...)).c_str());
        }

        void unmap_file(void* mmap, const wchar_t* file_name, const size_t size);

    public:
        CMemoryHandler(scs_log_t scs_log);
        ~CMemoryHandler();

        bool init();
        void destroy();

        // initializers
        void initialize_memory_file(const wchar_t* file_name, const wchar_t* format, void*& output_file) const;
        void initialize_memory_file_multiple(const wchar_t* file_name, const wchar_t* format, int count, void*& output_file) const;

        // read/write
        InputMemData read_input_mem() const;
        void write_state_mem(const PluginStateData data) const;
        void write_camera_mem(const CameraMemData data) const;
        void write_traffic_mem(const TrafficMemData data) const;
        void write_parked_vehicles_mem(const ParkedVehiclesMemData data) const;
        void write_semaphore_mem(const SemaphoreMemData data) const;
        void write_route_mem(const RouteMemData data) const;
    };
}
