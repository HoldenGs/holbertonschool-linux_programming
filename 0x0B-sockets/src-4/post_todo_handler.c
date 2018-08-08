#include "todo_api.h"

/**
 * get_todo_post_params - retrieve a new todo post and put it in the todo list
 *
 * @client_socket: socket file descriptor of the client
 * @msg: full recieved message from the client
 * @msg_len: length of message received
 * @todo_list: todo list
 * @id_count: pointer to id iterator
 *
 * Return: The error message and the new head todo list node
 */
int post_todo(int client_socket, char *msg,
	ssize_t msg_len, list_t **todo_list, int *id_count)
{
	int i, error, print_start, header_found = 0;
	list_t *new_todo;

	if (check_for_header(msg, "Content-Length") < 0)
		return (411);
	new_todo = initialize_todo(id_count);
	error = 0;
	find_parameters(msg, msg_len, new_todo);
	if (new_todo->description == NULL || new_todo->title == NULL)
		error = 422;
	if (error)
		free(new_todo);
	else
	{
		add_todo_node(new_todo, todo_list, id_count);
		send_data_to_client(new_todo, client_socket);
	}

	return (error);
}

/**
 * find_parameters - find the parameters in the body of the message to
 * add to the todo node
 *
 * @msg: message to find parameters within
 * @msg_len: length of @msg
 * @new_todo: new todo node to add parameters to
 */
void find_parameters(char *msg, ssize_t msg_len, list_t *new_todo)
{
	int i, print_start, header_found = 0;
	char *key, *value;

	for (i = 0; i < (int)msg_len; i++)
	{
		if (msg[i] == '\n' && msg[i + 1] == '\r' && msg[i + 2] == '\n')
		{
			i += 3;
			print_start = i;
			break;
		}
	}
	for (; i < (int)msg_len; i++)
	{
		if (msg[i] == '=')
		{
			key = malloc(i - print_start + 1);
			sprintf(key, "%.*s", i - print_start, msg + print_start);
			print_start = i + 1;
		}
		if (msg[i] == '&')
		{
			value = malloc(i - print_start + 1);
			sprintf(value, "%.*s", i - print_start, msg + print_start);
			if (!strcmp("title", key))
				new_todo->title = value;
			if (!strcmp("description", key))
				new_todo->description = value;
			print_start = i + 1;
		}
	}
	value = malloc(i - print_start + 1);
	sprintf(value, "%.*s", i - print_start, msg + print_start);
	if (!strcmp("title", key))
		new_todo->title = value;
	if (!strcmp("description", key))
		new_todo->description = value;
}

/**
 * check_for_header - check if a given @header exists in the request
 *
 * @msg: request data
 * @header: header to look for
 *
 * Return: -1 if not found or empty value, otherwise 0
 */
int check_for_header(char *msg, const char *header)
{
	int i, print_start, header_found = 0, content_length_found = 0;
	char *key, *value;

	for (i = 0; msg[i] != '\n'; i++)
		;
	i++;
	print_start = i;
	while (i < strlen(msg))
	{
		if (msg[i] == ':' && !header_found)
		{
			key = malloc(i - print_start + 1);
			sprintf(key, "%.*s", i - print_start, msg + print_start);
			if (!strcmp(key, header))
				content_length_found = 1;
			print_start = i + 2;
			header_found = 1;
		}
		if (msg[i] == '\n' && header_found)
		{
			value = malloc(i - print_start + 1);
			sprintf(value, "%.*s", i - print_start, msg + print_start);
			if (content_length_found)
			{
				if (!atoi(value))
					return (-1);
				else
					return (0);
			}
			print_start = i + 1;
			header_found = 0;
		}
		i++;
		if (msg[i] == '\n' && msg[i + 1] == '\n')
			break;
	}
	return (-1);
}

/**
 * send_data_to_client - send the client back the the post todo response
 *
 * @todo_list: todo list holding (possibly) newly created data
 * @client_socket: socket fd of client
 */
void send_data_to_client(list_t *todo_list, int client_socket)
{
	char *ok_msg;
	int clen, l1, l2, l3, l4;

	l1 = strlen("{\"id\":,\"title\":\"\",\"description\":\"\"}");
	l2 = int_len(todo_list->id);
	l3 = strlen(todo_list->title);
	l4 = strlen(todo_list->description);
	clen = l1 + l2 + l3 + l4;
	ok_msg = malloc(clen + 77);
	if (ok_msg == NULL)
		exit_with_error("malloc() failed");
	sprintf(
		ok_msg,
		"HTTP/1.1 201 Created\r\n"
		"Content-Length: %d\r\n"
		"Content-Type: application/json\r\n\r\n"
		"{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
		clen, todo_list->id, todo_list->title, todo_list->description
	);
	printf("201 Created\n");
	if (send(client_socket, ok_msg, strlen(ok_msg), 0) < 0)
		exit_with_error("send() failed");
}
