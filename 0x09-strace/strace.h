
#ifndef _STRACE_H_
#define _STRACE_H_

#include <linux/types.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>

#define IS_BIT_SET(field, bit)	((field & bit) == bit)

/* Prototypes */
int parent_trace(pid_t pid);
int wait_for_syscall(pid_t pid);
int child_trace(int ac, char **av, char **environ);
int print_syscall_with_params(struct user_regs_struct regs, pid_t pid);
char *read_string(int child, unsigned long addr);
void print_arg(pid_t pid, unsigned long long int param, int i, int j);
void print_ret(pid_t pid, unsigned long long int param, int i);
int print_flags_arg(pid_t pid, unsigned long long int param, int i, int j);
void print_open_flags(pid_t pid, unsigned long long int param, int i, int j);

#endif /* _STRACE_H_ */