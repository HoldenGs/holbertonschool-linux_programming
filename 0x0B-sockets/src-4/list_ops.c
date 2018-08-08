#include "todo_api.h"

/**
 * add_todo_node - add a new node to the todo list
 *
 * @new_node: new node to add
 * @head: pointer to the head of the todo list
 * @id_count: pointer to the id iterator
 */
void add_todo_node(list_t *new_node, list_t **head, int *id_count)
{
	list_t *walk;

	(*id_count)++;
	walk = *head;
	while (walk != NULL && walk->next != NULL)
		walk = walk->next;
	if (walk)
		walk->next = new_node;
	else
		*head = new_node;
}

/**
 * initialize_todo - creates a new todo node
 * 
 * @id_count: pointer to id iterator
 *
 * Return: new todo node
 */
list_t *initialize_todo(int *id_count)
{
	int i = 0;
	list_t *new_todo = NULL, *walk;

	new_todo = malloc(sizeof(list_t));
	if (new_todo == NULL)
		exit_with_error("malloc() failed");
	new_todo->title = NULL;
	new_todo->description = NULL;
	new_todo->next = NULL;
	new_todo->id = *id_count;

	return (new_todo);
}

