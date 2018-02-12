#ifndef _GETLINE_H


#define READ_SIZE 64


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


typedef struct fd_list_s
{
	char *buf;
	int bytes_left;
	int fd;
	struct fd_list_s *next;
} fd_list_t;


char *_getline(int);
char *_memchr(char *, char, int);
fd_list_t *prepend_fd(fd_list_t **, int);
char *_realloc(char *, int);
char *remove_null_bytes(char *, int);


#endif