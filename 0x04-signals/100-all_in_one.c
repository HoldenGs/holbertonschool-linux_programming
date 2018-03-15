#include "signals.h"
#include <string.h>

/**
 *
 */
void all_in_one(void)
{
	int i;
	struct sigaction sa;
	sa.sa_sigaction = &handler;
   	sa.sa_flags = SA_SIGINFO;
	for (i = 1; strstr(strsignal(i), "Unknown signal") == NULL; i++)
		sigaction(i, &sa, NULL);
}

/**
 *
 */
void handler(int signum, siginfo_t *info, void *context)
{
	(void) signum;
	(void) context;
	psiginfo(info, "Caught");
}