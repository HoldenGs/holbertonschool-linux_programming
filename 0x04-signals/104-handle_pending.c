#include "signals.h"
#include <string.h>

/**
 *
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
			sigaction(i, &sa, NULL);
	return (0);
}
