#include "../inc/readelf.h"

/**
 * get_section_by_name - retrieve a section by name
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


/**
 * get_section_by_type - retrieve a section by type
 *
 * @elf: elf data structure
 * @type: type of section
 *
 * Return: section associated with @name, or NULL if not found
 */
Section_t *get_section_by_type(Elf_t *elf, int type)
{
	int i;

	for (i = 0; i < elf->header->e_shnum; i++)
		if (elf->sections[i].header.sh_type == type)
			return (&elf->sections[i]);
	return (NULL);
}
