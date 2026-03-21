#include "memory_utils.hpp"

#include "link.h"

namespace ets2la_plugin::memory
{
    // If don't really know anything about low-level linux, so I have no idea how wrong/correct this is.
    // But it seems to work
    int iterate_phdr( dl_phdr_info* info, size_t size, void* data )
    {
        for ( size_t i = 0; i < info->dlpi_phnum; i++ )
        {
            // get first executable segment
            if ( ( info->dlpi_phdr[ i ].p_flags & 0x01 ) == 1 )
            {
                g_module_size = info->dlpi_phdr[ i ].p_memsz;
                g_game_base   = info->dlpi_phdr[ i ].p_vaddr;

                return 1;
            }
        }

        return 0;
    }

    void ensure_module_info()
    {
        if ( g_game_base != 0 && g_module_size != 0 )
        {
            return;
        }
        dl_iterate_phdr( iterate_phdr, nullptr );
    }
}
