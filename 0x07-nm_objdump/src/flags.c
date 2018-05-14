#include "../inc/readelf.h"


/**
 * get_flags - compile all flags into one number
 *
 * @elf: elf data structure
 *
 * Return: flags in an integer
 */
unsigned int get_flags(Elf_t *elf)
{
	unsigned int flags;

	flags = BFD_NO_FLAGS;
	if (elf->header->e_type == ET_EXEC)
		flags |= EXEC_P;
	if (elf->header->e_type == ET_DYN ||
		elf->header->e_type == ET_EXEC)
		flags |= D_PAGED;
	if (elf->header->e_type == ET_DYN)
		flags |= DYNAMIC;
	if (get_section_by_type(elf, SHT_REL))
		flags |= HAS_RELOC;
	if (!(flags & EXEC_P) && get_section_by_type(elf, SHT_RELA))
		flags |= HAS_RELOC;
	if (get_section_by_type(elf, SHT_SYMTAB) ||
		get_section_by_type(elf, SHT_DYNSYM))
		flags |= HAS_SYMS;
	return (flags);
}


/**
 * print_flags - print flags
 *
 * @flags: flags in integer form
 */
void print_flags(unsigned int flags)
{
	int ret;

	ret = 0;
	if (flags == BFD_NO_FLAGS)
		printf("BFD_NO_FLAGS");
	else
	{
		ret |= _F(flags, HAS_RELOC, ret);
		ret |= _F(flags, EXEC_P, ret);
		ret |= _F(flags, HAS_LINENO, ret);
		ret |= _F(flags, HAS_DEBUG, ret);
		ret |= _F(flags, HAS_SYMS, ret);
		ret |= _F(flags, DYNAMIC, ret);
		ret |= _F(flags, WP_TEXT, ret);
		ret |= _F(flags, D_PAGED, ret);
	}
	printf("\n");
}
