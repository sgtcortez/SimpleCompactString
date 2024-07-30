#ifndef SIMPLECOMPACTSTRING_SCS_H
#define SIMPLECOMPACTSTRING_SCS_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void *scs_t;

    scs_t scs_from_string ( char * );
    scs_t scs_from ( char *, uint64_t );
    uint64_t scs_size ( scs_t );
    void scs_free ( scs_t );
    char *scs_to_string (scs_t);

#ifdef __cplusplus
}
#endif

#endif // SIMPLECOMPACTSTRING_SCS_H
