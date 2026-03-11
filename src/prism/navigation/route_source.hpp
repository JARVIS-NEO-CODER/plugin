#pragma once

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    class simple_route_source_t // Size: 0x0170
    {
    public:
        char pad_0000[ 32 ];            //0x0000 (0x20)
        class route_task_t* route_task; //0x0020 (0x08)
        char pad_0028[ 328 ];           //0x0028 (0x148)
    };
    static_assert( sizeof( simple_route_source_t ) == 0x170 );

}

#pragma pack( pop )
