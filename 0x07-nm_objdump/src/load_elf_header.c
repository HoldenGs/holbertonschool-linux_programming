#include "../inc/readelf.h"


/**
 * load_elf_header - load the elf header file
 *
 * @elf: elf data structure
 */
void load_elf_header(Elf_t *elf)
{
	elf->ident = elf->data;
	if (check_elf_file(elf->ident) == -1)
	{
		fprintf(stderr, "%s is not an ELF file\n", elf->filename);
		exit(-1);
	}
	elf->header = safe_malloc(sizeof(Elf_Big_Ehdr_t));
	convert_elf_header_values(elf);
}


/**
 * convert_elf_header_values - convert elf header values to a universal
 * format
 *
 * @elf: elf data structure
 */
void convert_elf_header_values(Elf_t *elf)
{
	Elf_Ehdr_t *header;
	uint64_t (*convert_endian)(size_t, int);

	convert_endian = (elf->ident[EI_DATA] == ELFDATA2MSB) ?
		convert_big_endian : convert_little_endian;
	header = (Elf_Ehdr_t *)elf->data;
	if (IS_CLASS_64(elf))
	{
		elf->header->e_type = CONVERT_ENDIAN(header->Elf64.e_type);
		elf->header->e_machine = CONVERT_ENDIAN(header->Elf64.e_machine);
		elf->header->e_version = CONVERT_ENDIAN(header->Elf64.e_version);
		elf->header->e_entry = CONVERT_ENDIAN(header->Elf64.e_entry);
		elf->header->e_phoff = CONVERT_ENDIAN(header->Elf64.e_shoff);
		elf->header->e_shoff = CONVERT_ENDIAN(header->Elf64.e_shoff);
		elf->header->e_flags = CONVERT_ENDIAN(header->Elf64.e_flags);
		elf->header->e_ehsize = CONVERT_ENDIAN(header->Elf64.e_ehsize);
		elf->header->e_phentsize = CONVERT_ENDIAN(header->Elf64.e_phentsize);
		elf->header->e_phnum = CONVERT_ENDIAN(header->Elf64.e_phnum);
		elf->header->e_shentsize = CONVERT_ENDIAN(header->Elf64.e_shentsize);
		elf->header->e_shnum = CONVERT_ENDIAN(header->Elf64.e_shnum);
		elf->header->e_shstrndx = CONVERT_ENDIAN(header->Elf64.e_shstrndx);
	}
	else
	{
		elf->header->e_type = CONVERT_ENDIAN(header->Elf32.e_type);
		elf->header->e_machine = CONVERT_ENDIAN(header->Elf32.e_machine);
		elf->header->e_version = CONVERT_ENDIAN(header->Elf32.e_version);
		elf->header->e_entry = CONVERT_ENDIAN(header->Elf32.e_entry);
		elf->header->e_phoff = CONVERT_ENDIAN(header->Elf32.e_shoff);
		elf->header->e_shoff = CONVERT_ENDIAN(header->Elf32.e_shoff);
		elf->header->e_flags = CONVERT_ENDIAN(header->Elf32.e_flags);
		elf->header->e_ehsize = CONVERT_ENDIAN(header->Elf32.e_ehsize);
		elf->header->e_phentsize = CONVERT_ENDIAN(header->Elf32.e_phentsize);
		elf->header->e_phnum = CONVERT_ENDIAN(header->Elf32.e_phnum);
		elf->header->e_shentsize = CONVERT_ENDIAN(header->Elf32.e_shentsize);
		elf->header->e_shnum = CONVERT_ENDIAN(header->Elf32.e_shnum);
		elf->header->e_shstrndx = CONVERT_ENDIAN(header->Elf32.e_shstrndx);
	}
}
