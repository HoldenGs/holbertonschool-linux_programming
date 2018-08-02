
#ifndef _TODO_API_0_
#define _TODO_API_0_

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void exit_with_error(const char *err);
void handle_client(int client_socket);

#endif /* TODO_API_0_ */
