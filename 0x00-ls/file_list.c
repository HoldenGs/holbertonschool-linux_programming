#include "header.h"


/**
 * add_file_node - add a new node at the start of a file_list_t linked list
 *
 * @head: start of linked list
 * @file: file data to add to node
 * @sort: integer value to sort by
 * @curr_dir: current directory for lstat usage
 *
 * Return: address of new node; NULL if failure
 */
file_list_t *add_file_node(file_list_t **head, struct dirent *file, char sort, char *curr_dir)
{
	file_list_t *node;
	struct stat *buf;
	char path[400], dir[400];

	node = malloc(sizeof(file_list_t));
	if (node == NULL)
		return (NULL);
	if (sort != ' ')
	{
		buf = malloc(sizeof(struct stat));
		strncpy(path, strncat(strcat(strcpy(dir, curr_dir), "/"), file->d_name, 400 - strlen(curr_dir)), 399);
		lstat(path, buf);
		if (sort == 'S')
			node->sort_int = (int)buf->st_size;
		else if (sort == 't')
			node->sort_int = (int)buf->st_ctime;
		free(buf);
	}
	node->file = file;
	node->next = *head;
	node->prev = NULL;
	if (*head != NULL)
		(*head)->prev = node;
	*head = node;
	return (node);
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
file_list_t *insert_file_node_at_index(file_list_t **h, unsigned int idx, struct dirent *file)
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
				new->file = file;
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
	return (NULL);
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
file_list_t *add_node(file_list_t **h, file_list_t *w, struct dirent *file)
{

	file_list_t *new, *walk;

	walk = w;
	new = malloc(sizeof(file_list_t));
	if (new == NULL)
		return (NULL);
	new->file = file;
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
	return (new);
}



/**
 * reverse_file_list - reverse a linked list
 *
 * @head: linked list
 */
void reverse_file_list(file_list_t **head)
{
	file_list_t *current, *tmp;

	current = *head;
	tmp = NULL;
	while (current != NULL)
	{
		tmp = current->prev;
		current->prev = current->next;
		current->next = tmp;
		current = current->prev;
	}
	if (tmp != NULL)
		*head = tmp->prev;
}
