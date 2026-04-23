#pragma once

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{

    // Size: 0x01D0
    class traffic_vehicle_mp_t
    {
    public:
        char pad_0008[ 456 ]; // 0x0008 (0x1c8)

        virtual void destructor();
    };
    static_assert( sizeof( traffic_vehicle_mp_t ) == 0x1D0 );

}

#pragma pack( pop )
