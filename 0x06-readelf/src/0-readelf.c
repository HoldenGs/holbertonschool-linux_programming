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
	retval = print_elf_header(fp, &e_hdr);
	
	fclose(fp);
	return (retval);
}

int print_elf_header(FILE *fp, Elf64_Ehdr *e_hdr)
{
	char *buffer, *type, *machine, *class, *data,
	*version, *abi, *abi_version;
	int i;

	// type = get_elf_type(e_hdr);
	// machine = get_elf_machine(e_hdr);
	// version = get_elf_version(e_hdr);

	if (e_hdr->e_type == ET_NONE)
		type = "NONE (No file type)";
	else if (e_hdr->e_type == ET_REL)
		type = "REL (Relocatable file)";
	else if (e_hdr->e_type == ET_EXEC)
		type = "EXEC (Executable file)";
	else if (e_hdr->e_type == ET_DYN)
		type = "DYN (Shared object file)";
	else if (e_hdr->e_type == ET_CORE)
		type = "CORE (Core file)";
	else
		type = "Unknown";

	if (e_hdr->e_machine == EM_NONE)
		machine = "No machine";
	else if (e_hdr->e_machine == EM_M32)
		machine = "AT&T WE 32100";
	else if (e_hdr->e_machine == EM_SPARC)
		machine = "SUN SPARC";
	else if (e_hdr->e_machine == EM_386)
		machine = "Intel 80386";
	else if (e_hdr->e_machine == EM_68K)
		machine = "Motorola m68k family";
	else if (e_hdr->e_machine == EM_88K)
		machine = "Motorola m88k family";
	else if (e_hdr->e_machine == EM_860)
		machine = "Intel 80860";
	else if (e_hdr->e_machine == EM_MIPS)
		machine = "MIPS R3000 big-endian";
	else if (e_hdr->e_machine == EM_S370)
		machine = "IBM System/370";
	else if (e_hdr->e_machine == EM_MIPS_RS3_LE)
		machine = "MIPS R3000 little-endian";
	else if (e_hdr->e_machine == EM_X86_64)
		machine = "Advanced Micro Devices X86-64";
	else if (e_hdr->e_machine == EM_ARM)
		machine = "ARM";
	else
		machine = "Unknown";

	if (e_hdr->e_ident[EI_VERSION] == EV_CURRENT)
		version = "1 (current)";
	else
		version = "Invalid ELF version";

	if (e_hdr->e_ident[EI_CLASS] == ELFCLASSNONE)
		class = "Invalid class";
	else if (e_hdr->e_ident[EI_CLASS] == ELFCLASS32)
		class = "ELF32";
	else if (e_hdr->e_ident[EI_CLASS] == ELFCLASS64)
		class = "ELF64";
	else
		class = "Unknown";

	if (e_hdr->e_ident[EI_DATA] == ELFDATANONE)
		data = "Invalid data encoding";
	else if (e_hdr->e_ident[EI_DATA] == ELFDATA2LSB)
		data = "2's complement, little endian";
	else if (e_hdr->e_ident[EI_DATA] == ELFDATA2MSB)
		data = "2's complement, big endian";
	else
		data = "Unknown";

	if (e_hdr->e_ident[EI_OSABI] == ELFOSABI_NONE ||
		e_hdr->e_ident[EI_OSABI] == ELFOSABI_SYSV)
		abi = "Unix System V ABI";
	else if (e_hdr->e_ident[EI_OSABI] == ELFOSABI_HPUX)
		abi = "HP_UX";
	else if (e_hdr->e_ident[EI_OSABI] == ELFOSABI_NETBSD)
		abi = "NetBSD";
	else if (e_hdr->e_ident[EI_OSABI] == ELFOSABI_GNU ||
		e_hdr->e_ident[EI_OSABI] == ELFOSABI_LINUX)
		abi = "Object uses GNU ELF extensions";
	else if (e_hdr->e_ident[EI_OSABI] == ELFOSABI_SOLARIS)
		abi = "Sun Solaris";
	else if (e_hdr->e_ident[EI_OSABI] == ELFOSABI_AIX)
		abi = "IBM AIX";
	else if (e_hdr->e_ident[EI_OSABI] == ELFOSABI_IRIX)
		abi = "SGI Irix";
	else if (e_hdr->e_ident[EI_OSABI] == ELFOSABI_FREEBSD)
		abi = "FreeBSD";
	else if (e_hdr->e_ident[EI_OSABI] == ELFOSABI_TRU64)
		abi = "Compaq TRU64 UNIX";
	else if (e_hdr->e_ident[EI_OSABI] == ELFOSABI_MODESTO)
		abi = "Novell Modesto";
	else if (e_hdr->e_ident[EI_OSABI] == ELFOSABI_OPENBSD)
		abi = "OpenBSD";
	else if (e_hdr->e_ident[EI_OSABI] == ELFOSABI_ARM_AEABI)
		abi = "ARM EABI";
	else if (e_hdr->e_ident[EI_OSABI] == ELFOSABI_ARM)
		abi = "ARM";
	else if (e_hdr->e_ident[EI_OSABI] == ELFOSABI_STANDALONE)
		abi = "Standalone (embedded) application";


	printf("ELF HEADER:\nMagic: ");
	for (i = 0; i < 16; i++)
	{
		printf("%02x", e_hdr->e_ident[i]);
		(i == 15) ? printf("\n") : printf(" ");
	}
	printf("  Class:                             %s\n", class);
	printf("  Data:                              %s\n", data);
	printf("  Version:                           %s\n", version);
	printf("  OS/ABI:                            %s\n", abi);
	printf("  ABI Version:                       %d\n", e_hdr->e_ident[EI_ABIVERSION]);
	printf("  Type:                              %s\n", type);
	printf("  Machine:                           %s\n", machine);
	printf("  Version:                           0x%x\n", e_hdr->e_version);
	printf("  Entry point address:               0x%x\n", (unsigned int)e_hdr->e_entry);
	printf("  Start of program headers:          %d (bytes into file)\n", (int)e_hdr->e_phoff);
	printf("  Start of section headers:          %d (bytes into file)\n", (int)e_hdr->e_shoff);
	printf("  Flags:                             %x\n", e_hdr->e_flags);
	printf("  Size of this header:               %d (bytes)\n", e_hdr->e_ehsize);
	printf("  Size of program headers:           %d (bytes)\n", e_hdr->e_phentsize);
	printf("  Number of program headers:         %d\n", e_hdr->e_phnum);
	printf("  Size of section headers:           %d (bytes)\n", e_hdr->e_shentsize);
	printf("  Number of section headers:         %d\n", e_hdr->e_shnum);
	printf("  Section header string table index: %d\n", e_hdr->e_shstrndx);

	return (0);
}
