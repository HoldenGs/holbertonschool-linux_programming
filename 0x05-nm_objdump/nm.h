#ifndef NM_H
#define NM_H

/* mmap includes */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <stdint.h>
#include <elf.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int check_elf_file(Elf64_Ehdr *);
Elf32_Shdr *elf_sheader(Elf32_Ehdr *);
Elf32_Shdr *elf_section(Elf32_Ehdr *, int);
char *elf_str_table(Elf32_Ehdr *);
char *elf_lookup_string(Elf32_Ehdr *, int);
int print_sections(FILE *, Elf64_Ehdr *);

#endif