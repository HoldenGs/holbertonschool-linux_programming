
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


PHONEY: clean

clean: rm 0_OUTPUT