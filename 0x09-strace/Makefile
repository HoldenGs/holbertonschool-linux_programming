
0_OBJECTS	=	strace_0.c \
				strace.h

0_OUTPUT	=	strace_0

$(0_OUTPUT) : $(0_OBJECTS)
	gcc -o $@ $^


1_OBJECTS	=	strace_1.c \
				strace.h \
				syscalls.h

1_OUTPUT	=	strace_1

$(1_OUTPUT) : $(1_OBJECTS)
	gcc -o $@ $^


2_OBJECTS	=	strace_2.c \
				strace.h \
				syscalls.h

2_OUTPUT	=	strace_2

$(2_OUTPUT) : $(2_OBJECTS)
	gcc -o $@ $^


3_OBJECTS	=	strace_3.c \
				strace.h \
				syscalls.h

3_OUTPUT	=	strace_3

$(3_OUTPUT) : $(3_OBJECTS)
	gcc -o $@ $^


4_OBJECTS	=	strace_4.c \
				print_helpers_4.c \
				strace.h \
				syscalls.h

4_OUTPUT	=	strace_4

$(4_OUTPUT) : $(4_OBJECTS)
	gcc -o $@ $^


5_OBJECTS	=	strace_5.c \
				print_helpers_5.c \
				strace.h \
				syscalls.h

5_OUTPUT	=	strace_5

$(5_OUTPUT) : $(5_OBJECTS)
	gcc -o $@ $^


6_OBJECTS	=	strace_6.c \
				print_helpers_6.c \
				print_flags_6.c \
				strace.h \
				syscalls.h

6_OUTPUT	=	strace_6

$(6_OUTPUT) : $(6_OBJECTS)
	gcc -g3 -o $@ $^


PHONEY: clean

clean: rm 0_OUTPUT