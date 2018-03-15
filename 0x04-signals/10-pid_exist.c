#include <signal.h>
#include <errno.h>

/**
 * pid_exist - test if a given pid exists or not, by checking the error. If the
 * errno is EPERM and not ESRCH, then it exists.
 *
 * Return: 
 */
int pid_exist(pid_t pid)
{
	return (kill(pid, 0) == 0 ? 1 : (errno == ESRCH ? 0 : 1));
}