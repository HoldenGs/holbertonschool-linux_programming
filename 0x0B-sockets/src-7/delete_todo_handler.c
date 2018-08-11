#include "todo_api.h"

/**
 * get_todo - delete a single todo
 *
 * @client_socket: client socket to send information back to
 * @todo_list: todo list to search through
 * @id: ID of todo item we're searching for
 *
 * Return: 0 if success, else HTTP error code
 */
int delete_todo(int client_socket, list_t **todo_list, int id)
{
	char ok_msg[] = "HTTP/1.1 204 No Content\r\n\r\n";
	int clen, l1, l2, l3, l4;
	list_t *walk, *prev;

	printf("%d\n", id);
	if (id == -1)
		return (404);
	prev = *todo_list;
	for (walk = *todo_list; walk != NULL; walk = walk->next)
	{
		if (walk->id == id)
			break;
		prev = walk;
	}
	if (walk == NULL)
		return (404);
	if (walk == prev)
		*todo_list = walk->next;
	else
		prev->next = walk->next;
	free(walk->description);
	free(walk->title);
	free(walk);
	printf("204 No Content\n");
	if (send(client_socket, ok_msg, strlen(ok_msg), 0) < 0)
		exit_with_error("send() failed");
}
