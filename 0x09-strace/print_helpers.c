#include "strace.h"
#include "syscalls.h"

/**
 * print_syscall_with_params - print out the system call and it's parameters
 *
 * regs: Registers of the tracee
 */
void print_syscall_with_params(struct user_regs_struct regs, pid_t pid)
{
	unsigned long long int param;
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
	while (1)
	{
		if (read + sizeof(tmp) > allocated)
		{
			allocated *= 2;
			val = realloc(val, allocated);
			if (val == NULL)
				return (NULL);
		}
		tmp = ptrace(PTRACE_PEEKDATA, child, addr + read);
		if (errno != 0)
		{
			val[read] = 0;
			break;
		}
		memcpy(val + read, &tmp, sizeof(tmp));
		if (memchr(&tmp, 0, sizeof(tmp)) != NULL)
			break;
		read += sizeof(tmp);
	}
	return (val);
}