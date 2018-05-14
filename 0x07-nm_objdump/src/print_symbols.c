#include "../inc/readelf.h"


/**
 * print_symbols - print the array of symbols
 *
 * @elf: elf data structure
 * @symbols: list of symbols
 * @arg0: argument 0 of program
 */
void print_symbols(Elf_t *elf, Sym_t *symbols, char *arg0)
{
	Section_t *symtab;
	int symtab_entries, i;
	char letter;

	symtab = get_section_by_name(elf, ".symtab");
	if (symtab == NULL)
	{
		fprintf(stderr, "%s: %s: no symbols\n", arg0, elf->filename);
		exit(-1);
	}
	symtab_entries = symtab->header.sh_size / symtab->header.sh_entsize;
	for (i = 0; i < symtab_entries; i++)
	{
		letter = get_symbol_letter(elf, symbols[i].sym);
		if (IS_CLASS_64(elf) &&
			ELF64_ST_TYPE(symbols[i].sym.st_info) != STT_FILE)
		{
			if (strcmp(symbols[i].name, ""))
			{
				if ((!symbols[i].sym.st_value || letter == 'U') &&
					safe_cmp(symbols[i].name, "main"))
					printf("                 %c %s\n", letter,
						symbols[i].name);
				else
					printf("%016lx %c %s\n", symbols[i].sym.st_value,
						letter, symbols[i].name);
			}
		}
		else if (ELF32_ST_TYPE(symbols[i].sym.st_info) != STT_FILE)
		{
			if (strcmp(symbols[i].name, ""))
			{
				if ((!symbols[i].sym.st_value || letter == 'U') &&
					safe_cmp(symbols[i].name, "main"))
					printf("         %c %s\n", letter, symbols[i].name);
				else
					printf("%08lx %c %s\n", symbols[i].sym.st_value,
						letter, symbols[i].name);
			}
		}
	}
}
