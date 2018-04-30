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
	int retval;
	FILE *fp;
	Elf64_Ehdr e_hdr;

	if (ac != 2)
	{
		printf("Usage\n\t%s <binary_file>\n", av[0]);
		return (1);
	}
	fp = fopen(av[1], "rb");
	if (fp == NULL)
	{
		fprintf(stderr, "Error - Can't open %s: %s\n", av[1], strerror(errno));
		return (1);
	}
	fread(&e_hdr, sizeof(e_hdr), 1, fp);
	if (ferror(fp))
	{
		fprintf(stderr, "fread error: %s\n", strerror(errno));
		fclose(fp);
		return (1);
	}
	if (check_elf_file(&e_hdr))
	{
		fprintf(stderr, "'%s' isn't an ELF file\n", av[1]);
		fclose(fp);
		return (1);
	}
	retval = print_symbols(fp, &e_hdr);
	
	fclose(fp);
	return (retval);
}

int print_symbols(FILE *fp, Elf64_Ehdr *e_hdr)
{
	int i, symtab_entries, dynsym_entries, dynsym_offset,
	symtab_offset; // num_symtabs;
	char *names, *name, *symtab_names, *symtab_name, *type,
	*bind, *ndx, *vis; //symtabs[2];
	Elf64_Shdr sym_hdr, s_hdr;
	Elf64_Sym symtab;

	// symtabs[0] = ".dynsym";
	// symtabs[1] = ".symtab";

	// Get the section header names
	fseek(fp, e_hdr->e_shoff + e_hdr->e_shstrndx * sizeof(s_hdr), SEEK_SET);
	fread(&s_hdr, sizeof(s_hdr), 1, fp);
	names = malloc(s_hdr.sh_size);
	fseek(fp, s_hdr.sh_offset, SEEK_SET);
	fread(names, 1, s_hdr.sh_size, fp);

	for (i = 0; i < e_hdr->e_shnum; i++)
	{
		name = "";
		fseek(fp, e_hdr->e_shoff + i * sizeof(s_hdr), SEEK_SET);
		fread(&s_hdr, sizeof(s_hdr), 1, fp);
		if (s_hdr.sh_name)
			name = names + s_hdr.sh_name;

		if (!strcmp(name, ".strtab"))
		{
			symtab_names = malloc(s_hdr.sh_size);
			fseek(fp, s_hdr.sh_offset, SEEK_SET);
			fread(symtab_names, 1, s_hdr.sh_size, fp);
		}

		if (!strcmp(name, ".symtab"))
		{
			symtab_entries = s_hdr.sh_size / s_hdr.sh_entsize;
			symtab_offset = s_hdr.sh_offset;
		}

		if (!strcmp(name, ".dynsym"))
		{
			dynsym_entries = s_hdr.sh_size / s_hdr.sh_entsize;
			dynsym_offset = s_hdr.sh_offset;
		}
	}

	printf("\nSymbol table '.dynsym' contains %d entries:\n", dynsym_entries);
	printf("   Num:    Value          Size Type    Bind   Vis      Ndx Name\n");

	// for (j = 0; j < num_symtabs; j++)
	// {

	// }

	for (i = 0; i < dynsym_entries; i++)
	{
		fseek(fp, dynsym_offset + i * sizeof(symtab), SEEK_SET);
		fread(&symtab, sizeof(symtab), 1, fp);
		symtab_name = symtab_names + symtab.st_name;

		if (ELF32_ST_TYPE(symtab.st_info) == STT_NOTYPE)
			type = "NOTYPE";
		else if (ELF32_ST_TYPE(symtab.st_info) == STT_OBJECT)
			type = "OBJECT";
		else if (ELF32_ST_TYPE(symtab.st_info) == STT_FUNC)
			type = "FUNC";
		else if (ELF32_ST_TYPE(symtab.st_info) == STT_SECTION)
			type = "SECTION";
		else if (ELF32_ST_TYPE(symtab.st_info) == STT_FILE)
			type = "FILE";
		else if (ELF32_ST_TYPE(symtab.st_info) == STT_COMMON)
			type = "COMMON";
		else if (ELF32_ST_TYPE(symtab.st_info) == STT_TLS)
			type = "TLS";
		else if (ELF32_ST_TYPE(symtab.st_info) == STT_GNU_IFUNC)
			type = "GNU_IFUNC";

		if (ELF32_ST_BIND(symtab.st_info) == STB_LOCAL)
			bind = "LOCAL";
		else if (ELF32_ST_BIND(symtab.st_info) == STB_GLOBAL)
			bind = "GLOBAL";
		else if (ELF32_ST_BIND(symtab.st_info) == STB_WEAK)
			bind = "WEAK";
		else if (ELF32_ST_BIND(symtab.st_info) == STB_GNU_UNIQUE)
			bind = "GNU_UNIQUE";

		if (ELF64_ST_VISIBILITY(symtab.st_other) == STV_DEFAULT)
			vis = "DEFAULT";
		else if (ELF64_ST_VISIBILITY(symtab.st_other) == STV_INTERNAL)
			vis = "INTERNAL";
		else if (ELF64_ST_VISIBILITY(symtab.st_other) == STV_HIDDEN)
			vis = "HIDDEN";
		else if (ELF64_ST_VISIBILITY(symtab.st_other) == STV_PROTECTED)
			vis = "PROTECTED";

		if (symtab.st_shndx == STN_UNDEF)
			ndx = "UND";
		else if (symtab.st_shndx == 1)
			ndx = "ABS";
		else if (symtab.st_shndx == 2)
			ndx = "COMMON";
		else
		{
			ndx = malloc(sizeof(char) * 4);
			sprintf(ndx, "%3d", symtab.st_shndx);
		}

		printf("%6d: %016lx %5d %-7s %-6s %-8s %s %s\n", i, (unsigned long)symtab.st_value,
			(int)symtab.st_size, type, bind, vis, ndx, symtab_name);
	}

	printf("\nSymbol table '.dynsym' contains %d entries:\n", symtab_entries);
	printf("   Num:    Value          Size Type    Bind   Vis      Ndx Name\n");
	for (i = 0; i < symtab_entries; i++)
	{
		fseek(fp, symtab_offset + i * sizeof(symtab), SEEK_SET);
		fread(&symtab, sizeof(symtab), 1, fp);
		symtab_name = symtab_names + symtab.st_name;

		if (ELF32_ST_TYPE(symtab.st_info) == STT_NOTYPE)
			type = "NOTYPE";
		else if (ELF32_ST_TYPE(symtab.st_info) == STT_OBJECT)
			type = "OBJECT";
		else if (ELF32_ST_TYPE(symtab.st_info) == STT_FUNC)
			type = "FUNC";
		else if (ELF32_ST_TYPE(symtab.st_info) == STT_SECTION)
			type = "SECTION";
		else if (ELF32_ST_TYPE(symtab.st_info) == STT_FILE)
			type = "FILE";
		else if (ELF32_ST_TYPE(symtab.st_info) == STT_COMMON)
			type = "COMMON";
		else if (ELF32_ST_TYPE(symtab.st_info) == STT_TLS)
			type = "TLS";
		else if (ELF32_ST_TYPE(symtab.st_info) == STT_GNU_IFUNC)
			type = "GNU_IFUNC";

		if (ELF32_ST_BIND(symtab.st_info) == STB_LOCAL)
			bind = "LOCAL";
		else if (ELF32_ST_BIND(symtab.st_info) == STB_GLOBAL)
			bind = "GLOBAL";
		else if (ELF32_ST_BIND(symtab.st_info) == STB_WEAK)
			bind = "WEAK";
		else if (ELF32_ST_BIND(symtab.st_info) == STB_GNU_UNIQUE)
			bind = "GNU_UNIQUE";

		if (ELF64_ST_VISIBILITY(symtab.st_other) == STV_DEFAULT)
			vis = "DEFAULT";
		else if (ELF64_ST_VISIBILITY(symtab.st_other) == STV_INTERNAL)
			vis = "INTERNAL";
		else if (ELF64_ST_VISIBILITY(symtab.st_other) == STV_HIDDEN)
			vis = "HIDDEN";
		else if (ELF64_ST_VISIBILITY(symtab.st_other) == STV_PROTECTED)
			vis = "PROTECTED";

		if (symtab.st_shndx == STN_UNDEF)
			ndx = "UND";
		else if (symtab.st_shndx == 65521)
			ndx = "ABS";
		// else if (symtab.st_shndx == )
		// 	ndx = "COM";
		else
		{
			ndx = malloc(sizeof(char) * 4);
			sprintf(ndx, "%3d", symtab.st_shndx);
		}

		printf("%6d: %016lx %5d %-7s %-6s %-8s %s %s\n", i, (unsigned long)symtab.st_value,
			(int)symtab.st_size, type, bind, vis, ndx, symtab_name);
	}

	return (0);
}

// Implementation of itoa()
char *itoa(int num, char *str, int base)
{
    int i = 0;
    int is_negative = 0;
 
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        is_negative = 1;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
 
    // If number is negative, append '-'
    if (is_negative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}

/* A utility function to reverse a string  */
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    char tmp;
    while (start < end)
    {
        tmp = *(str+start);
        *(str+start) = *(str+end);
        *(str+end) = tmp;
        start++;
        end--;
    }
}
