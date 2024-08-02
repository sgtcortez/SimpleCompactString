#ifndef SIMPLECOMPACTSTRING_SCS_H
#define SIMPLECOMPACTSTRING_SCS_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void *scs_t;

    scs_t scs_from_string ( const char * );
    scs_t scs_from ( const char *, uint64_t );
    uint64_t scs_size ( const scs_t );
    void scs_free ( scs_t );
    const char *scs_to_string ( const scs_t );

#ifdef __cplusplus
}
#endif

#endif // SIMPLECOMPACTSTRING_SCS_H
