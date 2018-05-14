#ifndef HEADERS_H
#define HEADERS_H

#include <math.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <elf.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


# define _S						", "
# define _F(f, t, r)			printf("%s%s", r && (f & t) ? _S : "", f & t ? #t : "")

#define LOG_BASE_16 			1 / log(16)
#define IS_CLASS_64(elf) 		(elf->ident[EI_CLASS] == ELFCLASS64)

#define CONVERT_ENDIAN(field) 	convert_endian(field, sizeof(field))

/**
 * struct Elf_Big_Ehdr_s - universal elf header struct to hold both 32
 * and 64 bit values
 *
 * @e_ident: Magic number and other info
 * @e_type: Object file type
 * @e_machine: Architecture
 * @e_version: Object file version
 * @e_entry: Entry point virtual address
 * @e_phoff: Program header table file offset
 * @e_shoff: Section header table file offset
 * @e_flags: Processor-specific flags
 * @e_ehsize: ELF header size in bytes
 * @e_phentsize: Program header table entry size
 * @e_phnum: Program header table entry count
 * @e_shentsize: Section header table entry size
 * @e_shnum: Section header table entry count
 * @e_shstrndx: Section header string table index
 */
typedef struct Elf_Big_Ehdr_s
{
	unsigned char e_ident[EI_NIDENT];
	uint64_t	e_type;
	uint64_t	e_machine;
	uint64_t	e_version;
	uint64_t	e_entry;
	uint64_t	e_phoff;
	uint64_t	e_shoff;
	uint64_t	e_flags;
	uint64_t	e_ehsize;
	uint64_t	e_phentsize;
	uint64_t	e_phnum;
	uint64_t	e_shentsize;
	uint64_t	e_shnum;
	uint64_t	e_shstrndx;
} Elf_Big_Ehdr_t;

/**
 * struct Elf_Big_Shdr_s - universal elf section header struct to hold both 32
 * and 64 bit values
 *
 * @sh_name: section name (string tbl index)
 * @sh_type: Section type
 * @sh_flags: Section flags
 * @sh_addr: Section virtual addr at execution
 * @sh_offset: Section file offset
 * @sh_size: Section size in bytes
 * @sh_link: Link to another section, usually to a string table
 * @sh_info: Additional section information
 * @sh_addralign: Section alignment
 * @sh_entsize: Entry size if section holds table
 */
typedef struct Elf_Big_Shdr_s
{
	uint64_t	sh_name;
	uint64_t	sh_type;
	uint64_t	sh_flags;
	uint64_t	sh_addr;
	uint64_t	sh_offset;
	uint64_t	sh_size;
	uint64_t	sh_link;
	uint64_t	sh_info;
	uint64_t	sh_addralign;
	uint64_t	sh_entsize;
} Elf_Big_Shdr_t;

/**
 * struct Elf_Big_Sym_s - universal elf symbol struct to hold both 32
 * and 64 bit values
 *
 * @st_name: Symbol name (string tbl index)
 * @st_info: Symbol type and binding
 * @st_other: Symbol visibility
 * @st_shndx: Section index
 * @st_value: Symbol value
 * @st_size: Symbol size
 */
typedef struct Elf_Big_Sym_s
{
	uint64_t	st_name;
	uint64_t	st_info;
	uint64_t	st_other;
	uint64_t	st_shndx;
	uint64_t	st_value;
	uint64_t	st_size;
} Elf_Big_Sym_t;

/**
 * union Elf_Ehdr_u - union that holds either a 32 or 64 bit elf header
 *
 * @Elf32: 32 bit elf header
 * @Elf64: 64 bit elf header
 */
typedef union Elf_Ehdr_u
{
	Elf32_Ehdr Elf32;
	Elf64_Ehdr Elf64;
} Elf_Ehdr_t;

/**
 * union Elf_Shdr_u - union that holds either a 32 or 64 bit section header
 *
 * @Elf32: 32 bit section header
 * @Elf64: 64 bit section header
 */
typedef union Elf_Shdr_u
{
	Elf32_Shdr Elf32;
	Elf64_Shdr Elf64;
} Elf_Shdr_t;

/**
 * union Elf_Sym_u - union that holds either a 32 or 64 bit symbol
 *
 * @Elf32: 32 bit symbol
 * @Elf64: 64 bit symbol
 */
typedef union Elf_Sym_u
{
	Elf32_Sym Elf32;
	Elf64_Sym Elf64;
} Elf_Sym_t;

/**
 * struct Sym_s - Symbol struct abstraction,
 * holds symbol name and symbol struct
 *
 * @name: name of symbol
 * @sym: symbol structure
 */
typedef struct Sym_s
{
	char *name;
	Elf_Big_Sym_t sym;
} Sym_t;

/**
 * struct Section_s - Section header struct abstraction,
 * holds section name, section struct, and its data
 *
 * @name: name of section
 * @header: section header
 * @data: data of section
 */
typedef struct Section_s
{
	char *name;
	Elf_Big_Shdr_t header;
	unsigned char *data;
} Section_t;

/**
 * struct Elf_s - Elf header struct abstraction,
 * holds file name, size of file, and all of the file information
 *
 * @filename: name of file
 * @size: size of file
 * @data: raw bytes of file
 * @ident: magic header
 * @header: ELF header
 * @sections: array of sections
 */
typedef struct Elf_s
{
	char *filename;
	ssize_t size;
	unsigned char *data;
	unsigned char *ident;
	Elf_Big_Ehdr_t *header;
	Section_t *sections;
} Elf_t;


int nm(char *file);
int hobjdump(char *file);
int load_elf_contents(Elf_t *elf, char *file);
void load_elf_header(Elf_t *elf);
void load_sections(Elf_t *elf);
Elf_Big_Shdr_t convert_section_header_values(Elf_t *elf, int i);
void convert_elf_header_values(Elf_t *elf);
int check_elf_file(unsigned char *mag);
uint64_t convert_big_endian(uint64_t data, int size);
uint64_t convert_little_endian(uint64_t data, int size);
int free_elf(Elf_t *elf);
void *safe_malloc(size_t size);
Sym_t *load_symbols(Elf_t *elf);
Elf_Big_Sym_t convert_symbol_values(Elf_t *elf, Section_t *symtab, int i);
Section_t *get_section_by_name(Elf_t *elf, char *name);
void print_symbols(Elf_t *elf, Sym_t *symbols);
void print_sections(Elf_t *elf);
void print_section(Elf_t *elf, int i);

int hex_len(uint64_t n);
void print_hex(unsigned char *data, uint64_t size, int offset);
void print_ascii(unsigned char *data, uint64_t size, int offset);

char get_symbol_letter(Elf_t *elf, Elf_Big_Sym_t sym);
char letter_by_section(Elf_t *elf, Elf_Big_Sym_t sym, unsigned char bind);

#endif
