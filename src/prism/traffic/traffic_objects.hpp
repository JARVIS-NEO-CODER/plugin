#pragma once

#include "prism/common.hpp"
#include <cstdint>

#pragma pack( push, 1 )

namespace ets2la_plugin::prism
{

    class traffic_object_t // Size: 0x0010
    {
    public:
        char pad_0008[ 8 ]; // 0x0008 (0x08)

        virtual void destructor();

#if defined( __linux__ )
        virtual void destructor2();
#endif

        virtual uint64_t get_type() const; // 0x7 = traffic_semaphore_actor_t
    };
    static_assert( sizeof( traffic_object_t ) == 0x10 );

    class traffic_actor_t : public traffic_object_t // Size: 0x0080
    {
    public:
        char pad_0010[ 24 ];   // 0x0010 (0x18)
        placement_t placement; // 0x0028 (0x20)
        char pad_0048[ 56 ];   // 0x0048 (0x38)
    };
    static_assert( sizeof( traffic_actor_t ) == 0x80 );

#if defined( _WIN32 )

    class traffic_semaphore_actor_t : public traffic_actor_t // Size: 0x01D8 1.54
    {
    public:
        char pad_0080[ 64 ];                // 0x0080 (0x40)
        class tr_semaphore_model_u* model;  // 0x00C0 (0x08)
        char pad_00C8[ 24 ];                // 0x00C8 (0x18)
        class model_object_u* model_object; // 0x00E0 (0x08)
        char pad_00E8[ 16 ];                // 0x00E8 (0x10)
        class model_coll_u* model_coll;     // 0x00F8 (0x08)
        class traffic_rule_t* traffic_rule; // 0x0100 (0x08)
        uint8_t state;                      // 0x0108 (0x01) 2 = opening; 3 = open; 0 = closing; 1 = closed
        char pad_0109[ 15 ];                // 0x0109 (0x0f)
        float time_to_open;                 // 0x0118 (0x04)
        float open_time_remaining;          // 0x011C (0x04) used for double animation
        char pad_0120[ 8 ];                 // 0x0120 (0x08)
        float time_to_close;                // 0x0128 (0x04)
        float close_time_remaining;         // 0x012C (0x04) used for double animation
        char pad_0130[ 6 ];                 // 0x0130 (0x06)
        uint8_t animation_mode;             // 0x0136 (0x01) 1 = single animation; 2 = double animation
        uint8_t coll_mode;                  // 0x0137 (0x01)
        char pad_0138[ 4 ];                 // 0x0138 (0x04)
        float animation_time_elapsed;       // 0x013C (0x04) used for single animation
        uint8_t N00007222;                  // 0x0140 (0x01)
        uint8_t N0000723A;                  // 0x0141 (0x01)
        char pad_0142[ 150 ];               // 0x0142 (0x96)
    };
    static_assert( sizeof( traffic_semaphore_actor_t ) == 0x1D8 );

#else

    class traffic_semaphore_actor_t : public traffic_actor_t // Size: 0x01E0 1.54
    {
    public:
        char pad_0088[ 72 ];                // 0x0088 (0x48)
        class tr_semaphore_model_u* model;  // 0x00C8 (0x08)
        char pad_00D0[ 24 ];                // 0x00D0 (0x18)
        class model_object_u* model_object; // 0x00E8 (0x08)
        char pad_00F0[ 16 ];                // 0x00F0 (0x10)
        class model_coll_u* model_coll;     // 0x0100 (0x08)
        class traffic_rule_t* traffic_rule; // 0x0108 (0x08)
        uint8_t state;                      // 0x0110 (0x01) 2 = opening; 3 = open; 0 = closing; 1 = closed
        char pad_0111[ 15 ];                // 0x0111 (0x0f)
        float time_to_open;                 // 0x0120 (0x04)
        float open_time_remaining;          // 0x0124 (0x04) used for double animation
        char pad_0128[ 8 ];                 // 0x0128 (0x08)
        float time_to_close;                // 0x0130 (0x04)
        float close_time_remaining;         // 0x0134 (0x04) used for double animation
        char pad_0138[ 6 ];                 // 0x0138 (0x06)
        uint8_t animation_mode;             // 0x013E (0x01) 1 = single animation; 2 = double animation
        uint8_t coll_mode;                  // 0x013F (0x01)
        char pad_0140[ 4 ];                 // 0x0140 (0x04)
        float animation_time_elapsed;       // 0x0144 (0x04) used for single animation
        uint8_t N00007222;                  // 0x0148 (0x01)
        uint8_t N0000723A;                  // 0x0149 (0x01)
        char pad_014A[ 150 ];               // 0x014A (0x96)
    };
    static_assert( sizeof( traffic_semaphore_actor_t ) == 0x1E0 );

#endif

}

#pragma pack( pop )
