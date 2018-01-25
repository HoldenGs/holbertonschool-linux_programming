#ifndef HEADER_H


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <locale.h>


/**
 * struct listint_s - doubly linked list
 * @n: integer
 * @next: points to the next node
 *
 * Description: singly linked list node structure
 * for Holberton project
 */
typedef struct dir_list_s
{
	char dir[400];
	struct dir_list_s *next;
} dir_list_t;

/**
 * struct listint_s - doubly linked list
 * @n: integer
 * @next: points to the next node
 *
 * Description: singly linked list node structure
 * for Holberton project
 */
typedef struct file_list_s
{
	struct file_list_s *prev;
	char file[256];
	struct file_list_s *next;
} file_list_t;


/* main.c */
int main(int, char **);
int print_ls(char, char, file_list_t *);

/* dir_list.c */
dir_list_t *add_dir_node(dir_list_t **, char[400]);
void free_dir_list(dir_list_t **);
dir_list_t *reverse_dir_list(dir_list_t **head);

/* file_list.c */
file_list_t *add_file_node(file_list_t **head, const char[256]);
void free_file_list(file_list_t **head);
file_list_t *insert_file_node_at_index(file_list_t **h, unsigned int idx, const char[256]);
file_list_t *add_node(file_list_t **h, file_list_t *w, const char[256]);

/* sort_file_list.c */
void cocktail_sort_list(file_list_t **list);
void swap(file_list_t **list, file_list_t *to_swap, file_list_t *compare);


#endif