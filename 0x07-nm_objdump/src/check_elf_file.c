#include "../inc/readelf.h"

/**
 * check_elf_file - check that the file is actually ELF
 *
 * @mag: magic file string
 *
 * Return: 0 for ELF format, otherwise -1
 */
int check_elf_file(unsigned char *mag)
{
	if (!mag)
		return (-1);
	if (mag[EI_MAG0] != ELFMAG0)
		return (-1);
	if (mag[EI_MAG1] != ELFMAG1)
		return (-1);
	if (mag[EI_MAG2] != ELFMAG2)
		return (-1);
	if (mag[EI_MAG3] != ELFMAG3)
		return (-1);
	if (mag[EI_CLASS] != ELFCLASS64 && mag[EI_CLASS] != ELFCLASS32)
		return (-1);
	return (0);
}
