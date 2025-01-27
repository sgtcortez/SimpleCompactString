#include "scs/scs.h"

#include <bits/stdint-uintn.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>
#include <unistd.h>

#include "scs/arrayhelper.h"

#define ALIGN( number, alignment ) ( ( number + ( alignment - 1 ) ) & ~( alignment - 1 ) )

enum encoding
{
    ASCII = 0,
    UTF_8 = 0b1,
    UTF_16 = 0b10,
    UTF_32 = 0b11,
};

typedef union internal_options
{
    struct
    {
        /**
         * This flag stores the length of bytes used to store the user string
         * For now, the maximum size its 8 bytes of user message(thats a lot of
         * text) 2^3 = 8
         */
        uint8_t size_type : 3;

        /**
         * A boolean to specify if this string its updatable.
         * Would be nice to protect this memory with system call.
         * Maybe, we can take a look into this system call:
         * https://linux.die.net/man/2/mprotect
         */
        bool read_only : 1;

        /**
         * What type of encoding is it using?
         * 0 -> ASCII
         * 1 -> UTF-8
         * 10 -> UTF-16
         * 11 -> UTF-32
         */
        uint8_t encoding : 2;

        uint8_t unused : 1;
    };
    uint8_t byte;
} internal_options;

typedef struct scs_internal
{
    union
    {
        /**
         * Pointer to a read only buffer.
         * It helps to avoid some mistakes
         * Use this buffer when we intend to only read from memory
         */
        const char *r_buffer;

        /**
         * A writable buffer, use this one when intend to write to memory
         */
        char *rw_buffer;
    };
    internal_options options;
} scs_internal;

scs_t scs_from_internal ( const char *input, const uint64_t size, const bool read_only );

/**
 * Restore the public scs struct to the internal scs struct.
 * This works because we have a well defined Memory Layout for our structures
 */
scs_internal restore ( const scs_t );

scs_t scs_from ( const char *input, uint64_t size )
{
    return scs_from_internal ( input, size, false );
}

scs_t scs_from_readonly ( const char *input, uint64_t size )
{
    return scs_from_internal ( input, size, true );
}

scs_t scs_from_internal ( const char *input, const uint64_t size, const bool read_only )
{
    internal_options opt;
    opt.size_type = count_bytes ( size );
    opt.encoding = ASCII;

    /**
     * We always store the last byte as 0.
     * So we can always have a cstring compatible array
     * This one byte that we added, will not be present in scs_size
     */
    const uint64_t bytes_needed = sizeof ( opt ) + count_bytes ( size ) + size + 1;

    char *buffer = NULL;

    if ( !read_only )
    {
        /**
         * The user wants a SCS object that can be updated
         */
        opt.read_only = false;
        buffer = calloc ( bytes_needed, sizeof ( char ) );
    }
    else
    {
        /**
         * We need to align with a operating system memory size.
         * Usually 4kiB
         */
        opt.read_only = true;
        const int page_size = sysconf ( _SC_PAGESIZE );
        const uint64_t aligned_size = ALIGN ( bytes_needed, page_size );
        buffer = aligned_alloc ( page_size, aligned_size );
    }

    /**
     * Store the user input message size in a variable length size
     */
    array_store ( buffer, size, opt.size_type );

    /**
     * Store the Options
     */
    memcpy ( buffer + opt.size_type, &opt, sizeof ( opt ) );

    /**
     * Store the user message
     */
    memcpy ( buffer + sizeof ( opt ) + opt.size_type, input, size );

    if ( read_only )
    {
        const int page_size = sysconf ( _SC_PAGESIZE );
        const uint64_t aligned_size = ALIGN ( bytes_needed, page_size );
        if ( mprotect ( buffer, aligned_size, PROT_READ ) < 0 )
        {
            // error
            exit ( 1 );
        }
    }

    /**
     * Returns the C style string for the user
     */
    return buffer + sizeof ( opt ) + opt.size_type;
}

void scs_free ( scs_t scs )
{
    scs_internal internal = restore ( scs );
    if ( internal.options.read_only )
    {
        // We need to remove the protection of this memory region
        const uint64_t bytes_needed =
            sizeof ( internal_options ) + internal.options.size_type + scs_size ( scs );
        const int page_size = sysconf ( _SC_PAGESIZE );
        const uint64_t aligned_size = ALIGN ( bytes_needed, page_size );
        if ( mprotect ( internal.rw_buffer, aligned_size, PROT_READ | PROT_WRITE ) < 0 )
        {
            // error
            exit ( 1 );
        }
    }

    free ( internal.rw_buffer );
}

uint64_t scs_size ( const scs_t scs )
{
    const scs_internal internal = restore ( scs );
    const uint64_t bytes = sizeof ( internal_options ) + internal.options.size_type;
    char *start_index = scs - bytes;
    return array_restore ( start_index, internal.options.size_type );
}

scs_internal restore ( const scs_t scs )
{
    scs_internal internal = { 0 };
    internal.options.byte = scs[-1];
    const uint64_t bytes = sizeof ( internal_options ) + internal.options.size_type;
    internal.rw_buffer = scs - bytes;
    return internal;
}
