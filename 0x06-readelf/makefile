
OUTPUT0		=	0-hreadelf

OBJECTS0	=	src/0-readelf.c \
				src/check_elf_file.c \
				inc/readelf.h

$(OUTPUT0) : $(OBJECTS0)
	gcc -o $@ $^



OUTPUT1		=	1-hreadelf

OBJECTS1	=	src/1-readelf.c \
				src/check_elf_file.c \
				inc/readelf.h

$(OUTPUT1) : $(OBJECTS1)
	gcc -o $@ $^



OUTPUT2		=	2-hreadelf

OBJECTS2	=	src/2-readelf.c \
				src/check_elf_file.c \
				inc/readelf.h

$(OUTPUT2) : $(OBJECTS2)
	gcc -o $@ $^



OUTPUT100	=	100-hreadelf

OBJECTS100	=	src/100-readelf.c \
				src/check_elf_file.c \
				inc/readelf.h

$(OUTPUT100) : $(OBJECTS100)
	gcc -o $@ $^



all : $(OUTPUT0) $(OUTPUT1) $(OUTPUT2) $(OUTPUT100)



.PHONEY : clean

clean :
	rm -f $(OUTPUT0) $(OUTPUT1) $(OUTPUT2) $(OUTPUT100)
