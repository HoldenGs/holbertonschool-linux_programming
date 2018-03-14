#include <stdio.h>
#include <signal.h>
#include "signals.h"

/**
 *
 */
void gotcha(int signum)
{
    printf("Gotcha! [%d]\n", signum);
}

/**
 * handle_signal - handle how a signal will be processed
 */
int handle_signal(void)
{
    if (signal(SIGINT, gotcha) == SIG_ERR)
        return (-1);
    return (0);
}
