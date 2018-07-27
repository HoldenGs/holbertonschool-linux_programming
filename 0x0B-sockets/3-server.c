#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MSG_SIZE	512

void exit_with_error(const char *err);

/**
 * main - main function for a socket open without accepting connections
 *
 * Return: 0 on success, 1 on failure
 */
int main(void)
{
	char *client_name, msg[MSG_SIZE];
	int server_socket, client_socket;
	short server_port = 12345;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len;
	ssize_t message_size = 0;

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

	client_addr_len = sizeof(client_addr);
	client_socket = accept(server_socket, (struct sockaddr *)&client_addr,
		&client_addr_len);
	if (client_socket < 0)
		exit_with_error("accept() failed");
	client_name = inet_ntoa(client_addr.sin_addr);
	printf("Client connected: %s\n", client_name);

	memset(msg, '\0', MSG_SIZE);
	message_size = recv(client_socket, msg, MSG_SIZE, 0);
	if (message_size < 0)
		exit_with_error("recv() failed");
	printf("Message received: \"%s\"\n", msg);

	close(client_socket);
	close(server_socket);
	return (0);
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
