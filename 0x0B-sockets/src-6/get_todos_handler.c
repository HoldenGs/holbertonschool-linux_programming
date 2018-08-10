#include "todo_api.h"

/**
 * get_todos - get all the todos currently stored and send them to the client
 *
 * @client_socket: the client's socket
 * @todo_list: list of todos
 */
int get_todos(int client_socket, list_t *todo_list)
{
	char *ok_msg = NULL, *todos_str = NULL;
	int msg_len, todos_len = 0;
	char base_str[] = "HTTP/1.1 200 OK\r\n"
		"Content-Length: 2\r\n"
		"Content-Type: application/json\r\n\r\n[]";

	msg_len = strlen(base_str);
	todos_str = build_todos_string(todo_list, &todos_len);
	printf("200 OK\n");
	msg_len += todos_len;
	ok_msg = malloc(msg_len);
	memset(ok_msg, '\0', msg_len);
	if (todos_str)
	{
		sprintf(
			ok_msg,
			"HTTP/1.1 200 OK\r\n"
			"Content-Length: %ld\r\n"
			"Content-Type: application/json\r\n\r\n[%s]",
			strlen(todos_str) + 2, todos_str);
	}
	else
		sprintf(ok_msg, "%s", base_str);
	if (send(client_socket, ok_msg, msg_len, 0) < 0)
		exit_with_error("send() failed");
	return (0);
}

/**
 * build_todos_string - goes through @todo_list and appends each
 * to a string in json format
 *
 * @todo_list: todo_list storing all todo list information
 * @todos_len: length of the todo list string
 *
 * Return: the json formatted todo list string
 */
char *build_todos_string(list_t *todo_list, int *todos_len)
{
	char *todos_str = NULL;
	int l1, l2, l3, l4;
	list_t *walk;

	for (walk = todo_list; walk != NULL; walk = walk->next)
	{
		l1 = strlen("{\"id\":,\"title\":\"\",\"description\":\"\"}");
		l2 = int_len(walk->id);
		l3 = strlen(walk->title);
		l4 = strlen(walk->description);
		(*todos_len) += (l1 + l2 + l3 + l4);
		if (walk->next == NULL)
		{
			todos_str = realloc(todos_str, *todos_len);
			sprintf(
				todos_str + *todos_len - l1 - l2 - l3 - l4,
				"{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
				walk->id, walk->title, walk->description);
		}
		else
		{
			(*todos_len) += 1;
			todos_str = realloc(todos_str, *todos_len);
			sprintf(
				todos_str + *todos_len - l1 - l2 - l3 - l4 - 1,
				"{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"},",
				walk->id, walk->title, walk->description);
		}
	}
	if (todo_list && todo_list->next)
		(*todos_len) += 2;
	else
		(*todos_len)++;
	return (todos_str);
}
