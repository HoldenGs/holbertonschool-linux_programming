
#ifndef _STRACE_H_
#define _STRACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>

/* Prototypes */
int parent_trace(pid_t pid);
int wait_for_syscall(pid_t pid);
int child_trace(int ac, char **av);
void print_syscall_with_params(struct user_regs_struct regs);

#endif /* _STRACE_H_ */