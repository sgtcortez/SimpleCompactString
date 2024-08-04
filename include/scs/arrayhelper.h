#ifndef SIMPLECOMPACTSTRING_ARRAYHELPER_H
#define SIMPLECOMPACTSTRING_ARRAYHELPER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Stores an unsigned 64 bit number in an array
     */
    void array_store ( char *array, uint64_t number, uint8_t bytes );

    /**
     * Restores an unsigned 64 bit number from an array
     * @return
     */
    uint64_t array_restore ( const char *array, uint8_t bytes );

    /**
     * Count how many bytes are needed to store this number
     */
    uint8_t count_bytes ( const uint64_t number );

#ifdef __cplusplus
}
#endif

#endif // SIMPLECOMPACTSTRING_ARRAYHELPER_H
