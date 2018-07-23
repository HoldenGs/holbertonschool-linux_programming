#include "strace.h"
#include "syscalls.h"

/**
 * print_syscall_with_params - print out the system call and it's parameters
 *
 * @regs: Registers of the tracee
 * @pid: child process ID
 *
 * Return: index of syscall in syscall array
 */
int print_syscall_with_params(struct user_regs_struct regs, pid_t pid)
{
	unsigned long long int param;
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
		print_arg(pid, param, i, j);
		if (j < syscalls_64_g[i].nb_params - 1)
			printf(", ");
	}

	return (i);
}

/**
 * print_arg - print an argument of a given function
 *
 * @pid: child process ID
 * @param: data to print
 * @i: index of syscall in syscall array
 * @j: index of syscall argument type
 */
void print_arg(pid_t pid, unsigned long long int param, int i, int j)
{
	char *string;

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
	else if (syscalls_64_g[i].params[j] == INT)
		printf("%d", (int)param);
	else if (syscalls_64_g[i].params[j] == SIZE_T)
		printf("%zd", (size_t)param);
	else if (syscalls_64_g[i].params[j] == LONG)
		printf("%ld", (long)param);
	else if (syscalls_64_g[i].params[j] == SSIZE_T)
		printf("%zu", (ssize_t)param);
	else if (syscalls_64_g[i].params[j] == U64)
		printf("%llu", (__u64)param);
	else if (syscalls_64_g[i].params[j] == UINT32_T)
		printf("%u", (uint32_t)param);
	else if (syscalls_64_g[i].params[j] == UNSIGNED_INT)
		printf("%u", (unsigned int)param);
	else if (syscalls_64_g[i].params[j] == UNSIGNED_LONG)
		printf("%lu", (unsigned long)param);
	else if (syscalls_64_g[i].params[j] == PID_T)
		printf("%d", (pid_t)param);
	else if (param != 0)
		printf("0x%llx", param);
	else
		printf("0");
}

/**
 * print_ret - print the return value of a given function
 *
 * @pid: child process ID
 * @param: data to print
 * @i: index of syscall in syscall array
 */
void print_ret(pid_t pid, unsigned long long int param, int i)
{
	char *string;

	printf(") = ");
	if (syscalls_64_g[i].ret == CHAR_P)
	{
		string = read_string(pid, param);
		if (string == NULL)
			return;
		printf("\"%s\"\n", string);
		free(string);
	}
	else if (syscalls_64_g[i].ret == VARARGS)
		printf("...\n");
	else if (syscalls_64_g[i].ret == INT)
		printf("%d\n", (int)param);
	else if (syscalls_64_g[i].ret == SIZE_T)
		printf("%zd\n", (size_t)param);
	else if (syscalls_64_g[i].ret == LONG)
		printf("%ld\n", (long)param);
	else if (syscalls_64_g[i].ret == SSIZE_T)
		printf("%zu\n", (ssize_t)param);
	else if (syscalls_64_g[i].ret == U64)
		printf("%llu\n", (__u64)param);
	else if (syscalls_64_g[i].ret == UINT32_T)
		printf("%u\n", (uint32_t)param);
	else if (syscalls_64_g[i].ret == UNSIGNED_INT)
		printf("%u\n", (unsigned int)param);
	else if (syscalls_64_g[i].ret == UNSIGNED_LONG)
		printf("%lu\n", (unsigned long)param);
	else if (syscalls_64_g[i].ret == PID_T)
		printf("%d\n", (pid_t)param);
	else if (param != 0)
		printf("0x%llx\n", param);
	else
		printf("0\n");
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
