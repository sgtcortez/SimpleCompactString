#ifndef SIMPLECOMPACTSTRING_SCS_H
#define SIMPLECOMPACTSTRING_SCS_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef char *scs_t;
    typedef struct scs_iterator *scs_iterator_t;

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

    /**
     * Split the given scs using the given delimiter
     * This function only creates the iterator
     */
    scs_iterator_t scs_split ( const scs_t scs, const char *delimiter,
                               const uint32_t delimiter_size );

    /**
     * Returns the next entry
     * Returns NULL if reached the end
     */
    scs_t scs_iterator_next ( const scs_iterator_t scs_iterator );

    /**
     * Frees the iterator object
     */
    void scs_iterator_free ( scs_iterator_t scs_iterator );

#ifdef __cplusplus
}
#endif

#endif // SIMPLECOMPACTSTRING_SCS_H
