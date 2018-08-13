
#ifndef _MALLOC_H_
#define _MALLOC_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void *naive_malloc(size_t size);

void *_malloc(size_t size);

void _free(void *ptr);

void *_calloc(size_t nmemb, size_t size);

void *_realloc(void *ptr, size_t size);


#endif /* _MALLOC_H_ */
