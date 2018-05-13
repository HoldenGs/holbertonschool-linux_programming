#include "../inc/readelf.h"

/**
 * get_symbol_letter - get the letter corresponding to a given symbol
 *
 * @elf: elf data structure
 * @sym: symbol
 *
 * Return: letter
 */
char get_symbol_letter(Elf_t *elf, Elf_Big_Sym_t sym)
{
	unsigned char type, bind, letter;

	type = ELF32_ST_TYPE(sym.st_info);
	bind = ELF32_ST_BIND(sym.st_info);
	if (sym.st_shndx == SHN_ABS)
		letter = 'A';
	else if (bind == STB_WEAK && type == STT_OBJECT && !sym.st_value)
		letter = 'v';
	else if (bind == STB_WEAK && type == STT_OBJECT && sym.st_value)
		letter = 'V';
	else if (bind == STB_WEAK && !sym.st_value)
		letter = 'w';
	else if (bind == STB_WEAK && sym.st_value)
		letter = 'W';
	else if (sym.st_shndx == SHN_UNDEF)
		letter = 'U';
	else if (sym.st_shndx == SHN_COMMON)
		letter = 'C';
	else
		letter = letter_by_section(elf, sym, bind);
	return (letter);
}


/**
 * safe_cmp - compare two strings, never going further than the length
 * of the second string
 *
 * @s1: string one
 * @s2: string two
 *
 * Return: return an integer less than, equal to,
 * or greater than zero if s1 (or the first n bytes thereof) is found,
 * respectively,  to  be less than, to match, or be greater than s2.
 */
int safe_cmp(const char *s1, const char *s2)
{
	return (strncmp(s1, s2, strlen(s2)));
}


/**
 * letter_by_section - choose a letter based on the section of the symbol
 *
 * @elf: elf data structure
 * @sym: symbol to print letter for
 * @bind: bind of the symbol
 *
 * Return: letter to use
 */
char letter_by_section(Elf_t *elf, Elf_Big_Sym_t sym, unsigned char bind)
{
	char *name, letter;

	name = elf->sections[sym.st_shndx].name;
	if (!safe_cmp(name, ".init") || !safe_cmp(name, ".text") ||
		!safe_cmp(name, ".init_array") || !safe_cmp(name, ".init") ||
		!safe_cmp(name, ".fini") || !safe_cmp(name, ".fini_array") ||
		!safe_cmp(name, ".plt"))
		if (bind == STB_LOCAL)
			letter = 't';
		else
			letter = 'T';
	else if (!safe_cmp(name, ".dynamic") || !safe_cmp(name, ".data") ||
		!safe_cmp(name, ".jcr") || !safe_cmp(name, ".got.plt"))
		if (bind == STB_LOCAL)
			letter = 'd';
		else
			letter = 'D';
	else if (!safe_cmp(name, ".eh_frame") || !safe_cmp(name, ".rodata"))
		if (bind == STB_LOCAL)
			letter = 'r';
		else
			letter = 'R';
	else if (!safe_cmp(name, ".bss"))
		if (bind == STB_LOCAL)
			letter = 'b';
		else
			letter = 'B';
	else if (!safe_cmp(name, ".debug"))
		letter = 'N';
	else
		letter = '?';
	return (letter);
}
