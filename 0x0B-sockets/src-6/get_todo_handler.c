#include "todo_api.h"

/**
 * get_todo - retrieve a single todo and send it in the response
 *
 * @client_socket: client socket to send information back to
 * @todo_list: todo list to search through
 * @id: ID of todo item we're searching for
 *
 * Return: 0 if success, else HTTP error code
 */
int get_todo(int client_socket, list_t *todo_list, int id)
{
	char *ok_msg;
	int clen, l1, l2, l3, l4;
	list_t *walk;

	if (id == -1)
		return (404);
	for (walk = todo_list; walk != NULL; walk = walk->next)
	{
		if (walk->id == id)
			break;
	}
	if (walk == NULL)
		return (404);
	l1 = strlen("{\"id\":,\"title\":\"\",\"description\":\"\"}");
	l2 = int_len(walk->id);
	l3 = strlen(walk->title);
	l4 = strlen(walk->description);
	clen = l1 + l2 + l3 + l4;
	ok_msg = malloc(clen + 72);
	if (ok_msg == NULL)
		exit_with_error("malloc() failed");
	sprintf(
		ok_msg,
		"HTTP/1.1 200 OK\r\n"
		"Content-Length: %d\r\n"
		"Content-Type: application/json\r\n\r\n"
		"{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
		clen, walk->id, walk->title, walk->description
	);
	printf("200 OK\n");
	if (send(client_socket, ok_msg, strlen(ok_msg), 0) < 0)
		exit_with_error("send() failed");
}

/**
 * get_id - get the id in the request route if one exists
 *
 * @msg: raw request message
 *
 * Return: id of message, or -1 on failure
 */
int get_id(char *msg)
{
	int i, id;
	char *id_start = msg + 14, *id_str;

	for (i = 0; id_start[i] != ' '; i++)
		if (id_start[i] < 48 || id_start[i] > 57)		
			return (-1);
	id_str = malloc(i + 1);
	sprintf(id_str, "%.*s", i, id_start);
	id = atoi(id_str);
	free(id_str);
	return (id);
}
