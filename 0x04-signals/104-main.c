#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "signals.h"

/* Our functions */
int _signals_block(int *signals);
int _signals_unblock(int *signals);

/**
 * handler - Handler for pending signals
 * @signum: Signal number
 */
void handler(int signum)
{
    printf("\nCaught signal %d\n", signum);
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    int signals[] = {
        SIGINT,
        SIGQUIT,
        SIGSEGV,
        SIGTRAP,
        0
    };

    if (_signals_block(signals) == -1)
    {
        fprintf(stderr, "Failed to block signals\n");
        return (EXIT_FAILURE);
    }

    sleep(5);
    if (handle_pending(&handler) == -1)
    {
        printf("Failed to set up handler for pending signals\n");
        return (EXIT_FAILURE);
    }

    if (_signals_unblock(signals) == -1)
    {
        fprintf(stderr, "Failed to unblock signals\n");
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
