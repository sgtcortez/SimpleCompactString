#include "scs/scs.h"

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

        // Two bits left unused for now
        uint8_t unused : 2;
    };
    uint8_t byte;
} internal_options;

typedef struct scs_internal
{
    char *buffer;
    internal_options options;
} scs_internal;

/**
 * Restore the public scs struct to the internal scs struct.
 * This works because we have a well defined Memory Layout for our structures
 */
scs_internal restore ( scs_t );

scs_t scs_from_string ( char *input )
{
    input = input == NULL ? "" : input;
    const size_t input_size = strlen ( input );
    return scs_from ( input, input_size );
}

scs_t scs_from ( char *input, uint64_t size )
{
    internal_options opt;
    opt.size_type = count_bytes ( size );
    opt.updatable = 1;
    opt.encoding = ASCII;

    const uint64_t bytes_needed = sizeof ( opt ) + count_bytes ( size ) + size;

    char *buffer = calloc ( bytes_needed, sizeof ( char ) );

    /**
     * Store the Options in the first byte
     */
    memcpy ( buffer, &opt, sizeof ( opt ) );

    /**
     * Store the user input message size in a variable length size
     */
    array_store ( buffer + sizeof ( opt ), size, opt.size_type );

    /**
     * Store the user message
     */
    memcpy ( buffer + sizeof ( opt ) + opt.size_type, input, size );

    return buffer;
}

void scs_free ( scs_t scs )
{
    free ( scs );
}

char *scs_to_string (scs_t scs)
{
    const scs_internal internal = restore ( scs );
    const uint64_t size = scs_size ( scs );
    char *string = calloc ( size + 1, sizeof ( char ) );
    memcpy ( string, internal.buffer, size );
    return string;
}

uint64_t scs_size ( scs_t scs )
{
    const scs_internal internal = restore ( scs );
    const char *buffer = (char *)scs;
    return array_restore ( buffer + 1, internal.options.size_type );
}

scs_internal restore ( scs_t scs )
{
    char *buffer = (char *)scs;
    scs_internal internal = { 0 };
    internal.options.byte = buffer[0];
    internal.buffer = buffer + sizeof ( internal_options ) + internal.options.size_type;
    return internal;
}
