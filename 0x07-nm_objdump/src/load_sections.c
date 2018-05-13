#include "../inc/readelf.h"

/**
 * load_sections - load all sections into @elf
 *
 * @elf: elf data structure
 */
void load_sections(Elf_t *elf)
{
	unsigned int i;
	char *names;
	Elf_Shdr_t *header;
	uint64_t (*convert_endian)(size_t, int);

	elf->sections = malloc(elf->header->e_shnum * sizeof(Section_t));
	if (elf->sections == NULL)
	{
		fprintf(stderr, "Failed to malloc space for sections\n");
		exit(errno);
	}
	for (i = 0; i < elf->header->e_shnum; i++)
	{
		elf->sections[i].header = convert_section_header_values(elf, i);
		elf->sections[i].data = elf->data + elf->sections[i].header.sh_offset;
		if (i == elf->header->e_shstrndx)
			names = elf->data + elf->sections[i].header.sh_offset;
	}
	for (i = 0; i < elf->header->e_shnum; i++)
		elf->sections[i].name = names + elf->sections[i].header.sh_name;
}


/**
 * convert_section_header_values - convert section header values
 * to a universal format and add it to @elf
 *
 * @elf: elf data structure
 * @i: index of section header
 *
 * Return: our custom section struct
 */
Elf_Big_Shdr_t convert_section_header_values(Elf_t *elf, int i)
{
	Elf_Big_Shdr_t section_header;
	Elf_Shdr_t *header;
	uint64_t (*convert_endian)(size_t, int);

	convert_endian = (elf->ident[EI_DATA] == ELFDATA2MSB) ?
		convert_big_endian : convert_little_endian;
	header = (Elf_Shdr_t *)(elf->data + elf->header->e_shoff +
		elf->header->e_shentsize * i);
	if (IS_CLASS_64(elf))
	{
		section_header.sh_name = CONVERT_ENDIAN(header->Elf64.sh_name);
		section_header.sh_type = CONVERT_ENDIAN(header->Elf64.sh_type);
		section_header.sh_flags = CONVERT_ENDIAN(header->Elf64.sh_flags);
		section_header.sh_addr = CONVERT_ENDIAN(header->Elf64.sh_addr);
		section_header.sh_offset = CONVERT_ENDIAN(header->Elf64.sh_offset);
		section_header.sh_size = CONVERT_ENDIAN(header->Elf64.sh_size);
		section_header.sh_link = CONVERT_ENDIAN(header->Elf64.sh_link);
		section_header.sh_info = CONVERT_ENDIAN(header->Elf64.sh_info);
		section_header.sh_addralign = CONVERT_ENDIAN(header->Elf64.sh_addralign);
		section_header.sh_entsize = CONVERT_ENDIAN(header->Elf64.sh_entsize);
	}
	else
	{
		section_header.sh_name = CONVERT_ENDIAN(header->Elf64.sh_name);
		section_header.sh_type = CONVERT_ENDIAN(header->Elf64.sh_type);
		section_header.sh_flags = CONVERT_ENDIAN(header->Elf64.sh_flags);
		section_header.sh_addr = CONVERT_ENDIAN(header->Elf64.sh_addr);
		section_header.sh_offset = CONVERT_ENDIAN(header->Elf64.sh_offset);
		section_header.sh_size = CONVERT_ENDIAN(header->Elf64.sh_size);
		section_header.sh_link = CONVERT_ENDIAN(header->Elf64.sh_link);
		section_header.sh_info = CONVERT_ENDIAN(header->Elf64.sh_info);
		section_header.sh_addralign = CONVERT_ENDIAN(header->Elf64.sh_addralign);
		section_header.sh_entsize = CONVERT_ENDIAN(header->Elf64.sh_entsize);
	}

	return (section_header);
}
