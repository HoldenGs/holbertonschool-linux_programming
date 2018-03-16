#include "signals.h"

/**
 * signals_unblock - unblock all signals specified in the array @signals
 *
 * @signals: array of signal numbers to unblock
 *
 * Return: 0 if success, -1 for an error setting the unblocked signals
 */
int signals_unblock(int *signals)
{
	int i;
	sigset_t set;

	sigemptyset(&set);
	for (i = 0; signals[i] != 0; i++)
		sigaddset(&set, signals[i]);
	if (sigprocmask(SIG_UNBLOCK, &set, NULL) == -1)
		return (-1);
	return (0);
}
