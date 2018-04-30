#include "../inc/readelf.h"

int check_elf_file(Elf64_Ehdr *e_hdr)
{
	if (!e_hdr)
		return (1);
	if (e_hdr->e_ident[EI_MAG0] != ELFMAG0)
		return (1);
	if (e_hdr->e_ident[EI_MAG1] != ELFMAG1)
		return (1);
	if (e_hdr->e_ident[EI_MAG2] != ELFMAG2)
		return (1);
	if (e_hdr->e_ident[EI_MAG3] != ELFMAG3)
		return (1);
	return (0);
}
