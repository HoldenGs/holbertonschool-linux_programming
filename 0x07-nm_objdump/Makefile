
# hnm program makefile

HNM_OUTPUT	=	hnm

HNM_OBJECTS	=	src/hnm_v2.c \
				src/check_elf_file.c \
				src/get_symbol_letter.c \
				src/convert_endian.c \
				src/load_elf_contents.c \
				src/load_elf_header.c \
				src/load_sections.c \
				src/load_symbols.c \
				src/print_symbols.c \
				src/free.c \
				inc/readelf.h
# make hnm
$(HNM_OUTPUT) : $(HNM_OBJECTS)
	gcc -g -o $@ $^


# hobjdump program makefile

HOBJDUMP_OUTPUT		=	hobjdump

HOBJDUMP_OBJECTS	=	src/hobjdump.c \
						src/check_elf_file.c \
						inc/readelf.h
# make hobjdump
$(HOBJDUMP_OUTPUT) : $(HOBJDUMP_OBJECTS)
	gcc -o $@ $^



# make both programs

all : $(HNM_OUTPUT) $(HOBJDUMP_OUTPUT)


# delete both programs

.PHONEY : clean

clean :
	rm -f $(HNM_OUTPUT) $(HOBJDUMP_OUTPUT)
