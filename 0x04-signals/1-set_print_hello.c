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
 *
 * Return: 0 for success, -1 if failure to assign signal handler
 */
void set_print_hello(void)
{
    signal(SIGINT, print_hello);
}
