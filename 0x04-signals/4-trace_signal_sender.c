#include "signals.h"

/**
 * trace_signal - print out the pid of the signal sender
 *
 * @signum: signal number
 * @info: signal info
 * @context: signal context
 */
void trace_signal(int signum, siginfo_t *info, void *context)
{
    (void) context;
    (void) signum;

    printf("SIGQUIT sent by %d\n", info->si_pid);
}

/**
 * trace_signal_sender - set the signal handler for SIGQUIT
 *
 * Return: 0 for success, -1 for error setting the signal handler
 */
int trace_signal_sender(void)
{
    struct sigaction sa;

    sa.sa_sigaction = &trace_signal;
    sa.sa_flags = SA_SIGINFO;
    return (sigaction(SIGQUIT, &sa, NULL));
}
