#include "game_actor.hpp"
#include "memory/memory_utils.hpp"
#include "patterns.hpp"
#include "prism/controllers/base_ctrl.hpp"
#include "prism/input/input_mix.hpp"
#include <stdexcept>

namespace ets2la_plugin::prism
{
    uint32_t game_actor_u::steering_input_offset = 0;
    uint32_t game_actor_u::throttle_input_offset = 0;
    uint32_t game_actor_u::brake_input_offset    = 0;

    bool game_actor_u::scan_patterns()
    {
        const auto throttle_addr = memory::get_address_for_pattern( patterns::game_actor_throttle_input_offset, 13 );

        if ( throttle_addr == 0 )
        {
            throw std::runtime_error( "Failed to find throttle input offset" );
        }
        game_actor_u::throttle_input_offset = *reinterpret_cast< uint32_t* >( throttle_addr );

        const auto brake_addr = memory::get_address_for_pattern( patterns::game_actor_brake_input_offset, 13 );

        if ( brake_addr == 0 )
        {
            throw std::runtime_error( "Failed to find brake input offset" );
        }
        game_actor_u::brake_input_offset = *reinterpret_cast< uint32_t* >( brake_addr );

        return true;
    }

    game_actor_u* game_actor_u::get()
    {
        auto* base_ctrl = prism::base_ctrl_u::get();

        if ( base_ctrl == nullptr || base_ctrl_u::game_actor_offset == 0 )
        {
            return nullptr;
        }

        return *reinterpret_cast< game_actor_u** >(
            reinterpret_cast< char* >( base_ctrl ) + base_ctrl_u::game_actor_offset
        );
    }

    void game_actor_u::set_throttle_input( float value )
    {
        auto input_ref = reinterpret_cast< mix_fixed_ref_t* >(
            reinterpret_cast< uint64_t >( this ) + game_actor_u::throttle_input_offset
        );
        if ( input_ref->input == nullptr )
        {
            return;
        }
        input_ref->input->value = value;
    }

    void game_actor_u::set_brake_input( float value )
    {
        auto input_ref = reinterpret_cast< mix_fixed_ref_t* >(
            reinterpret_cast< uint64_t >( this ) + game_actor_u::brake_input_offset
        );
        if ( input_ref->input == nullptr )
        {
            return;
        }
        input_ref->input->value = value;
    }
}
