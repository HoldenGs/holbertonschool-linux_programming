#include "malloc.h"

/**
 * _malloc - allocate space in a way that can be easily freed and reallocated
 *
 * @size: number of bytes to allocate
 *
 * Return: A pointer to the allocated space
 */
void *_malloc(size_t size)
{
	return (&size);
}
