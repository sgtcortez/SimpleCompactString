#include "scs/scs.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct scs_iterator
{
    /**
     * The SCS origin value.
     * This value is never changed
     */
    const scs_t origin;

    /**
     * The delimiter value
     * This value is never changed
     */
    const char *delimiter;

    /**
     * The delimiter size
     */
    const uint32_t delimiter_size;

    /**
     * The next start index
     */
    uint64_t next_start_index;
};

char *duplicate ( const char *array, const uint32_t array_size );

scs_iterator_t scs_split ( const scs_t scs, const char *delimiter, const uint32_t delimiter_size )
{
    /**
     * We want to use const pointers inside the struct,
     * the compiler will not allow us to allocate memory for the struct and change its internal data
     * This is why we allocate this way
     */
    struct scs_iterator it = { .origin = scs,
                               .delimiter = duplicate ( delimiter, delimiter_size ),
                               .delimiter_size = delimiter_size,
                               .next_start_index = 0 };

    struct scs_iterator *iterator = calloc ( 1, sizeof ( struct scs_iterator ) );
    memcpy ( iterator, &it, sizeof ( struct scs_iterator ) );

    return iterator;
}

scs_t scs_iterator_next ( const scs_iterator_t scs_iterator )
{
    const uint64_t scs_original_size = scs_size ( scs_iterator->origin );
    for ( uint32_t start_index = scs_iterator->next_start_index; start_index < scs_original_size;
          start_index++ )
    {
        bool found = true;
        for ( uint32_t delimiter_index = 0; delimiter_index < scs_iterator->delimiter_size &&
                                            start_index + delimiter_index < scs_original_size;
              delimiter_index++ )
        {
            if ( scs_iterator->origin[start_index + delimiter_index] !=
                 scs_iterator->delimiter[delimiter_index] )
            {
                found = false;
            }
        }
        /**
         * If we found the delimiter or we reached the end of the scs string
         */
        if ( found || start_index == scs_original_size - 1 )
        {
            /**
             * We found this delimiter, now we create a scs from this array split
             */
            const uint64_t begin = scs_iterator->next_start_index;
            uint64_t end = found ? start_index : scs_original_size;

            /**
             * sets the index for the next iteration
             */
            scs_iterator->next_start_index = end + scs_iterator->delimiter_size;

            const scs_t scs = scs_from ( scs_iterator->origin + begin, end - begin );

            return scs;
        }
    }
    return NULL;
}

void scs_iterator_free ( scs_iterator_t scs_iterator )
{
    if ( scs_iterator == NULL )
    {
        return;
    }
    free ( (void *)scs_iterator->delimiter );
    free ( scs_iterator );
}

char *duplicate ( const char *array, const uint32_t array_size )
{
    char *newarray = calloc ( array_size, sizeof ( char ) );
    memcpy ( newarray, array, array_size );
    return newarray;
}
