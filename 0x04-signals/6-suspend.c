#include "signals.h"
#include <unistd.h>

/**
 * handler - signal handler for SIGINT
 *
 * @signum: signal number
 */
void handler(int signum)
{
	printf("Caught %d\n", signum);
}
void handler(int);

/**
 * main - main funtion for program
 *
 * Return: EXIT_SUCCESS
 */
int main(void)
{
	signal(SIGINT, handler);
	pause();
	puts("Signal received");
	return (EXIT_SUCCESS);
}
