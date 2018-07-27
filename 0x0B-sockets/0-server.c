#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

void exit_with_error(const char *err);

/**
 * main - main function for a socket open without accepting connections
 *
 * Return: 0 on success, 1 on failure
 */
int main(void)
{
	int sock;
	short port = 12345;
	struct sockaddr_in addr_port;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		exit_with_error("socket() failed");

	addr_port.sin_family = AF_INET;
	addr_port.sin_port = htons(port);
	addr_port.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (struct sockaddr *)&addr_port, sizeof(addr_port)) < 0)
		exit_with_error("bind() failed");

	printf("Server listening on port %d\n", port);
	while (1)
	{
		if (listen(sock, 64) < 0)
			exit_with_error("listen() failed");
	}

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
