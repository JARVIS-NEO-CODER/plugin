#include "memory/virtual/memory_handler.hpp"
#include "core.hpp"
#include <sstream>

namespace ets2la_plugin
{
    CMemoryHandler::CMemoryHandler(scs_log_t scs_log) : scs_log_(scs_log),
        input_file(NULL),
        camera_file(NULL),
        traffic_file(NULL),
        semaphore_file(NULL),
        route_file(NULL)
    {
    }

    CMemoryHandler::~CMemoryHandler()
    {
        destroy();
    }

    bool CMemoryHandler::init()
    {
        this->initialize_memory_file(const_cast<wchar_t*>(state_mem_name), L"ibb", state_file);
        this->initialize_memory_file(const_cast<wchar_t*>(input_mem_name), L"fbdfbd", input_file);
        this->initialize_memory_file(const_cast<wchar_t*>(camera_mem_name), L"ffffssffffffffffffffffffff", camera_file);
        this->initialize_memory_file_multiple(const_cast<wchar_t*>(traffic_mem_name),
            L"ffffffffffffssbbffffffffffffffffffffffffffffff", 
            40, traffic_file
        );
        this->initialize_memory_file_multiple(const_cast<wchar_t*>(semaphore_mem_name), L"fffssffffifii", 40, semaphore_file);
        this->initialize_memory_file_multiple(const_cast<wchar_t*>(route_mem_name), L"lff", 6000, route_file);
        
        return true;
    }

    void CMemoryHandler::destroy()
    {
        if (input_file != NULL) {
            CloseHandle(input_file);
            input_file = NULL;
        }
        
        if (camera_file != NULL) {
            CloseHandle(camera_file);
            camera_file = NULL;
        }
        
        if (traffic_file != NULL) {
            CloseHandle(traffic_file);
            traffic_file = NULL;
        }
        
        if (semaphore_file != NULL) {
            CloseHandle(semaphore_file);
            semaphore_file = NULL;
        }
        
        if (route_file != NULL) {
            CloseHandle(route_file);
            route_file = NULL;
        }
    }

    // This is used to boot up the shared memory file. It creates a file mapping
    // based on the format below.
    // f - float      (4 bytes)
    // b - bool       (1 byte)
    // i - int        (4 bytes)
    // s - short      (2 bytes)
    // l - long long  (8 bytes)
    // d - double     (8 bytes)
    void CMemoryHandler::initialize_memory_file(wchar_t* file_name, wchar_t* format, HANDLE& output_file) const {
        std::wstring wformat(format);
        std::string sformat(wformat.begin(), wformat.end());
        std::wstring wfile_name(file_name);
        std::string sfile_name(wfile_name.begin(), wfile_name.end());
        this->info("Initializing shared mem file with format {}", sformat);

        const char float_type = 'f';
        const char boolean_type = 'b';
        const char integer_type = 'i';
        const char short_type = 's';
        const char long_long_type = 'l';
        const char double_type = 'd';

        size_t size = 0;
        for (int i = 0; format[i] != '\0'; i++)
        {
            if (format[i] == float_type)
            {
                size += sizeof(float);
            }
            else if (format[i] == boolean_type)
            {
                size += sizeof(bool);
            }
            else if (format[i] == integer_type)
            {
                size += sizeof(int);
            }
            else if (format[i] == short_type)
            {
                size += sizeof(short);
            }
            else if (format[i] == long_long_type)
            {
                size += sizeof(long long);
            }
            else if (format[i] == double_type)
            {
                size += sizeof(double);
            }
        }

        output_file = CreateFileMapping(
            INVALID_HANDLE_VALUE,    // paging file
            NULL,                    // default security
            PAGE_READWRITE,          // read/write access
            0,                       // maximum object size (high-order DWORD)
            size,                    // maximum object size (low-order DWORD)
            file_name);              // name of mapping object

        if (output_file == NULL) {
            DWORD dw = GetLastError();
            std::stringstream ss;
            ss << dw;
            std::string message = "Failed to create file mapping. Error code: " + ss.str();
            this->error(message.c_str());
            return;
        }

        void* pBuf = MapViewOfFile(output_file, FILE_MAP_ALL_ACCESS, 0, 0, size);

        if (pBuf == NULL) {
            DWORD dw = GetLastError();
            std::stringstream ss;
            ss << dw;
            this->error("Failed to map view of file {}. Error code: {}", sfile_name, ss.str());
            CloseHandle(output_file);
            output_file = NULL;
            return;
        }

        float f = 0.0f;
        bool b = false;
        int n = 0;
        short s = 0;
        long long l = 0;
        double d = 0.0;

        int offset = 0;
        for (int i = 0; format[i] != '\0'; i++)
        {
            switch (format[i])
            {
                case float_type:
                    memcpy(static_cast<char*>(pBuf) + offset, &f, sizeof(float));
                    offset += sizeof(float);
                    break;
                case boolean_type:
                    memcpy(static_cast<char*>(pBuf) + offset, &b, sizeof(bool));
                    offset += sizeof(bool);
                    break;
                case integer_type:
                    memcpy(static_cast<char*>(pBuf) + offset, &n, sizeof(int));
                    offset += sizeof(int);
                    break;
                case short_type:
                    memcpy(static_cast<char*>(pBuf) + offset, &s, sizeof(short));
                    offset += sizeof(short);
                    break;
                case long_long_type:
                    memcpy(static_cast<char*>(pBuf) + offset, &l, sizeof(long long));
                    offset += sizeof(long long);
                    break;
                case double_type:                    
                    memcpy(static_cast<char*>(pBuf) + offset, &d, sizeof(double));
                    offset += sizeof(double);
                    break;
                default:
                    break;
            }
        }

        UnmapViewOfFile(pBuf);
        this->info("Successfully opened shared mem file {} with size {}", sfile_name, size);
    }

    InputMemData CMemoryHandler::read_input_mem() const {
        if (input_file == NULL) {
            this->error("Shared mem file not open.");
            return InputMemData();
        }

        void* pBuf = MapViewOfFile(input_file, FILE_MAP_ALL_ACCESS, 0, 0, 18);

        float steering = 0.0f;
        bool override_steering = false;
        double steering_timestamp = 0;
        float acceleration = 0.0f;
        bool override_acceleration = false;
        double acceleration_timestamp = 0;

        steering = *reinterpret_cast<float*>(static_cast<char*>(pBuf));
        override_steering = *reinterpret_cast<bool*>(static_cast<char*>(pBuf) + 4);
        steering_timestamp = *reinterpret_cast<double*>(static_cast<char*>(pBuf) + 5);
        acceleration = *reinterpret_cast<float*>(static_cast<char*>(pBuf) + 13);
        override_acceleration = *reinterpret_cast<bool*>(static_cast<char*>(pBuf) + 17);
        acceleration_timestamp = *reinterpret_cast<double*>(static_cast<char*>(pBuf) + 18);

        UnmapViewOfFile(pBuf);

        return InputMemData{ steering, override_steering, steering_timestamp, acceleration, override_acceleration, acceleration_timestamp };
    }

    void CMemoryHandler::write_state_mem(const PluginStateData data) const {
        if (state_file == NULL) {
            this->error("Shared mem file not open.");
            return;
        }

        void* pBuf = MapViewOfFile(state_file, FILE_MAP_ALL_ACCESS, 0, 0, 6);

        memcpy(static_cast<char*>(pBuf), &data.version, sizeof(int));
        memcpy(static_cast<char*>(pBuf) + 4, &data.steering_overridden, sizeof(bool));
        memcpy(static_cast<char*>(pBuf) + 5, &data.acceleration_overridden, sizeof(bool));

        UnmapViewOfFile(pBuf);
    }

    void CMemoryHandler::write_camera_mem(const CameraMemData data) const {
        if (camera_file == NULL) {
            this->error("Shared mem file not open.");
            return;
        }

        void* pBuf = MapViewOfFile(camera_file, FILE_MAP_ALL_ACCESS, 0, 0, 36);

        memcpy(static_cast<char*>(pBuf), &data.fov, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 4, &data.pos_x, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 8, &data.pos_y, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 12, &data.pos_z, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 16, &data.cx, sizeof(int16_t));
        memcpy(static_cast<char*>(pBuf) + 18, &data.cz, sizeof(int16_t));
        memcpy(static_cast<char*>(pBuf) + 20, &data.qw, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 24, &data.qx, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 28, &data.qy, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 32, &data.qz, sizeof(float));
        
        memcpy(static_cast<char*>(pBuf) + 36, &data.m11, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 40, &data.m12, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 44, &data.m13, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 48, &data.m14, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 52, &data.m21, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 56, &data.m22, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 60, &data.m23, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 64, &data.m24, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 68, &data.m31, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 72, &data.m32, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 76, &data.m33, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 80, &data.m34, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 84, &data.m41, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 88, &data.m42, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 92, &data.m43, sizeof(float));
        memcpy(static_cast<char*>(pBuf) + 96, &data.m44, sizeof(float));

        UnmapViewOfFile(pBuf);
    }

    void CMemoryHandler::write_traffic_mem(const TrafficMemData data) const
    {
        if (traffic_file == NULL)
        {
            this->error("Traffic shared mem file not open.");
            return;
        }

        void* pBuf = MapViewOfFile(traffic_file, FILE_MAP_ALL_ACCESS, 0, 0, 6960);
        int offset = 0;

        for (int i = 0; i < 40; i++)
        {
            memcpy(static_cast<char*>(pBuf) + offset, &data.vehicles[i].vehicle.x, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 4, &data.vehicles[i].vehicle.y, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 8, &data.vehicles[i].vehicle.z, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 12, &data.vehicles[i].vehicle.qw, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 16, &data.vehicles[i].vehicle.qx, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 20, &data.vehicles[i].vehicle.qy, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 24, &data.vehicles[i].vehicle.qz, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 28, &data.vehicles[i].vehicle.width, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 32, &data.vehicles[i].vehicle.height, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 36, &data.vehicles[i].vehicle.length, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 40, &data.vehicles[i].vehicle.speed, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 44, &data.vehicles[i].vehicle.acceleration, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 48, &data.vehicles[i].vehicle.trailer_count, sizeof(short));
            memcpy(static_cast<char*>(pBuf) + offset + 50, &data.vehicles[i].vehicle.id, sizeof(short));
            memcpy(static_cast<char*>(pBuf) + offset + 52, &data.vehicles[i].vehicle.is_tmp, sizeof(bool));
            memcpy(static_cast<char*>(pBuf) + offset + 53, &data.vehicles[i].vehicle.is_trailer, sizeof(bool));
            offset += 54;

            for (int j = 0; j < 3; j++) // Trailers
            {
                memcpy(static_cast<char*>(pBuf) + offset, &data.vehicles[i].trailers[j].x, sizeof(float));
                memcpy(static_cast<char*>(pBuf) + offset + 4, &data.vehicles[i].trailers[j].y, sizeof(float));
                memcpy(static_cast<char*>(pBuf) + offset + 8, &data.vehicles[i].trailers[j].z, sizeof(float));
                memcpy(static_cast<char*>(pBuf) + offset + 12, &data.vehicles[i].trailers[j].qw, sizeof(float));
                memcpy(static_cast<char*>(pBuf) + offset + 16, &data.vehicles[i].trailers[j].qx, sizeof(float));
                memcpy(static_cast<char*>(pBuf) + offset + 20, &data.vehicles[i].trailers[j].qy, sizeof(float));
                memcpy(static_cast<char*>(pBuf) + offset + 24, &data.vehicles[i].trailers[j].qz, sizeof(float));
                memcpy(static_cast<char*>(pBuf) + offset + 28, &data.vehicles[i].trailers[j].width, sizeof(float));
                memcpy(static_cast<char*>(pBuf) + offset + 32, &data.vehicles[i].trailers[j].height, sizeof(float));
                memcpy(static_cast<char*>(pBuf) + offset + 36, &data.vehicles[i].trailers[j].length, sizeof(float));
                offset += 40;
            }
        }

        UnmapViewOfFile(pBuf);
    }

    void CMemoryHandler::write_semaphore_mem(const SemaphoreMemData data) const
    {
        if (semaphore_file == NULL)
        {
            this->error("Semaphore shared mem file not open.");
            return;
        }

        void* pBuf = MapViewOfFile(semaphore_file, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SemaphoreMemData));
        int offset = 0;

        for (int i = 0; i < 40; i++)
        {
            memcpy(static_cast<char*>(pBuf) + offset, &data.semaphores[i].x, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 4, &data.semaphores[i].y, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 8, &data.semaphores[i].z, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 12, &data.semaphores[i].cx, sizeof(short));
            memcpy(static_cast<char*>(pBuf) + offset + 14, &data.semaphores[i].cz, sizeof(short));
            memcpy(static_cast<char*>(pBuf) + offset + 16, &data.semaphores[i].qw, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 20, &data.semaphores[i].qx, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 24, &data.semaphores[i].qy, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 28, &data.semaphores[i].qz, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 32, &data.semaphores[i].type, sizeof(int));
            memcpy(static_cast<char*>(pBuf) + offset + 36, &data.semaphores[i].time_remaining, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 40, &data.semaphores[i].state, sizeof(int));
            memcpy(static_cast<char*>(pBuf) + offset + 44, &data.semaphores[i].id, sizeof(int));

            offset += 48;
        }

        UnmapViewOfFile(pBuf);
    }

    void CMemoryHandler::write_route_mem(const RouteMemData data) const
    {
        if (route_file == NULL)
        {
            this->error("Route shared mem file not open.");
            return;
        }

        void* pBuf = MapViewOfFile(route_file, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(RouteMemData));
        int offset = 0;

        for (int i = 0; i < 6000; i++)
        {
            memcpy(static_cast<char*>(pBuf) + offset, &data.tasks[i].uid, sizeof(long long));
            memcpy(static_cast<char*>(pBuf) + offset + 8, &data.tasks[i].distance, sizeof(float));
            memcpy(static_cast<char*>(pBuf) + offset + 12, &data.tasks[i].time, sizeof(float));

            offset += 16;
        }

        UnmapViewOfFile(pBuf);
    }

    void CMemoryHandler::initialize_memory_file_multiple(wchar_t* file_name, wchar_t* format, int count, HANDLE& output_file) const {
        wchar_t* total_format = (wchar_t*)malloc(count * (wcslen(format) + 1) * sizeof(wchar_t)); // count objects, +1 for null terminator
        if (!total_format) {
            this->error("Couldn't generate total_format");
            return;
        }

        std::wstring wformat(format);
        std::wstring wtotal_format;
        for (int i = 0; i < count; i++) {
            wtotal_format += wformat;
        }
        wcscpy(total_format, wtotal_format.c_str());

        this->initialize_memory_file(file_name, total_format, output_file);
        free(total_format);
    }
}
