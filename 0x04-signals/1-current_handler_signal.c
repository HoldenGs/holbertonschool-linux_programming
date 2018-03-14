#include "signals.h"

/**
 * current_handler_signal - find the current handler for the SIGINT signal
 *
 * Return: the current signal handler function for SIGINT
 */
void (*current_handler_signal(void))(int)
{
    typedef void (*sighandler_t)(int);
    sighandler_t current_signal;

    current_signal = signal(SIGINT, SIG_IGN);
    signal(SIGINT, current_signal);
    return (current_signal);
}
