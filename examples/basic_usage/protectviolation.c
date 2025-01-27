#include "scs/scs.h"
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void main2 ();

static void handler ( int signal, siginfo_t *info, void *unsued )
{
    fprintf ( stderr, "Got signal: %d at address: 0x%lx\n", signal, (long)info->si_addr );
    main2 ();
    exit ( 0 );
}

static void main2 ()
{
    const char *input = "This memory region is protected!";
    scs_t rd_string = scs_from_readonly ( input, strlen ( input ) );
    puts ( rd_string );
    scs_free ( rd_string );
    // Since we already freed, we can now update the memory region,
    // this should work because we "allow the write to this memory region now"
    rd_string[0] = '%';
    rd_string[1] = '&';
    rd_string[1] = '#';
    rd_string[1] = '@';
    puts ( rd_string );
}

int main ( int argc, char **argv )
{

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset ( &sa.sa_mask );
    sa.sa_sigaction = handler;
    const bool updatable = false;

    if ( sigaction ( SIGSEGV, &sa, NULL ) == -1 )
    {
        perror ( "Error registering SIGSEV ..." );
    }

    const char *input = "Someone is trying to access a protected memory region ... ";

    scs_t rd_string = scs_from_readonly ( input, strlen ( input ) );

    puts ( rd_string );

    rd_string[5] = 0xFF;

    fprintf ( stderr, "We shouldnt reach here ...\n" );
    return 1;
}