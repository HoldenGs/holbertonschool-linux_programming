#include "../inc/readelf.h"

/**
 * load_elf_contents - load elf file contents and size into a data structure
 *
 * @elf: elf file data structure
 * @file: file to load from
 *
 * Return: 0 if success, -1 for failure
 */
int load_elf_contents(Elf_t *elf, char *file)
{
	int fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "Couldn't open %s: %s\n",
			file, strerror(errno));
		exit(errno);
	}
	elf->size = lseek(fd, 0, SEEK_END);
	if (elf->size == (off_t)-1)
	{
		fprintf(stderr, "lseek error on file %s: %s\n",
			file, strerror(errno));
		exit(errno);
	}
	if (lseek(fd, 0, SEEK_SET) == ((off_t)-1))
	{
		fprintf(stderr, "lseek error on file %s: %s\n",
			file, strerror(errno));
		exit(errno);
	}
	elf->data = safe_malloc(elf->size);
	if (read(fd, elf->data, elf->size) == -1)
	{
		fprintf(stderr, "read error on file %s: %s\n",
			file, strerror(errno));
		exit(errno);
	}
	close(fd);
	elf->filename = file;
	return (0);
}
