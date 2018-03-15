#ifndef SIGNALS_H
#define SIGNALS_H


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int handle_signal(void);
void gotcha(int);
void (*current_handler_signal(void))(int);
int handle_sigaction(void);
void (*current_handler_sigaction(void))(int);
int trace_signal_sender(void);
void trace_signal(int, siginfo_t *, void *);
int pid_exist(pid_t);
void all_in_one(void);
void handler(int, siginfo_t *, void *);
int sigset_init(sigset_t *, int *);
int signals_block(int *);


#endif