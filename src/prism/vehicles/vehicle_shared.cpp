#include "vehicle_shared.hpp"

#include "memory/memory_utils.hpp"
#include "patterns.hpp"

namespace ets2la_plugin::prism
{
    uint32_t vehicle_shared_u::steering_angle_offset = 0;

    bool vehicle_shared_u::scan_patterns()
    {
        const auto addr = memory::get_address_for_pattern( patterns::vehicle_shared_u_steering_offset, 6 );

        if ( addr == 0 )
        {
            return false;
        }

        vehicle_shared_u::steering_angle_offset = *reinterpret_cast< uint32_t* >( addr );

        return true;
    }

    bool vehicle_shared_u::set_steering_angle( const float new_steering_value )
    {
        if ( vehicle_shared_u::steering_angle_offset == 0 )
        {
            return false;
        }

        *reinterpret_cast< float* >( reinterpret_cast< char* >( this ) + vehicle_shared_u::steering_angle_offset ) =
            new_steering_value;

        return true;
    }

    float vehicle_shared_u::get_steering_angle()
    {
        if ( vehicle_shared_u::steering_angle_offset == 0 )
        {
            return 0.0f;
        }

        return *reinterpret_cast< float* >( reinterpret_cast< char* >( this ) + vehicle_shared_u::steering_angle_offset );
    }
}
