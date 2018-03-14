#include "signals.h"

/**
 * current_handler_sigaction - find the function pointer of the SIGINT signal handler using sigaction()
 *
 * Return: pointer to signal handler, or NULL on failure
 */
void (*current_handler_sigaction(void))(int)
{
    struct sigaction sa, oldsa;

    sa.sa_handler = SIG_IGN;
    if (sigaction(SIGINT, &sa, &oldsa) == -1)
        return (NULL);
    if (sigaction(SIGINT, &oldsa, NULL) == -1)
        return (NULL);
    return (oldsa.sa_handler);
}
