#include "todo_api_4.h"

#define MSG_SIZE			1028

int id_count = 0;

/**
 * main - main function for a socket open without accepting connections
 *
 * Return: 0 on success, 1 on failure
 */
int main(void)
{
	char *client_name, msg[MSG_SIZE];
	int server_socket, client_socket, r, v = 1;
	short server_port = 8080;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len;
	list_t *todo_list = NULL;

	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
		exit_with_error("socket() failed");
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(v)) < 0)
		exit_with_error("setsockopt() failed");
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(server_socket, (struct sockaddr *)&server_addr,
		sizeof(server_addr)) < 0)
		exit_with_error("bind() failed");

	if (listen(server_socket, 64) < 0)
		exit_with_error("listen() failed");
	printf("Server listening on port %d\n", server_port);

	while (1)
	{
		client_addr_len = sizeof(client_addr);
		client_socket = accept(server_socket, (struct sockaddr *)&client_addr,
			&client_addr_len);
		if (client_socket < 0)
			exit_with_error("accept() failed");
		client_name = inet_ntoa(client_addr.sin_addr);
		printf("Client connected: %s\n", client_name);

		todo_list = handle_client(client_socket, todo_list);
		close(client_socket);
	}

	close(server_socket);
	return (0);
}

/**
 * handle_client - recieve messages from a client and handle the data
 *
 * @client_socket: client socket
 * @todo_list: linked list of todo's
 */
list_t *handle_client(int client_socket, list_t *todo_list)
{
	char msg[MSG_SIZE];
	int error;
	ssize_t message_size = 0;
	err_and_list_t err_and_todo;
	list_t *tmp;

	memset(msg, '\0', MSG_SIZE);
	message_size = recv(client_socket, msg, MSG_SIZE, 0);
	if (message_size < 0)
		exit_with_error("recv() failed");
	printf("Raw request: \"%s\"\n", msg);

	if (!strncmp(msg, "POST /todos", 11))
	{
		err_and_todo = get_todo_post_params(msg, message_size, todo_list);
		todo_list = err_and_todo.head;
		if (todo_list->description == NULL || todo_list->title == NULL)
			err_and_todo.error = 422;
	}
	else
		err_and_todo.error = 404;
	if (err_and_todo.error)
	{
		if (err_and_todo.error != 404 && err_and_todo.error != 411)
		{
			tmp = err_and_todo.head;
			err_and_todo.head = (err_and_todo.head)->next;
			free(tmp);
			id_count--;
		}
		process_error(err_and_todo.error, client_socket);
	}
	else
	{
		send_data_to_client(todo_list, client_socket);
	}
	return (todo_list);
}

/**
 * send_data_to_client - send the client back the proper response
 *
 * @todo_list: todo list holding (possibly) newly created data
 * @client_socket: socket fd of client
 */
void send_data_to_client(list_t *todo_list, int client_socket)
{
	char *ok_msg;
	int clen;

	clen = strlen("{\"id\":,\"title\":\"\",\"description\":\"\"}") + \
		int_len(todo_list->id) + \
		strlen(todo_list->title) + \
		strlen(todo_list->description);
	ok_msg = malloc(clen + 77);
	sprintf(
		ok_msg,
		"HTTP/1.1 201 Created\r\n\
Content-Length: %d\r\n\
Content-Type: application/json\r\n\
\r\n\
{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
		clen, todo_list->id, todo_list->title, todo_list->description
	);
	if (send(client_socket, ok_msg, strlen(ok_msg), 0) < 0)
		exit_with_error("send() failed");
}

/**
 * int_len - count the number of digits in an integer
 *
 * @num: number to measure
 *
 * Return: length of integer
 */
int int_len(int num)
{
	int count = 0;

	if (num == 0)
		return (1);
	while (num != 0)
	{
		num /= 10;
		count++;
	}
	return (count);
}

/**
 * process_error - send back a message to the client for the corresponding
 * error
 *
 * @error: HTTP error code
 * @client_socket: client socket to send information to
 */
void process_error(int error, int client_socket)
{
	if (error == 422)
	{
		char msg[] = "HTTP/1.1 422 Unprocessable Entity\r\n\r\n";
		if (send(client_socket, msg, strlen(msg), 0) < 0)
			exit_with_error("send() failed");
	}
	else if (error == 411)
	{
		char msg[] = "HTTP/1.1 411 Length Required\r\n\r\n";
		if (send(client_socket, msg, strlen(msg), 0) < 0)
			exit_with_error("send() failed");
	}
	else if (error == 404)
	{
		char msg[] = "HTTP/1.1 404 Not found\r\n\r\n";
		if (send(client_socket, msg, strlen(msg), 0) < 0)
			exit_with_error("send() failed");
	}
}

/**
 * get_todo_post_params - retrieve a new todo post and put it in the todo list
 *
 * @msg: full recieved message from the client
 * @msg_len: length of message received
 * @todo_list: todo list
 */
err_and_list_t get_todo_post_params(char *msg, ssize_t msg_len, list_t *todo_list)
{
	int i, j, print_start, header_found = 0;
	char *key, *value;
	err_and_list_t err_and_todo;

	todo_list = initialize_todo(todo_list);
	err_and_todo.head = todo_list;
	err_and_todo.error = 0;
	if (check_for_header(msg, "Content-Length") < 0)
	{
		err_and_todo.error = 411;
		return (err_and_todo);
	}
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
				todo_list->title = value;
			if (!strcmp("description", key))
				todo_list->description = value;
			print_start = i + 1;
		}
	}
	value = malloc(i - print_start + 1);
	sprintf(value, "%.*s", i - print_start, msg + print_start);
	if (!strcmp("title", key))
		todo_list->title = value;
	if (!strcmp("description", key))
		todo_list->description = value;
	return (err_and_todo);
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

	for (i = 0; msg[i] != '\n'; i++);
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
 * initialize_todo - creates a new todo node
 *
 * Return: new todo node
 */
list_t *initialize_todo(list_t *todo_list)
{
	int i = 0;
	list_t *new_todo = NULL;

	new_todo = malloc(sizeof(list_t));
	if (new_todo == NULL)
		exit_with_error("malloc() failed");
	new_todo->title = NULL;
	new_todo->description = NULL;
	new_todo->next = todo_list;
	new_todo->id = id_count;
	id_count++;
	todo_list = new_todo;

	return (new_todo);
}

/**
 * exit_with_error - exit the program with a custom error message
 *
 * @err: message to use
 */
void exit_with_error(const char *err)
{
	fprintf(stderr, "%s\n", err);
	exit(1);
}
