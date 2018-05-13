#include "../inc/readelf.h"

/**
 * load_symbols - load the list of symbols
 *
 * @elf: elf data structure
 *
 * Return: list of symbols
 */
Sym_t *load_symbols(Elf_t *elf)
{
	int i, symtab_entries;
	Section_t *symtab, *strtab;
	Sym_t *symbols;

	symtab = get_section_by_name(elf, ".symtab");
	if (symtab == NULL)
	{
		fprintf(stderr, "symtab not found in %s\n", elf->filename);
		exit(-1);
	}
	strtab = get_section_by_name(elf, ".strtab");
	if (strtab == NULL)
	{
		fprintf(stderr, "strtab not found in %s\n", elf->filename);
		exit(-1);
	}
	symtab_entries = symtab->header.sh_size / symtab->header.sh_entsize;
	symbols = safe_malloc(sizeof(Sym_t) * symtab_entries);
	for (i = 0; i < symtab_entries; i++)
	{
		symbols[i].sym = convert_symbol_values(elf, symtab, i);
		symbols[i].name = &strtab->data[symbols[i].sym.st_name];
	}
	return (symbols);
}


/**
 * convert_symbol_values - convert symbol values
 * to a universal format and add it to @elf
 *
 * @elf: elf data structure
 * @symtab: symbol table
 * @i: index of symbol
 *
 * Return: custom symbol struct
 */
Elf_Big_Sym_t convert_symbol_values(Elf_t *elf, Section_t *symtab, int i)
{
	Elf_Big_Sym_t sym;
	Elf_Sym_t *tmp_sym;
	uint64_t (*convert_endian)(size_t, int);

	convert_endian = (elf->ident[EI_DATA] == ELFDATA2MSB) ?
		convert_big_endian : convert_little_endian;
	tmp_sym = (Elf_Sym_t *)&symtab->data[i * symtab->header.sh_entsize];
	if (IS_CLASS_64(elf))
	{
		sym.st_name = CONVERT_ENDIAN(tmp_sym->Elf64.st_name);
		sym.st_info = CONVERT_ENDIAN(tmp_sym->Elf64.st_info);
		sym.st_other = CONVERT_ENDIAN(tmp_sym->Elf64.st_other);
		sym.st_shndx = CONVERT_ENDIAN(tmp_sym->Elf64.st_shndx);
		sym.st_value = CONVERT_ENDIAN(tmp_sym->Elf64.st_value);
		sym.st_size = CONVERT_ENDIAN(tmp_sym->Elf64.st_size);
	}
	else
	{
		sym.st_name = CONVERT_ENDIAN(tmp_sym->Elf32.st_name);
		sym.st_info = CONVERT_ENDIAN(tmp_sym->Elf32.st_info);
		sym.st_other = CONVERT_ENDIAN(tmp_sym->Elf32.st_other);
		sym.st_shndx = CONVERT_ENDIAN(tmp_sym->Elf32.st_shndx);
		sym.st_value = CONVERT_ENDIAN(tmp_sym->Elf32.st_value);
		sym.st_size = CONVERT_ENDIAN(tmp_sym->Elf32.st_size);
	}
	return (sym);
}
