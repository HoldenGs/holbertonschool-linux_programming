#include <stdio.h>
#include <signal.h>
#include "signals.h"

/**
 * gotcha - handles the SIGINT signal
 *
 * @signum: Signal number
 */
void gotcha(int signum)
{
    printf("Gotcha! [%d]\n", signum);
}

/**
 * handle_signal - handle how a signal will be processed
 *
 * Return: 0 if success, -1 for failure to assign signal handler
 */
int handle_signal(void)
{
    if (signal(SIGINT, gotcha) == SIG_ERR)
        return (-1);
    return (0);
}
