#include "nm.h"

int print_sections(FILE *fp, Elf64_Ehdr *e_hdr)
{
	Elf64_Shdr s_hdr;
	Elf64_Sym sym;
	char info, *names, *name, *st_name, *st_names;
	int i, j, sym_entries;

	info = '\0';
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
			st_names = malloc(s_hdr.sh_size);
			fseek(fp, s_hdr.sh_offset, SEEK_SET);
			fread(st_names, 1, s_hdr.sh_size, fp);
		}


		if (!strcmp(name, ".symtab"))
		{
			fseek(fp, s_hdr.sh_offset, SEEK_SET);
			sym_entries = s_hdr.sh_size / s_hdr.sh_entsize;
			for (j = 0; j < sym_entries; j++)
			{
				fseek(fp, s_hdr.sh_offset + j * sizeof(sym), SEEK_SET);
				fread(&sym, sizeof(sym), 1, fp);
				if (sym.st_name)
				{
					st_name = st_names + sym.st_name + s_hdr.sh_link;
					printf("%016lx %d %s\n", \
							sym.st_value, info, st_name);
				}

			}
		}

		printf("%s\n", name);
	}

	free(names);
	return (0);
}
