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
 * nm - print out symbols for an ELF file
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


/**
 *
 */
void print_sections(Elf_t *elf)
{
	int i, class;
	char *machine, *machine_prepend;

	machine_prepend = "";
	if (elf->ident[EI_CLASS] == ELFCLASS64)
		class = 64;
	else
		class = 32;
	if (elf->header->e_machine == EM_386)
		machine = "i386";
	else if (elf->header->e_machine == EM_X86_64)
	{
		machine = "x86-64";
		machine_prepend = "i386";
	}
	else
	{
		if (elf->ident[EI_DATA] == ELFDATA2MSB)
			machine = "big";
		else
			machine = "little";
	}
	printf("\n%s:     file format elf%d-%s\n", elf->filename, class, machine);
	printf("architecture: %s:%s, flags 0x%08x:\n", machine_prepend, machine);
	if (IS_CLASS_64(elf))
		printf("start address 0x%016lx\n\n", elf->header->e_entry);
	else
		printf("start address 0x%08lx\n\n", elf->header->e_entry);
	for (i = 0; i < elf->header->e_shnum; i++)
		print_section(elf, i);
}


/**
 *
 */
void print_section(Elf_t *elf, int i)
{
	int padding, idx;

	if (strcmp(elf->sections[i].name, "") &&
		strcmp(elf->sections[i].name, ".bss") &&
		strstr(elf->sections[i].name, "tab") == NULL)
	{
		printf("Contents of section %s:\n", elf->sections[i].name);
		for (idx = 0; idx < elf->sections[i].header.sh_size; idx += 16)
		{
			padding = hex_len(elf->sections[i].header.sh_addr + idx);
			if (padding < 4)
				padding = 4;
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
	return (floor(log(n) / LOG_BASE_16) + 1);
}

/**
 *
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

