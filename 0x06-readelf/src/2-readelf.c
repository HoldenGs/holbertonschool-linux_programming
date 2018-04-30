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
	retval = print_program_headers(fp, &e_hdr);
	
	fclose(fp);
	return (retval);
}

int print_program_headers(FILE *fp, Elf64_Ehdr *e_hdr)
{
	int i, j, res;
	char *e_type, *type, buf[8], *interp_name, *names, *name;
	Elf64_Phdr p_hdr;
	Elf64_Shdr s_hdr;

	// Grab string table
	fseek(fp, e_hdr->e_shoff + e_hdr->e_shstrndx * sizeof(s_hdr), SEEK_SET);
	fread(&s_hdr, sizeof(s_hdr), 1, fp);
	names = malloc(s_hdr.sh_size);
	fseek(fp, s_hdr.sh_offset, SEEK_SET);
	fread(names, 1, s_hdr.sh_size, fp);

	if (e_hdr->e_type == ET_NONE)
		e_type = "NONE (No file type)";
	else if (e_hdr->e_type == ET_REL)
		e_type = "REL (Relocatable file)";
	else if (e_hdr->e_type == ET_EXEC)
		e_type = "EXEC (Executable file)";
	else if (e_hdr->e_type == ET_DYN)
		e_type = "DYN (Shared object file)";
	else if (e_hdr->e_type == ET_CORE)
		e_type = "CORE (Core file)";
	else
		e_type = "Unknown";
	printf("\nElf file type is %s\nEntry point 0x%x\n", e_type, (unsigned int)e_hdr->e_entry);
	printf("There are %d program headers, starting at offset %d\n\n",
		(int)e_hdr->e_phnum, (int)e_hdr->e_phoff);
	printf("Program Headers:\n");
	printf("  Type           Offset             VirtAddr           PhysAddr\n");
	printf("                 FileSiz            MemSiz              Flags  Align\n");

	for (i = 0; i < e_hdr->e_phnum; i++)
	{
		interp_name = "";
		fseek(fp, e_hdr->e_phoff + i * sizeof(p_hdr), SEEK_SET);
		fread(&p_hdr, sizeof(p_hdr), 1, fp);

		if (p_hdr.p_type == PT_NULL)
			type = "NULL";
		else if (p_hdr.p_type == PT_LOAD)
			type = "LOAD";
		else if (p_hdr.p_type == PT_DYNAMIC)
			type = "DYNAMIC";
		else if (p_hdr.p_type == PT_INTERP)
		{
			type = "INTERP";
			interp_name = malloc(sizeof(char) * 32);
			fseek(fp, p_hdr.p_offset, SEEK_SET);
			fread(interp_name, sizeof(char) * 32, 1, fp);
		}
		else if (p_hdr.p_type == PT_NOTE)
			type = "NOTE";
		else if (p_hdr.p_type == PT_SHLIB)
			type = "SHLIB";
		else if (p_hdr.p_type == PT_PHDR)
			type = "PHDR";
		else if (p_hdr.p_type == PT_TLS)
			type = "TLS";
		else if (p_hdr.p_type == PT_NUM)
			type = "NUM";
		else if (p_hdr.p_type == PT_GNU_EH_FRAME)
			type = "GNU_EH_FRAME";
		else if (p_hdr.p_type == PT_GNU_STACK)
			type = "GNU_STACK";
		else if (p_hdr.p_type == PT_GNU_RELRO)
			type = "GNU_RELRO";
		else if (p_hdr.p_type == PT_SUNWBSS)
			type = "SUNWBSS";
		else if (p_hdr.p_type == PT_SUNWSTACK)
			type = "SUNWSTACK";

		(p_hdr.p_flags & PF_X) ? (buf[2] = 'E') : (buf[2] = ' ');
		(p_hdr.p_flags & PF_W) ? (buf[1] = 'W') : (buf[1] = ' ');
		(p_hdr.p_flags & PF_R) ? (buf[0] = 'R') : (buf[0] = ' ');
		buf[3] = '\0';

		printf("  %-14s 0x%016x 0x%016x 0x%016x\n", type, (int)p_hdr.p_offset,
			(int)p_hdr.p_vaddr, (int)p_hdr.p_paddr);
		printf("                 0x%016x 0x%016x  %-6s  %lx\n", (int)p_hdr.p_filesz,
			(int)p_hdr.p_memsz, buf, (unsigned long)p_hdr.p_align);
		if (interp_name != "")
			printf("      [Requesting program interpreter: %s]\n", interp_name);
	}

	printf("\n Section to Segment mapping:\n  Segment Sections...\n");
	for (i = 0; i < e_hdr->e_phnum; i++)
	{
		fseek(fp, e_hdr->e_phoff + i * sizeof(p_hdr), SEEK_SET);
		fread(&p_hdr, sizeof(p_hdr), 1, fp);
		printf("   %02d     ", i);
		for (j = 0; j < e_hdr->e_shnum; j++)
		{
			name = "";
			fseek(fp, e_hdr->e_shoff + j * sizeof(s_hdr), SEEK_SET);
			fread(&s_hdr, sizeof(s_hdr), 1, fp);
			if (s_hdr.sh_name)
				name = names + s_hdr.sh_name;
			if (ELF_IS_SECTION_IN_SEGMENT(s_hdr, p_hdr) && name != "")
				printf("%s ", name);
		}
		putc('\n', stdout);
	}

	return (0);
}

