#pragma once

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    // Size: 0x0020
    class traffic_physics_t
    {
    public:
        char pad_0008[ 24 ]; // 0x0008 (0x18)

        virtual void destructor();
    };
    static_assert( sizeof( traffic_physics_t ) == 0x20 );

    // Size: 0x0028
    class traffic_physics_object_t
    {
    public:
        char pad_0008[ 24 ]; // 0x0008 (0x18)
#if !defined( _WIN32 )
        char pad_0020[ 8 ]; // 0x0020 (0x08)
#endif

        virtual void destructor();
    };
#if defined( _WIN32 )
    static_assert( sizeof( traffic_physics_object_t ) == 0x20 );
#else
    static_assert( sizeof( traffic_physics_object_t ) == 0x28 );
#endif

}

#pragma pack( pop )
