#include "../inc/readelf.h"

/**
 * print_sections - print all section data in hex and ascii form,
 * as well as ELF header information
 *
 * @elf: elf data structure
 */
void print_sections(Elf_t *elf)
{
	unsigned int flags;
	int i, class;
	char *machine, *arch;

	arch = "";
	if (elf->ident[EI_CLASS] == ELFCLASS64)
		class = 64;
	else
		class = 32;
	if (elf->header->e_machine == EM_386)
	{
		machine = "i386";
		arch = "i386";
	}
	else if (elf->header->e_machine == EM_X86_64)
	{
		machine = "x86-64";
		arch = "i386:x86-64";
	}
	else
	{
		if (elf->ident[EI_DATA] == ELFDATA2MSB)
			machine = "big";
		else
			machine = "little";
		arch = "UNKNOWN!";
	}
	flags = get_flags(elf);
	printf("\n%s:     file format elf%d-%s\n",
		elf->filename, class, machine);
	printf("architecture: %s, flags 0x%08x:\n",
		arch, flags);
	print_flags(flags);
	if (IS_CLASS_64(elf))
		printf("start address 0x%016lx\n\n", elf->header->e_entry);
	else
		printf("start address 0x%08lx\n\n", elf->header->e_entry);
	for (i = 0; i < elf->header->e_shnum; i++)
		print_section(elf, i);
}


/**
 * print_section - print the section data in hex and ascii format
 *
 * @elf: elf data structure
 * @i: index of section
 */
void print_section(Elf_t *elf, int i)
{
	int padding, idx;

	if (elf->sections[i].header.sh_size > 0 &&
		strcmp(elf->sections[i].name, "") &&
		strcmp(elf->sections[i].name, ".bss") &&
		strcmp(elf->sections[i].name, ".symtab") &&
		strcmp(elf->sections[i].name, ".strtab") &&
		strcmp(elf->sections[i].name, ".shstrtab") &&
		(strstr(elf->sections[i].name, ".rel") == NULL ||
			!strcmp(elf->sections[i].name, ".rel.dyn") ||
			!strcmp(elf->sections[i].name, ".rela.dyn") ||
			!strcmp(elf->sections[i].name, ".rela.plt") ||
			!strcmp(elf->sections[i].name, ".rel.plt") ||
			!strcmp(elf->sections[i].name, ".rel.got") ||
			!strcmp(elf->sections[i].name, ".data.rel.ro")))
	{
		printf("Contents of section %s:\n", elf->sections[i].name);
		for (idx = 0; idx < elf->sections[i].header.sh_size; idx += 16)
		{
			padding = hex_len(elf->sections[i].header.sh_addr +
				elf->sections[i].header.sh_size);
			if (padding < 4)
				padding = 4;
			else if (padding > 8)
				padding = 8;
			printf(" %0*lx", padding, elf->sections[i].header.sh_addr + idx);
			print_hex(
				elf->sections[i].data,
				elf->sections[i].header.sh_size,
				idx);
			print_ascii(
				elf->sections[i].data,
				elf->sections[i].header.sh_size,
				idx);
			putchar('\n');
		}
	}
}

/**
 * print_hex - print out the next sixteen bytes of data as hex values
 *
 * @data: memory area to print
 * @size: size of memory area @data
 * @offset: current offset into memory area @data
 */
void print_hex(unsigned char *data, uint64_t size, int offset)
{
	int i;

	for (i = 0; i < 16; i++)
	{
		if (i % 4 == 0)
			putchar(' ');
		if (i + offset < size)
			printf("%02x", data[i + offset]);
		else
			printf("  ");
	}
}


/**
 * print_ascii - print out the next sixteen bytes of data as ascii values
 *
 * @data: memory area to print
 * @size: size of memory area @data
 * @offset: current offset into memory area @data
 */
void print_ascii(unsigned char *data, uint64_t size, int offset)
{
	int i;
	char c;

	printf("  ");
	for (i = 0; i < 16; i++)
	{
		if (i + offset < size)
		{
			c = data[i + offset];
			if (c < 32 || c >= 127)
				c = '.';
		}
		else
			c = ' ';
		putchar(c);
	}
}


/**
 * hex_len - calculate the digit length of a given hex number
 *
 * @n: number
 *
 * Return: hex digit length
 */
int hex_len(uint64_t n)
{

	if (n <= 65535)
		return (4);
	if (n <= 1048575)
		return (5);
	if (n <= 16777215)
		return (6);
	if (n <= 268435455)
		return (7);
	return (8);
}
