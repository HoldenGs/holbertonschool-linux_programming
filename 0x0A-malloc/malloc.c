#include "malloc.h"

#define CURR_OFFSET(prev, size, heap_start)		(((char *)prev + \
					sizeof(size_t) + size) - (char *)heap_start)

/**
 * _malloc - allocate space in a way that can be easily freed and reallocated
 *
 * @size: number of bytes to allocate
 *
 * Return: A pointer to the allocated space
 */
void *_malloc(size_t size)
{
	static void *heap_start;
	static size_t chunk_num = 0, page_multiple = 1;
	void *prev, *ret, *brk_ret;
	size_t page_size, chunk_size, i, space_left, offset, curr_off;

	page_size = sysconf(_SC_PAGESIZE);
	if (heap_start == NULL)
	{
		brk_ret = sbrk(page_size);
		if (brk_ret == (void *) -1)
			return (NULL);
		heap_start = brk_ret;
		prev = brk_ret;
	}
	else
	{
		prev = heap_start;
		for (i = 0; i < chunk_num; i++)
		{
			chunk_size = *(size_t *)prev;
			curr_off = CURR_OFFSET(prev, size, heap_start);
			space_left = page_size * page_multiple - curr_off;
			if (chunk_size + sizeof(size_t) * 2 >= space_left)
			{
				brk_ret = sbrk(page_size);
				if (brk_ret == (void *) -1)
					return (NULL);
				page_multiple++;
			}
			prev = (char *)prev + chunk_size;
		}
	}
	offset = size + sizeof(size_t);
	*(size_t *)prev = offset + (8 - (offset % 8)) % 8;
	ret = (char *)prev + sizeof(size_t);
	curr_off = (((char *)ret + size) - (char *)heap_start);
	*((size_t *)((char *)ret + size)) = page_size * page_multiple - curr_off;
	chunk_num += 1;
	return (ret);
}
