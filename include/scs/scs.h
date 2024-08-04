#ifndef SIMPLECOMPACTSTRING_SCS_H
#define SIMPLECOMPACTSTRING_SCS_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef char *scs_t;

    /**
     * Creates a scs object from a c string.
     * Note: The return scs its a c style compatible string
     */
    scs_t scs_from_string ( const char *string );

    /**
     * Creates a scs object from an array.
     */
    scs_t scs_from ( const char *array, const uint64_t array_size );

    /**
     * Returns the size of scs object
     */
    uint64_t scs_size ( const scs_t scs );

    /**
     * Frees the scs object
     */
    void scs_free ( scs_t scs );

#ifdef __cplusplus
}
#endif

#endif // SIMPLECOMPACTSTRING_SCS_H
