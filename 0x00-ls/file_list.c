#include "header.h"


/**
 * add_file_list - add a new node at the start of a file_list_t linked list
 *
 * @head: start of linked list
 * @file: file data to add to node
 *
 * Return: address of new node; NULL if failure
 */
file_list_t *add_file_node(file_list_t **head, const char file[256])
{
	file_list_t *node;

	node = malloc(sizeof(file_list_t));
	if (node == NULL)
		return (NULL);
	strcpy(node->file, file);
	node->next = *head;
	node->prev = NULL;
	if (*head != NULL)
		(*head)->prev = node;
	*head = node;
	return node;
}


/**
 * free_file_list - free a file_list_t linked list and set the @head to NULL
 *
 * @head: linked list start point
 *
 * Return: void
 */
void free_file_list(file_list_t **head)
{
	file_list_t *current, *tmp;

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


/**
 * insert_file_node_at_index - insert a new node in a doubly linked list
 *
 * @h: list head
 * @idx: position in list
 * @file: file data
 *
 * Return: new node; NULL if failure
 */
file_list_t *insert_file_node_at_index(file_list_t **h, unsigned int idx, const char file[256])
{
	file_list_t *walk, *new;
	unsigned int i;

	if (*h != NULL)
	{
		walk = *h;
		for (i = 0; walk != NULL; i++)
		{
			if (i == idx)
			{
				new = malloc(sizeof(file_list_t));
				if (new == NULL)
					return (NULL);
				strcpy(new->file, file);
				new->next = walk;
				new->prev = walk->prev;
				if (walk->prev != NULL)
				{
					walk = walk->prev;
					walk->next = new;
				}
				else
					*h = new;
				walk = new->next;
				walk->prev = new;
				return (new);
			}
			walk = walk->next;
		}
	}
	if (i == idx)
		return (add_node(h, walk, file));
	return NULL;
}


/**
 * add_node - adds a node to the correspond position of @w in a linked list
 *
 * @h: list head
 * @w: current node
 * @file: file data
 *
 * Return: new node; NULL if failure
 */
file_list_t *add_node(file_list_t **h, file_list_t *w, const char file[256])
{

	file_list_t *new, *walk;

	walk = w;
	new = malloc(sizeof(file_list_t));
	if (new == NULL)
		return (NULL);
	strcpy(new->file, file);
	new->next = NULL;
	if (*h != NULL)
	{
		walk = *h;
		if (walk != NULL)
			while (walk->next != NULL)
				walk = walk->next;
		walk->next = new;
		new->prev = walk;
	}
	else
	{
		*h = new;
		new->prev = NULL;
	}
	return new;
}
