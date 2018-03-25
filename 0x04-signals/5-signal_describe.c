#include "signals.h"
#include <string.h>
#include <stdlib.h>

/**
 * main - describe the signal number given as an argument
 *
 * @ac: arg count
 * @arg vector
 *
 * Return: 0 if success, EXIT_FAILURE on failure
 */
int main(int ac, char **av)
{
    int signum;

    if (ac != 2)
    {
        printf("Usage: %s <signum>\n", av[0]);
        exit(EXIT_FAILURE);
    }
    signum = atoi(av[1]);
    printf("%d: %s\n", signum, strsignal(signum));
    return (0);
}