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
	return (nm(av[1]));
}


/**
 * nm - print out symbols for an ELF file
 *
 * @file: name of file
 *
 * Return: 0 for success, -1 for failure
 */
int nm(char *file)
{
	Sym_t *symbols;
	Elf_t elf;

	load_elf_contents(&elf, file);

	load_elf_header(&elf);

	load_sections(&elf);

	symbols = load_symbols(&elf);

	print_symbols(&elf, symbols);

	free_elf(&elf);

	return (0);
}


/**
 * get_section_by_name - get the symtab section header
 *
 * @elf: elf data structure
 * @name: name of section
 *
 * Return: section associated with @name, or NULL if not found
 */
Section_t *get_section_by_name(Elf_t *elf, char *name)
{
	int i;

	for (i = 0; i < elf->header->e_shnum; i++)
		if (!strcmp(elf->sections[i].name, name))
			return (&elf->sections[i]);
	return (NULL);
}


