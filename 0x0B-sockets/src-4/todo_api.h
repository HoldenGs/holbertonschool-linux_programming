
#ifndef _TODO_API_4_
#define _TODO_API_4_

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MSG_SIZE			1028

/**
 * struct list_s - list type for holding todo list info
 *
 * @prev: previous node in list
 * @next: next node in list
 * @title: title of todo node
 * @description: 
 */
typedef struct list_s
{
	struct list_s *next;
	char *title;
	char *description;
	int id;
} list_t;

void exit_with_error(const char *err);
void handle_client(int client_socket, list_t **todo_list, int *id_count);
int post_todo(int client_socket, char *msg,
ssize_t msg_len, list_t **todo_list, int *id_count);
void find_parameters(char *msg, ssize_t msg_len, list_t *new_todo);
list_t *initialize_todo(int *id_count);
void add_todo_node(list_t *new_node, list_t **head, int *id_count);
void send_data_to_client(list_t *todo_list, int client_socket);
int check_for_header(char *msg, const char *header);
void process_error(int error, int client_socket);

#endif /* TODO_API_4_ */
