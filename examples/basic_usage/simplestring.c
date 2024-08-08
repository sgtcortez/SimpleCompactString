#include <stdio.h>

#include "scs/scs.h"
#include <stdlib.h>

int main ( int argc, char **argv )
{

    char *input = "Hello, my name is sgt-cortez!. Nice to meet with you ...";

    scs_t string = scs_from ( input, strlen ( input ) );
    puts ( string );
    scs_free ( string );
    return 0;
}
