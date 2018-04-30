#ifndef HEADERS_H
#define HEADERS_H

#include <stdint.h>
#include <elf.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* .tbss is special.  It doesn't contribute memory space to normal
   segments and it doesn't take file space in normal segments.  */
#define ELF_SECTION_SIZE(sec_hdr, segment)				\
	   (((sec_hdr.sh_flags & SHF_TLS) == 0				\
	     || sec_hdr.sh_type != SHT_NOBITS				\
	     || segment.p_type == PT_TLS) ? sec_hdr.sh_size : 0)

/* Decide if the given sec_hdr is in the given segment.  PT_TLS segment
   contains only SHF_TLS sections.  Only PT_LOAD and PT_TLS segments
   can contain SHF_TLS sections.  */
#define ELF_IS_SECTION_IN_SEGMENT(sec_hdr, segment)					\
			  (((((sec_hdr.sh_flags & SHF_TLS) != 0)				\
				     && (segment.p_type == PT_TLS					\
					  || segment.p_type == PT_LOAD))				\
			    || ((sec_hdr.sh_flags & SHF_TLS) == 0				\
					&& segment.p_type != PT_TLS))					\
	/* Any section besides one of type SHT_NOBITS must have a file	\
			      offset within the segment.  */					\
			   && (sec_hdr.sh_type == SHT_NOBITS					\
		  || ((unsigned long) sec_hdr.sh_offset >= segment.p_offset		\
		&& (sec_hdr.sh_offset + ELF_SECTION_SIZE(sec_hdr, segment)	\
			       <= segment.p_offset + segment.p_filesz)))		\
/* SHF_ALLOC sections must have VMAs within the segment.  */		\
			   && ((sec_hdr.sh_flags & SHF_ALLOC) == 0				\
		       || (sec_hdr.sh_addr >= segment.p_vaddr				\
		&& (sec_hdr.sh_addr + ELF_SECTION_SIZE(sec_hdr, segment)	\
			       <= segment.p_vaddr + segment.p_memsz))))

int print_elf_header(FILE *, Elf64_Ehdr *);
int print_section_headers(FILE *, Elf64_Ehdr *);
int print_program_headers(FILE *, Elf64_Ehdr *);
int print_symbols(FILE *, Elf64_Ehdr *);
char *itoa(int, char *, int);
void reverse(char str[], int length);

#endif
