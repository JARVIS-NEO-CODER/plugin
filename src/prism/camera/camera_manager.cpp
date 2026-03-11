#include "camera_manager.hpp"

namespace ets2la_plugin::prism
{

    uint64_t camera_manager_u::instance_ptr_address = 0;

    bool camera_manager_u::scan_patterns()
    {
        const auto addr = memory::get_address_for_pattern( patterns::camera_manager, 3 );

        if ( addr == 0 )
        {
            return false;
        }
        camera_manager_u::instance_ptr_address = addr + *reinterpret_cast< int32_t* >( addr ) + 4;

        return true;
    }

    camera_manager_u* camera_manager_u::get()
    {
        if ( camera_manager_u::instance_ptr_address != 0 )
        {
            return *reinterpret_cast< camera_manager_u** >( camera_manager_u::instance_ptr_address );
        }
        return nullptr;
    }

}
