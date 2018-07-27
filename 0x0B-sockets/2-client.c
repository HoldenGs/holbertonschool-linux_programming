#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void exit_with_error(const char *err);

/**
 * main - connect to a server and port specified by the first two args
 *
 * @ac: argument count
 * @av: argument vector
 *
 * Return: 0 on success, 1 on failure
 */
int main(int ac, char **av)
{
	int client_socket;
	short port;
	char *host;
	struct sockaddr_in server_addr;

	if (ac != 3)
		exit_with_error("Usage: %s <host> <port>");

	port = atoi(av[2]);
	if (!strcmp(av[1], "localhost"))
		host = "127.0.0.1";
	else
		host = av[1];
	client_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (client_socket < 0)
		exit_with_error("socket() failed");

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	if (inet_aton(host, &server_addr.sin_addr) == 0)
		exit_with_error("inet_aton() failed");
	printf("host: %d\n", server_addr.sin_addr.s_addr);
	if (connect(
		client_socket,
		(struct sockaddr *)&server_addr,
		sizeof(server_addr)
		) < 0)
		exit_with_error("connect() failed");

	printf("Connected to %s:%d\n", av[1], port);

	return (EXIT_SUCCESS);
}

/**
 * exit_with_error - exit the program with a custom error message
 *
 * @err: message to use
 */
void exit_with_error(const char *err)
{
	fprintf(stderr, "%s\n", err);
	exit(EXIT_FAILURE);
}
