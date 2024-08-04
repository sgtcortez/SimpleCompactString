#include "scs/scs.h"

#include <bits/stdint-uintn.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>
#include <unistd.h>

#include "scs/arrayhelper.h"

#define ALIGN(number, alignment)((number + (alignment - 1)) & ~(alignment - 1))

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
        uint8_t updatable : 1;

        /**
         * What type of encoding is it using?
         * 0 -> ASCII
         * 1 -> UTF-8
         * 10 -> UTF-16
         * 11 -> UTF-32
         */
        uint8_t encoding : 2;

        /**
         * A flag to determine if the string is binary or not
         * If binary = 0, this means that the last byte must be the null byte, hence, we need to
         * ignore it in the scs_size function
         */
        uint8_t binary : 1;

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

scs_t scs_from_internal ( const char *input, const uint64_t size, const bool binary, const bool updatable );

/**
 * Restore the public scs struct to the internal scs struct.
 * This works because we have a well defined Memory Layout for our structures
 */
scs_internal restore ( const scs_t );


scs_t scs_from_string ( const char *input, const bool updatable )
{
    const char *handled_input = input == NULL ? "" : input;
    // +1 byte to store the null byte
    const size_t input_size = strlen ( handled_input ) + 1;
    return scs_from_internal ( handled_input, input_size, false, updatable );
}

scs_t scs_from ( const char *input, const uint64_t size, const bool updatable )
{
    return scs_from_internal ( input, size, true, updatable );
}

scs_t scs_from_internal ( const char *input, const uint64_t size, const bool binary, const bool updatable )
{
    internal_options opt;
    opt.updatable = updatable;
    opt.size_type = count_bytes ( size );
    opt.encoding = ASCII;
    opt.binary = binary;

    const uint64_t bytes_needed = sizeof ( opt ) + count_bytes ( size ) + size;
    char *buffer;

    if (updatable)
    {
        buffer = calloc ( bytes_needed, sizeof ( char ) );
    }
    else 
    {
        const int page_size = sysconf(_SC_PAGESIZE);
        const uint64_t aligned_size = ALIGN(bytes_needed, page_size);
        buffer = aligned_alloc(page_size, aligned_size);
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

    if (!updatable)
    {
        const int page_size = sysconf(_SC_PAGESIZE);
        const uint64_t aligned_size = ALIGN(bytes_needed, page_size);        
        if (mprotect(buffer, aligned_size, PROT_READ) < 0) 
        {
            // an error happened
            // Handle this latter
            exit(1);
        }
    }
    /**
     * Returns the C style string for the user
     */
    return buffer + sizeof ( opt ) + opt.size_type;
}

void scs_free ( scs_t scs )
{
    if (!scs)
    {
        return;
    }
    scs_internal internal = restore ( scs );
    if (!scs) 
    {
        return;
    }
    if (!(internal.options.updatable)) 
    {
        // We need to remove the protection of this memory region
        const uint64_t bytes_needed = sizeof ( internal_options ) + internal.options.size_type + scs_size(scs);
        const int page_size = sysconf(_SC_PAGESIZE);
        const uint64_t aligned_size = ALIGN(bytes_needed, page_size);        
        if (mprotect(scs, aligned_size, PROT_READ | PROT_WRITE) < 0)
        {
            // TODO Handle
            exit(1);
        }
    }
    free ( internal.rw_buffer );
}

uint64_t scs_size ( const scs_t scs )
{
    const scs_internal internal = restore ( scs );
    const uint64_t bytes = sizeof ( internal_options ) + internal.options.size_type;
    char *start_index = scs - bytes;

    const uint64_t size = array_restore ( start_index, internal.options.size_type );
    if ( internal.options.binary )
    {
        return size;
    }
    // we must not count the null byte as part of the string length
    return size - 1;
}

scs_internal restore ( const scs_t scs )
{
    scs_internal internal = { 0 };
    internal.options.byte = scs[-1];
    const uint64_t bytes = sizeof ( internal_options ) + internal.options.size_type;
    internal.rw_buffer = scs - bytes;
    return internal;
}

