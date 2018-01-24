#ifndef HEADER_H


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>


/**
 * struct listint_s - singly linked list
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


int main(int, char **);
int print_ls(char, char, DIR *);

dir_list_t *append_dir_list(dir_list_t **head, char[400]);
void free_dir_list(dir_list_t **head);


#endif