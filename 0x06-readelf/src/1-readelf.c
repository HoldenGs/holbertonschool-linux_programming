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
	retval = print_section_headers(fp, &e_hdr);
	
	fclose(fp);
	return (retval);
}

int print_section_headers(FILE *fp, Elf64_Ehdr *e_hdr)
{
	uint64_t flags, flag;
	int i;
	char *names, *name, *type;
	Elf64_Shdr *s_hdr_table, s_hdr;

	s_hdr_table = malloc(e_hdr->e_shnum * sizeof(s_hdr));
	fseek(fp, e_hdr->e_shoff + i * sizeof(s_hdr), SEEK_SET);
	fread(s_hdr_table, sizeof(s_hdr), e_hdr->e_shnum, fp);

	fseek(fp, e_hdr->e_shoff + e_hdr->e_shstrndx * sizeof(s_hdr), SEEK_SET);
	fread(&s_hdr, sizeof(s_hdr), 1, fp);

	names = malloc(s_hdr.sh_size);
	fseek(fp, s_hdr.sh_offset, SEEK_SET);
	fread(names, 1, s_hdr.sh_size, fp);

	printf("There are %d headers, starting at offset 0x%x:\n\nSection Headers:\n\n",
		e_hdr->e_shnum, (unsigned int)e_hdr->e_shoff);
	puts("  [Nr] Name              Type             Address           Offset");
	puts("       Size              EntSize          Flags  Link  Info  Align");
	for (i = 0; i < e_hdr->e_shnum; i++)
	{
		name = "";
		fseek(fp, e_hdr->e_shoff + i * sizeof(s_hdr), SEEK_SET);
		fread(&s_hdr, sizeof(s_hdr), 1, fp);
		if (s_hdr.sh_name)
			name = names + s_hdr.sh_name;
		if (s_hdr.sh_type == SHT_NULL)
			type = "NULL";
		else if (s_hdr.sh_type == SHT_PROGBITS)
			type = "PROGBITS";
		else if (s_hdr.sh_type == SHT_SYMTAB)
			type = "SYMTAB";
		else if (s_hdr.sh_type == SHT_STRTAB)
			type = "STRTAB";
		else if (s_hdr.sh_type == SHT_RELA)
			type = "RELA";
		else if (s_hdr.sh_type == SHT_HASH)
			type = "HASH";
		else if (s_hdr.sh_type == SHT_DYNAMIC)
			type = "DYNAMIC";
		else if (s_hdr.sh_type == SHT_NOTE)
			type = "NOTE";
		else if (s_hdr.sh_type == SHT_NOBITS)
			type = "NOBITS";
		else if (s_hdr.sh_type == SHT_REL)
			type = "REL";
		else if (s_hdr.sh_type == SHT_SHLIB)
			type = "SHLIB";
		else if (s_hdr.sh_type == SHT_DYNSYM)
			type = "DYNSYM";
		else if (s_hdr.sh_type == SHT_INIT_ARRAY)
			type = "INIT_ARRAY";
		else if (s_hdr.sh_type == SHT_FINI_ARRAY)
			type = "FINI_ARRAY";
		else if (s_hdr.sh_type == SHT_GROUP)
			type = "GROUP";
		else if (s_hdr.sh_type == SHT_SYMTAB_SHNDX)
			type = "SYMTAB_SHNDX";
		else if (s_hdr.sh_type == SHT_GNU_ATTRIBUTES)
			type = "GNU_ATTRIBUTES";
		else if (s_hdr.sh_type == SHT_GNU_HASH)
			type = "GNU_HASH";
		else if (s_hdr.sh_type == SHT_GNU_LIBLIST)
			type = "GNU_LIBLIST";
		else if (s_hdr.sh_type == SHT_CHECKSUM)
			type = "CHECKSUM";
		else if (s_hdr.sh_type == SHT_GNU_verdef)
			type = "VERDEF";
		else if (s_hdr.sh_type == SHT_GNU_verneed)
			type = "VERNEED";
		else if (s_hdr.sh_type == SHT_GNU_versym)
			type = "VERSYM";

		char flag_buf[16], *ptr = flag_buf;
		for (flags = s_hdr.sh_flags; flags; ptr++)
		{
			flag = flags & -flags; // take next bit from flags
			flags &= ~flag; // turn off currently used bit
			if (flag == SHF_WRITE)
				*ptr = 'W';
			else if (flag == SHF_ALLOC)
				*ptr = 'A';
			else if (flag == SHF_EXECINSTR)
				*ptr = 'X';
			else if (flag == SHF_MERGE)
				*ptr = 'M';
			else if (flag == SHF_STRINGS)
				*ptr = 'S';
			else if (flag == SHF_INFO_LINK)
				*ptr = 'I';
			else if (flag == SHF_LINK_ORDER)
				*ptr = 'L';
			else if (flag == SHF_OS_NONCONFORMING)
				*ptr = 'O';
			else if (flag == SHF_GROUP)
				*ptr = 'G';
			else if (flag == SHF_TLS)
				*ptr = 'T';
			else if (flag == SHF_EXCLUDE)
				*ptr = 'E';
			else if (flag & SHF_MASKOS)
				*ptr = 'o';
			else if (flag & SHF_MASKPROC)
				*ptr = 'p';
		}
		*ptr = '\0';
		printf("  [%2d] %-18s %-15s %016d  %08d\n       %016d %016d %4s   %5d %5d     %-5d\n",
			i, name, type, (int)s_hdr.sh_addr, (int)s_hdr.sh_offset,
			(int)s_hdr.sh_size, (int)s_hdr.sh_entsize, flag_buf,
			(int)s_hdr.sh_link, (int)s_hdr.sh_info, (int)s_hdr.sh_addralign);
	}
	printf("Key to Flags:\n\
  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)\n\
  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n\
  O (extra OS processing required) o (OS specific), p (processor specific)\n");
	free(names);
	return (0);
}

