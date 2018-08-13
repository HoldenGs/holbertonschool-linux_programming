#include "malloc.h"

/**
 * naive_malloc - basically a wrapper for sbrk
 *
 * @size: size to extend data segment by
 *
 * Return: pointer to allocated data segment
 */
void *naive_malloc(size_t size)
{
	static void *heap_start = NULL;
	static size_t chunk_num = 0, page_multiple = 1;
	void *prev, *ret, *brk_ret;
	size_t page_size, chunk_size, i, space_left, offset;

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
			space_left = page_size * page_multiple - (((char *)prev + sizeof(size_t) + size) - (char *)heap_start);
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
	*((size_t *)((char *)ret + size)) = page_size * page_multiple - (((char *)ret + size) - (char *)heap_start);
	chunk_num += 1;
	return (ret);
}
