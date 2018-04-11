#include "nm.h"

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
	print_sections(fp, &e_hdr);
	

	fclose(fp);
	return (0);
}
