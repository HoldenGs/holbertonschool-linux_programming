#include "../inc/readelf.h"


/**
 *free_elf - free the elf data structures
 *
 * @elf: elf data structure
 *
 * Return: 0 for success
 */
int free_elf(Elf_t *elf)
{
	free(elf->data);
	free(elf->sections);
	free(elf->header);
	return (0);
}


/**
 * safe_malloc - safely malloc space
 *
 * @size: size to malloc
 *
 * Return: pointer to space malloced
 */
void *safe_malloc(size_t size)
{
	void *ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		fprintf(stderr, "Couldn't malloc space of size %lu\n", size);
		exit(errno);
	}
	return (ptr);
}
