
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

/**
 * struct err_and_list_s - holds the head of the new todo list and the error
 * of the function
 *
 * @head: head of the new todo_list
 * @error: error if any error was set
 */
typedef struct err_and_list_s
{
	list_t *head;
	int error;
} err_and_list_t;


void exit_with_error(const char *err);
list_t *handle_client(int client_socket, list_t *todo_list);
err_and_list_t get_todo_post_params(char *msg, ssize_t msg_len, list_t *todo_list);
list_t *initialize_todo(list_t *todo_list);
void send_data_to_client(list_t *todo_list, int client_socket);
int check_for_header(char *msg, const char *header);
void process_error(int error, int client_socket);

#endif /* TODO_API_4_ */
