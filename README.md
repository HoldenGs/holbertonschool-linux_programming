# holbertonschool-linux_programming

This is a collection of signal handling functions that I've made as part of a Holberton project. If you're interested in signal-processing in your programs, feel free to copy from the following functions to suit your needs.

## Functions:

These are signal functions with main functions you can use to test with.

0. Registers a signal handler function by using signal()
1. Checks the registered signal for SIGINT
2. Same function as 0, but uses sigaction()
3. Same as 1, but uses sigaction()
4. Registers a handler function that prints out the process ID of who sent the signal
5. Describe the signal number given as an argument
6. Pause the process using pause() until a signal is received
7. Terminate a process, just give the program the process ID as an argument
8. Terminate a process, but in bash instead of C
9. Register a signal handler in bash
10. Test is a PID given as a argument exists or not
  100. Handles all signals possible, and prints out information about the signal
  101. Initialize a signal set
  102. Block all signals given as an integer array by putting them in a signal set and setting the signal set
  103. Use previous program to block, and then unblock the signals specified
  104. Use previous programs to block, set a signal handler, and then unblock, all possible signals

## Compiling

For all programs except ones with only a main.c, you can compile like so:
`gcc signals.h <some_functions.c> <main.c>`
