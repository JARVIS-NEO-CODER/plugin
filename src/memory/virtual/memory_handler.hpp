#pragma once

#include "scssdk_telemetry.h"
#include <windows.h>
#include <string>
#include <array>

namespace ets2la_plugin
{
    struct PluginStateData;
    struct InputMemData;
    struct CameraMemData;
    struct TrafficMemData;
    struct SemaphoreMemData;
    struct RouteMemData;

    class CMemoryHandler
    {
    private:
        HANDLE state_file;
        HANDLE input_file;
        HANDLE camera_file;
        HANDLE traffic_file;
        HANDLE semaphore_file;
        HANDLE route_file;
        
        const wchar_t* state_mem_name      = L"Local\\ETS2LAPluginStatus";
        const wchar_t* input_mem_name      = L"Local\\ETS2LAPluginInput";
        const wchar_t* camera_mem_name     = L"Local\\ETS2LACameraProps";
        const wchar_t* traffic_mem_name    = L"Local\\ETS2LATraffic";
        const wchar_t* semaphore_mem_name  = L"Local\\ETS2LASemaphore";
        const wchar_t* route_mem_name      = L"Local\\ETS2LARoute";
        
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

    public:
        CMemoryHandler(scs_log_t scs_log);
        ~CMemoryHandler();

        bool init();
        void destroy();

        // initializers
        void initialize_memory_file(wchar_t* file_name, wchar_t* format, HANDLE& output_file) const;
        void initialize_memory_file_multiple(wchar_t* file_name, wchar_t* format, int count, HANDLE& output_file) const;
        
        // read/write
        InputMemData read_input_mem() const;
        void write_state_mem(const PluginStateData data) const;
        void write_camera_mem(const CameraMemData data) const;
        void write_traffic_mem(const TrafficMemData data) const;
        void write_semaphore_mem(const SemaphoreMemData data) const;
        void write_route_mem(const RouteMemData data) const;
    };
}