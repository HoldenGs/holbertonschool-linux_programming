#include "header.h"


/**
 * append_dir_list - add a new node at the start of a dir_list_t linked list
 *
 * @head: start of linked list
 * @dir: directory data to add to node
 *
 * Return: address of new node; NULL if failure
 */
dir_list_t *add_dir_node(dir_list_t **head, char dir[400])
{
	dir_list_t *node;

	node = malloc(sizeof(dir_list_t));
	if (node == NULL)
		return (NULL);
	strncpy(node->dir, dir, 400);
	node->next = *head;
	*head = node;
	return (node);
}


/**
 * free_dir_list - free a dir_list_t linked list and set the @head to NULL
 *
 * @head: linked list start point
 *
 * Return: void
 */
void free_dir_list(dir_list_t **head)
{
	dir_list_t *current, *tmp;

	if (head == NULL)
		return;
	current = *head;
	while (current != NULL)
	{
		tmp = current;
		current = current->next;
		free(tmp);
	}
	*head = NULL;
}
