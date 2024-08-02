#include <stdio.h>

#include "scs/scs.h"
#include <stdlib.h>

int main ( int argc, char **argv )
{
    scs_t string = scs_from_string ( "Hello, my name is sgt-cortez!. Nice to meet with you ..." );

    char *str = scs_to_string ( string );

    puts ( str );
    scs_free ( string );

    return 0;
}
