#include "signals.h"

/**
 * main - kill a process specified by the first argument to the main argument
 *
 * @ac: argument count
 * @av: argument vector
 *
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("Usage: %s <pid>\n", av[0]);
		exit(EXIT_FAILURE);
	}
	if (kill(atoi(av[1]), SIGINT) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
