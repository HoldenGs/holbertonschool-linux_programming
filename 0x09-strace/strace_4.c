#include "strace.h"
#include "syscalls.h"

/**
 * main - Prints out system calls along with argument information
 *
 * @ac: argument count
 * @av: argument vector
 * @environ: environment variables
 *
 * Return: 0
 */
int main(int ac, char **av, char **environ)
{
	int i;
	pid_t pid;
	char *args[ac];

	pid = fork();
	if (pid == 0)
	{
		if (child_trace(ac - 1, av + 1, environ) == -1)
			return (-1);
	}
	else
	{
		memcpy(args, av + 1, (ac - 1) * sizeof(char *));
		args[ac - 1] = NULL;
		printf("execve(\"%s\", [", args[0]);
		for (i = 0; i < ac - 1; i++)
		{
			printf("\"%s\"", args[i]);
			if (i < ac - 2)
				printf(", ");
			else
				printf("], ");
		}
		for (i = 0; environ[i]; i++)
			;
		printf("[/* %d vars */]", i);
		if (parent_trace(pid) == -1)
			return (-1);
	}

	return (0);
}

/**
 * parent_trace - trace the system call ids made by the child being traced
 *
 * @pid: child pid
 *
 * Return: 0 on success, -1 on failure
 */
int parent_trace(pid_t pid)
{
	int status;

	waitpid(pid, &status, 0);
	ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);
	while (1)
	{
		struct user_regs_struct regs;

		if (wait_for_syscall(pid) == 1)
			break;

		ptrace(PTRACE_GETREGS, pid, 0, &regs);
		if (regs.orig_rax != 59)
			print_syscall_with_params(regs, pid);
		if (regs.orig_rax == 231)
			printf(") = ?\n");

		fflush(stdout);

		if (wait_for_syscall(pid) == 1)
			break;

		ptrace(PTRACE_GETREGS, pid, 0, &regs);
		if (regs.rax == 0)
			printf(") = %llx\n", regs.rax);
		else
			printf(") = 0x%llx\n", regs.rax);
	}

	return (0);
}

/**
 * wait_for_syscall - wait for a ptraced system call, then return
 *
 * @pid: traced child's pid
 *
 * Return: 0 for a system call stop, 1 for an exited process.
 */
int wait_for_syscall(pid_t pid)
{
	int status;

	while (1)
	{
		ptrace(PTRACE_SYSCALL, pid, 0, 0);
		waitpid(pid, &status, 0);
		if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80)
			return (0);
		if (WIFEXITED(status))
			return (1);
	}
}

/**
 * child_trace - begin tracing the child process
 *
 * @ac: argument count
 * @av: argument vector
 * @environ: environment variables
 *
 * Return: nothing on success, -1 on failure
 */
int child_trace(int ac, char **av, char **environ)
{
	int p_ret, e_ret, i;
	char *args[ac + 1];

	memcpy(args, av, ac * sizeof(char *));
	args[ac] = NULL;
	p_ret = ptrace(PTRACE_TRACEME, 0, 0, 0);
	kill(getpid(), SIGSTOP);
	e_ret = execve(args[0], args, environ);
	free(args);
	if (e_ret == -1 || p_ret == -1)
		return (-1);
	return (0);
}
