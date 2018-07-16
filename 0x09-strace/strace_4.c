#include "strace.h"
#include "syscalls.h"

extern char **environ;

/**
 * main - sort of working system call number printer. Prints out system calls twice
 *
 * @ac: argument count
 * @av: argument vector
 *
 * Return: 0
 */
int main(int ac, char **av)
{
	int i;
	pid_t pid;
	char *args[ac];

	pid = fork();
	if (pid == 0)
	{
		if (child_trace(ac - 1, av + 1) == -1)
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
		for (i = 0; environ[i]; i++);
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
 * print_syscall_with_params - print out the system call and it's parameters
 *
 * regs: Registers of the tracee
 */
void print_syscall_with_params(struct user_regs_struct regs, pid_t pid)
{
	long long unsigned int param;
	char *syscall_name, *string;
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
			param = regs.r10;
		if (j == 4)
			param = regs.r8;
		if (j == 5)
			param = regs.r9;
		if (syscalls_64_g[i].params[j] == CHAR_P)
		{
			string = read_string(pid, param);
			if (string == NULL)
				return;
			printf("\"%s\"", string);
			free(string);
		}
		else if (syscalls_64_g[i].params[j] == VARARGS)
			printf("...");
		else if (param != 0 && regs.orig_rax != 59)
			printf("0x%llx", param);
		else
			printf("0");
		if (j < syscalls_64_g[i].nb_params - 1)
			printf(", ");
	}
}

/**
 * read_string - read a string from an address until the end. Found on
 * StackOverflow:
 * https://stackoverflow.com/questions/10385784/how-to-get-a-char-with-ptrace
 *
 * @child: child pid to trace
 * @addr: address in child userspace to peek at
 *
 * Return: address of allocated string, or NULL on failure
 */
char *read_string(int child, unsigned long addr)
{
    char *val = malloc(4096);
    int allocated = 4096, read = 0;
    unsigned long tmp = 0;

	if (val == NULL)
		return (NULL);
    while (1) {
        if (read + sizeof(tmp) > allocated)
		{
            allocated *= 2;
            val = realloc(val, allocated);
			if (val == NULL)
				return (NULL);
        }
        tmp = ptrace(PTRACE_PEEKDATA, child, addr + read);
        if(errno != 0)
		{
            val[read] = 0;
            break;
        }
        memcpy(val + read, &tmp, sizeof(tmp));
        if (memchr(&tmp, 0, sizeof(tmp)) != NULL) break;
        read += sizeof(tmp);
    }
    return (val);
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
int child_trace(int ac, char **av)
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
