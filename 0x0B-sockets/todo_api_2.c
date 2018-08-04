#include "todo_api_2.h"

#define MSG_SIZE	1028


/**
 * main - main function for a socket open without accepting connections
 *
 * Return: 0 on success, 1 on failure
 */
int main(void)
{
	char *client_name, msg[MSG_SIZE];
	int server_socket, client_socket;
	short server_port = 8080;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len;

	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
		exit_with_error("socket() failed");

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

		handle_client(client_socket);
		close(client_socket);
	}

	close(server_socket);
	return (0);
}

/**
 * handle_client - recieve messages from a client and handle the data
 *
 * @client_socket: client socket
 */
void handle_client(int client_socket)
{
	char msg[MSG_SIZE], ok_msg[] = "HTTP/1.1 200 OK\r\n\r\n";
	ssize_t message_size = 0;

	memset(msg, '\0', MSG_SIZE);
	message_size = recv(client_socket, msg, MSG_SIZE, 0);
	if (message_size < 0)
		exit_with_error("recv() failed");
	printf("Raw request: \"%s\"\n", msg);

	print_headers(msg);

	if (send(client_socket, ok_msg, strlen(ok_msg), 0) < 0)
		exit_with_error("send() failed");
}

/**
 * print_headers - print path and queries of the query string sent to
 * the server
 *
 * @msg: full recieved message from the client
 */
void print_headers(char *msg)
{
	int i, print_start, header_found = 0;

	for (i = 0; msg[i] != '\n'; i++);
	i++;
	print_start = i;
	while (i < strlen(msg))
	{
		if (msg[i] == ':' && !header_found)
		{
			printf("Header: \"%.*s\" ", i - print_start, msg + print_start);
			print_start = i + 2;
			header_found = 1;
		}
		if (msg[i] == '\n' && header_found)
		{
			printf("-> \"%.*s\"\n", i - print_start - 1, msg + print_start);
			print_start = i + 1;
			header_found = 0;
		}
		i++;
		if (msg[i] == '\n' && msg[i + 1] == '\n')
			break;
	}
	fflush(NULL);
}

/**
 * print_query_params - print path and queries of the query string sent to
 * the server
 *
 * @msg: full recieved message from the client
 */
void print_query_params(char *msg)
{
	int i, print_start;

	for (i = 0; msg[i] != ' '; i++);
	i++;
	print_start = i;
	while (msg[i] != ' ')
	{
		if (msg[i] == '?')
		{
			printf("Path: %.*s\n", i - print_start, msg + print_start);
			print_start = i + 1;
		}
		if (msg[i] == '=')
		{
			printf("Query: \"%.*s\" ", i - print_start, msg + print_start);
			print_start = i + 1;
		}
		if (msg[i] == '&')
		{
			printf("-> \"%.*s\"\n", i - print_start, msg + print_start);
			print_start = i + 1;
		}
		i++;
	}
	printf("-> \"%.*s\"\n", i - print_start, msg + print_start);
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
