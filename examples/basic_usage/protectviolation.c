#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include "scs/scs.h"

static void handler(int signal, siginfo_t *info, void * unsued)
{
    fprintf(stderr, "Got signal: %d at address: 0x%lx\n", signal, (long) info->si_addr);
    exit(0);
}

int main ( int argc, char **argv )
{

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handler;
    const bool updatable = false;


    if (sigaction(SIGSEGV, &sa, NULL) == -1)
    {
        perror("Error registering SIGSEV ...");
    }

    scs_t string = scs_from_string ( "Someone is trying to access a protected memory region ... ", updatable);

    puts(string);

    string[5] = 0xFF;

    fprintf(stderr, "We shouldnt reach here ...\n");
    return 1;
}
