
#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

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

#endif /* _SYSCALLS_H_ */