#ifndef SIGNALS_H
#define SIGNALS_H


#include <stdio.h>
#include <signal.h>

int handle_signal(void);
void gotcha(int);
void (*current_handler_signal(void))(int);
int handle_sigaction(void);
void (*current_handler_sigaction(void))(int);


#endif