#include "strace.h"
#include "syscalls.h"

/**
 * main - sort of working system call number printer. Prints out system calls twice
 *
 * @ac: argument count
 * @av: argument vector
 *
 * Return: 0
 */
int main(int ac, char **av, char **environ)
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		if (child_trace(ac - 1, av + 1, environ) == -1)
			return (-1);
	}
	else
	{
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
 * print_syscall_with_params - print out the system call and it's parameters
 *
 * @regs: Registers of the tracee
 * @pid: child pid
 */
int print_syscall_with_params(struct user_regs_struct regs, pid_t pid)
{
	unsigned long int param;
	char *syscall_name;
	int i, j;

	for (i = 0; i < 318; i++)
	{
		if (regs.orig_rax == syscalls_64_g[i].nr)
		{
			syscall_name = strdup(syscalls_64_g[i].name);
			break;
		}
	}
	printf("%s(", syscall_name);
	free(syscall_name);
	for (j = 0; j < syscalls_64_g[i].nb_params; j++)
	{
		if (j == 0)
			param = regs.rdi;
		if (j == 1)
			param = regs.rsi;
		if (j == 2)
			param = regs.rdx;
		if (j == 3)
			param = regs.rcx;
		if (j == 4)
			param = regs.r8;
		if (j == 5)
			param = regs.r9;
		if (syscalls_64_g[i].params[j] == VARARGS)
			printf("...");
		else if (param != 0 && regs.orig_rax != 59)
			printf("0x%lx", param);
		else
			printf("0");
		if (j < syscalls_64_g[i].nb_params - 1)
			printf(", ");
	}
	return (i);
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
 * 
 * Return: nothing on success, -1 on failure
 */
int child_trace(int ac, char **av, char **environ)
{
	int p_ret, e_ret;
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
