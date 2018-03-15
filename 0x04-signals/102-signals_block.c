#include "signals.h"

/**
 * signals_block - block all signals specified in the array @signals
 *
 * @signals: array of signal numbers to block
 *
 * Return: 0 if success, -1 for an error setting the blocked signals
 */
int signals_block(int *signals)
{
	int i;
	sigset_t set;

	sigemptyset(&set);
	for (i = 0; signals[i] != 0; i++)
		sigaddset(&set, signals[i]);
	if (sigprocmask(SIG_SETMASK, &set, NULL) == -1)
		return (-1);
	return (0);
}
