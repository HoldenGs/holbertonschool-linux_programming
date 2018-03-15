#ifndef SIGNALS_H
#define SIGNALS_H


#include <stdio.h>
#include <signal.h>

int handle_signal(void);
void gotcha(int);
void (*current_handler_signal(void))(int);
int handle_sigaction(void);
void (*current_handler_sigaction(void))(int);
int trace_signal_sender(void);
void trace_signal(int, siginfo_t *, void *);


#endif