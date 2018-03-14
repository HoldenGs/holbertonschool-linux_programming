#include "signals.h"

/**
 * print_hello - print hello when a signal is caught
 *
 * @signum: signal number
 */
void print_hello(int signum)
{
    (void) signum;
    printf("Hello :)\n");
}

/**
 * set_print_hello - set the signal handler for SIGINT to print_hello()
 */
void set_print_hello(void)
{
    struct sigaction sa;

    sa.sa_handler = &print_hello;
    sigaction(SIGINT, &sa, NULL);
}
