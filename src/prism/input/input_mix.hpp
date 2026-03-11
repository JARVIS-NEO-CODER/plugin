#pragma once

#include "prism/collections/array_dyn.hpp"
#include "prism/string.hpp"
#include "prism/token.hpp"

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{
    // Size: 0x0028
    class input_object_t
    {
    public:
        token_t name;        // 0x0008 (0x08)
        char pad_0010[ 16 ]; // 0x0010 (0x10)
        uint64_t ext_ref;    // 0x0020 (0x08)

        virtual void destructor();
    };
    static_assert( sizeof( input_object_t ) == 0x28 );

    // Size: 0x0148
    class input_mix_t : public /* [0x28] @ 0x00 */ input_object_t
    {
    public:
        string_dyn_t mix_formula;                         // 0x0028 (0x18)
        char pad_0040[ 80 ];                              // 0x0040 (0x50)
        array_dyn_t< uint32_t > N0000BF6A;                // 0x0090 (0x20)
        array_dyn_t< class input_mix_t* > N0000D4F0;      // 0x00B0 (0x20)
        array_dyn_t< class input_constant_t* > N0000D4F1; // 0x00D0 (0x20)
        array_dyn_t< class input_impulse_t* > N0000D4F5;  // 0x00F0 (0x20)
        array_dyn_t< class input_mix_t* > N0000D4F9;      // 0x0110 (0x20)
        char pad_0130[ 16 ];                              // 0x0130 (0x10)
        float value;                                      // 0x0140 (0x04)
        char pad_0144[ 4 ];                               // 0x0144 (0x04)
    };
    static_assert( sizeof( input_mix_t ) == 0x148 );

    // Size: 0x0010
    class mix_fixed_ref_t
    {
    public:
        input_mix_t* input; // 0x0008 (0x08)

        virtual void destructor();
    };
    static_assert( sizeof( mix_fixed_ref_t ) == 0x10 );
}

#pragma pack( pop )
