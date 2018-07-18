#include "strace.h"
#include "syscalls.h"

/**
 * print_flags_arg - print the flags argument of a syscall
 *
 * @pid: process ID of child to ptrace
 * @param: parameter register data
 * @i: index of syscall in syscall array
 * @j: index of parameter in syscall
 *
 * Return: 1 if printing happened, otherwise 0
 */
int print_flags_arg(pid_t pid, unsigned long long int param, int i, int j)
{
	int ret = 0, count = 0;

	if (syscalls_64_g[i].nr == 2 && j == 1)
	{
		ret = 1;
		print_open_flags(pid, param, i, j);
	}
	return (ret);
}

/**
 * print_open_flags - print the flags for the open syscall
 */
void print_open_flags(pid_t pid, unsigned long long int param, int i, int j)
{
	int count = 0;

	if (IS_BIT_SET(param, O_RDONLY))
		printf("O_RDONLY"), count++;
	if (IS_BIT_SET(param, O_WRONLY))
	{
		(count) ? printf("|") : 0 ;
		printf("O_WRONLY");
	}
	if (IS_BIT_SET(param, O_CLOEXEC))
	{
		(count) ? printf("|") : 0 ;
		printf("O_CLOEXEC"), count++;
	}
}
