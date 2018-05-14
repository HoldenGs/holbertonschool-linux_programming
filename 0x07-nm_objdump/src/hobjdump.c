#include "../inc/readelf.h"


/**
 * main - program entrypoint
 *
 * @ac: argument count
 * @av: argument vector
 *
 * Return: 0 on success, 1 for failure
 */
int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("Usage\n\t%s <binary_file>\n", av[0]);
		return (1);
	}
	return (hobjdump(av[1]));
}


/**
 * hobjdump - print out section data for an ELF file
 *
 * @file: name of file
 *
 * Return: 0 for success, -1 for failure
 */
int hobjdump(char *file)
{
	Elf_t elf;

	load_elf_contents(&elf, file);

	load_elf_header(&elf);

	load_sections(&elf);

	print_sections(&elf);

	free_elf(&elf);

	return (0);
}
