#include "signals.h"


/**
 * gotcha - print line whenever a signal is caught
 *
 * @signum: signal number
 */
void gotcha(int signum)
{
    printf("Gotcha! [%d]\n", signum);
}

/**
 * handle_sigaction - assign gotcha() to handle the signal SIGINT usings sigaction()
 *
 * Return: 0 if success, -1 for failure to assign signal handler
 */
int handle_sigaction(void)
{
    struct sigaction sa;

    sa.sa_handler = &gotcha;
    return (sigaction(SIGINT, &sa, NULL));
}
