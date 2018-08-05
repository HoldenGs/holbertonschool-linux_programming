
#ifndef _TODO_API_3_
#define _TODO_API_3_

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
void print_body_params(char *msg, ssize_t msg_len);
void print_query_params(char *msg);

#endif /* TODO_API_3_ */
