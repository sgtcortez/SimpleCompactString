#include "scs/scs.h"

#include <bits/stdint-uintn.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "scs/arrayhelper.h"

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

scs_t scs_from_internal ( const char *input, const uint64_t size );

/**
 * Restore the public scs struct to the internal scs struct.
 * This works because we have a well defined Memory Layout for our structures
 */
scs_internal restore ( const scs_t );

scs_t scs_from ( const char *input, uint64_t size )
{
    return scs_from_internal ( input, size );
}

scs_t scs_from_internal ( const char *input, const uint64_t size )
{
    internal_options opt;
    opt.size_type = count_bytes ( size );
    opt.updatable = 1;
    opt.encoding = ASCII;


    /**
     * We always store the last byte as 0. 
     * So we can always have a cstring compatible array
     * This one byte that we added, will not be present in scs_size
    */
    const uint64_t bytes_needed = sizeof ( opt ) + count_bytes ( size ) + size + 1;

    char *buffer = calloc ( bytes_needed, sizeof ( char ) );

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

    /**
     * Returns the C style string for the user
     */
    return buffer + sizeof ( opt ) + opt.size_type;
}

void scs_free ( scs_t scs )
{
    scs_internal internal = restore ( scs );
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
