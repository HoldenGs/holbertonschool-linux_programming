#ifndef HEADER_H

#define MAX_FILE_SIZE 255

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <locale.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>


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
	struct dirent *file;
	int sort_int;
	struct file_list_s *next;
} file_list_t;


/* main.c */
int main(int, char **);
int print_files(char, char *, file_list_t *);
int file_type(mode_t);
void file_perms(mode_t, char[11]);
int ls(char *, char *, char, char, char, char, char, int);

/* dir_list.c */
dir_list_t *add_dir_node(dir_list_t **, char[400]);
void free_dir_list(dir_list_t **);
dir_list_t *reverse_dir_list(dir_list_t **head);

/* file_list.c */
file_list_t *add_file_node(file_list_t **head, struct dirent *, char, char *);
void free_file_list(file_list_t **head);
file_list_t *insert_file_node_at_index(file_list_t **h, unsigned int idx, struct dirent *);
file_list_t *add_node(file_list_t **h, file_list_t *w, struct dirent *);
void reverse_file_list(file_list_t **);

/* sort_file_list.c */
void cocktail_sort_by_name(file_list_t **list);
void cocktail_sort_by_int(file_list_t **list);
void swap(file_list_t **list, file_list_t *to_swap, file_list_t *compare);


#endif