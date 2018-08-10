#include "todo_api.h"

/**
 * main - main function for a socket open without accepting connections
 *
 * Return: 0 on success, 1 on failure
 */
int main(void)
{
	char *client_name, msg[MSG_SIZE];
	int server_socket, client_socket, r, id_count = 0, v = 1;
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
	fflush(NULL);
	while (1)
	{
		client_addr_len = sizeof(client_addr);
		client_socket = accept(server_socket, (struct sockaddr *)&client_addr,
			&client_addr_len);
		if (client_socket < 0)
			exit_with_error("accept() failed");
		client_name = inet_ntoa(client_addr.sin_addr);
		printf("%s ", client_name);

		handle_client(client_socket, &todo_list, &id_count);
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
 * @id_count: pointer to id iterator
 */
void handle_client(int client_socket, list_t **todo_list, int *id_count)
{
	char msg[MSG_SIZE];
	int error, i;
	ssize_t msg_size = 0;
	list_t *tmp, *walk;

	memset(msg, '\0', MSG_SIZE);
	msg_size = recv(client_socket, msg, MSG_SIZE, 0);
	if (msg_size < 0)
		exit_with_error("recv() failed");

	for (i = 0; msg[i] != ' '; i++)
		;
	i++;
	for (; msg[i] != ' '; i++)
		;
	printf("%.*s -> ", i, msg);

	if (!strncmp(msg, "POST /todos ", 12))
		error = post_todo(client_socket, msg, msg_size, todo_list, id_count);
	else if (!strncmp(msg, "GET /todos ", 11))
		error = get_todos(client_socket, *todo_list);
	else
		error = 404;
	if (error)
		process_error(error, client_socket);
	fflush(NULL);
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

		printf("422 Unprocessable Entity\n");
		if (send(client_socket, msg, strlen(msg), 0) < 0)
			exit_with_error("send() failed");
	}
	else if (error == 411)
	{
		char msg[] = "HTTP/1.1 411 Length Required\r\n\r\n";

		printf("411 Length Required\n");
		if (send(client_socket, msg, strlen(msg), 0) < 0)
			exit_with_error("send() failed");
	}
	else if (error == 404)
	{
		char msg[] = "HTTP/1.1 404 Not Found\r\n\r\n";

		printf("404 Not Found\n");
		if (send(client_socket, msg, strlen(msg), 0) < 0)
			exit_with_error("send() failed");
	}
}

/**
 * int_len - count the number of base-10 digits in an integer
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
 * exit_with_error - exit the program with a custom error message
 *
 * @err: message to use
 */
void exit_with_error(const char *err)
{
	fprintf(stderr, "%s\n", err);
	exit(1);
}
