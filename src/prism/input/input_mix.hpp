#pragma once

#include "prism/collections/array_dyn.hpp"
#include "prism/string.hpp"
#include "prism/token.hpp"

namespace ets2la_plugin::prism
{
#pragma pack( push, 1 )

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

    // Size: W 0x0180, L|A 0x0148 1.60
    class input_mix_t : public /* [W|L|A 0x28] @ W|L|A 0x00 */ input_object_t
    {
    public:
        string_dyn_t mix_formula;                         // W|L|A 0x0028 (W 0x20, L|A 0x18)
        char pad_W_0x0048__LA_0x0040[ 8 ];                // W 0x0048, L|A 0x0040 (W|L|A 0x08)
        array_dyn_t< void* > N0000F32B;                   // W 0x0050, L|A 0x0048 (W 0x28, L|A 0x20)
        char pad_W_0x0078__LA_0x0068[ 32 ];               // W 0x0078, L|A 0x0068 (W|L|A 0x20)
        class expression_element_t* expression_element;   // W 0x0098, L|A 0x0088 (W|L|A 0x08)
        array_dyn_t< uint64_t > N0000F335;                // W 0x00A0, L|A 0x0090 (W 0x28, L|A 0x20)
        array_dyn_t< class input_mix_t* > N0000F33A;      // W 0x00C8, L|A 0x00B0 (W 0x28, L|A 0x20)
        array_dyn_t< class input_constant_t* > N0000F33F; // W 0x00F0, L|A 0x00D0 (W 0x28, L|A 0x20)
        array_dyn_t< class input_impulse_t* > N0000F344;  // W 0x0118, L|A 0x00F0 (W 0x28, L|A 0x20)
        array_dyn_t< class input_mix_t* > N0000F349;      // W 0x0140, L|A 0x0110 (W 0x28, L|A 0x20)
        char pad_W_0x0168__LA_0x0130[ 16 ];               // W 0x0168, L|A 0x0130 (W|L|A 0x10)
        float value;                                      // W 0x0178, L|A 0x0140 (W|L|A 0x04)
        char pad_W_0x017C__LA_0x0144[ 4 ];                // W 0x017C, L|A 0x0144 (W|L|A 0x04)
    };

#if defined( _WIN32 )
    static_assert( sizeof( input_mix_t ) == 0x180 ); // W
#elif defined( __linux__ ) || defined( __APPLE__ )
    static_assert( sizeof( input_mix_t ) == 0x148 ); // L|A
#endif

    // Size: 0x0010 actually a template
    class mix_fixed_ref_t
    {
    public:
        input_mix_t* input; // 0x0008 (0x08)

        virtual void destructor();
    };
    static_assert( sizeof( mix_fixed_ref_t ) == 0x10 );

#pragma pack( pop )
}
