#include "signals.h"
#include <string.h>

/**
 * handle_pending - add a handler for all of the currently blocked signals
 *
 * @handler: pointer to the handler function
 *
 * Return: 0 if success, -1 if error setting handler or reading the
 * existing blocked sigset
 */
int handle_pending(void (*handler)(int))
{
	int i;
	sigset_t set;
	struct sigaction sa;

	sigemptyset(&set);
	if (sigprocmask(SIG_BLOCK, NULL, &set) == -1)
		return (-1);
	sa.sa_handler = handler;
	for (i = 1; strstr(strsignal(i), "Unknown signal") == NULL; i++)
		if (sigismember(&set, i))
			if (sigaction(i, &sa, NULL) == -1)
				return (-1);
	return (0);
}
