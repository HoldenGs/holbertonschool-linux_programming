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
<<<<<<< HEAD
char *file_to_buffer(fd_list_t **, fd_list_t **, const int);
=======
>>>>>>> 8361edbf9fa6a9d83a88218ab182d18ecc7d0710
fd_list_t *prepend_fd(fd_list_t **, int);
char *_realloc(char *, int, int);
char *remove_null_bytes(char *, int);


#endif