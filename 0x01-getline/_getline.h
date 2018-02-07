#ifndef _GETLINE_H


#define READ_SIZE 64


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


char *_getline(int);
char *add_buffer_space(int, char *);
int check_for_newline(char *, int);
char *remove_null_bytes(char *, int);


#endif