#include "signals.h"

/**
 * sigset_init - initialize a sigset
 *
 * @set: set structure to initialize
 * @signals: array of signal numbers to add to set
 *
 * Return: 0
 */
int sigset_init(sigset_t *set, int *signals)
{
	int i;

	sigemptyset(set);
	for (i = 0; signals[i] != 0; i++)
		sigaddset(set, signals[i]);
	return (0);
}
