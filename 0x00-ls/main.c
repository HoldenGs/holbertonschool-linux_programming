#include "header.h"


/**
 * main - main ls function
 *
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0, or the errno of the error
 */
int main(int argc, char **argv)
{
	struct dirent *read;
	char dir[400], error_message[400], format, hidden;
	int i, j, dir_count, max_src_bytes = 397;
	dir_list_t *dir_list, *dir_head;
	file_list_t *file_list;
	DIR *dirp;
	int errno;

	format = ' ';
	hidden = ' ';
	dir_count = 0;
	file_list = NULL;
	dir_list = NULL;
	setlocale(LC_ALL, "");

	strcpy(dir, ".");
	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			for (j = 1; argv[i][j]; j++)
			{
				if (argv[i][j] == '1')
					format = '1';
				else if (argv[i][j] == 'l')
					format = 'l';
				if (argv[i][j] == 'a')
					hidden = 'a';
				else if (argv[i][j] == 'A')
					hidden = 'A';
			}
		}
		else
		{
			memset(dir, 0, 2);
			strncpy(dir, argv[i], max_src_bytes);
			dir_list = add_dir_node(&dir_list, dir);
			if (dir_count == 0)
				dir_list->next = NULL;
			dir_count++;
		}
	}

	if (dir_count == 0)
	{
		dir_list = add_dir_node(&dir_list, dir);
		dir_list->next = NULL;
	}

	dir_list = reverse_dir_list(&dir_list);
	dir_head = dir_list;
	while (dir_list != NULL)
	{
		dirp = opendir(dir_list->dir);
		if (dirp == NULL)
		{
			strcpy(error_message, "hls: cannot access ");
			max_src_bytes = 381;
			perror(strncat(error_message, dir_list->dir, max_src_bytes));
			return (errno);
		}
		if (dir_count > 1)
			printf("%s:\n", dir_list->dir);


		while ((read = readdir(dirp)) != NULL)
			file_list = add_file_node(&file_list, read->d_name);
		cocktail_sort_list(&file_list);


		switch (format)
		{
			case '1':
				print_ls(hidden, '\n', file_list);
				break;
			case 'l':
				print_ls(hidden, '\n', file_list);
				break;
			default:
				print_ls(hidden, '\t', file_list); 
		}
		if (dir_list->next != NULL)
			putchar('\n');

		free_file_list(&file_list);
		closedir(dirp);
		dir_list = dir_list->next;
	}

	free_dir_list(&dir_head);
	return 0;
}


/**
 * print_ls - print contents in the default ls format, i.e. columns
 *
 * @hidden: parameter denoting the option for revealing hidden files
 * @format: printing format parameter
 * @dirp: pointer to the directory data
 *
 * Return: 0 for success, 1 for failure
 */
int print_ls(char hidden, char format, file_list_t *file_list)
{
	while (file_list != NULL)
	{
		if (hidden == 'a')
		{
			printf("%s", file_list->file);
			if (file_list->next != NULL)
				putchar(format);
		}
		else if (hidden == 'A')
		{
			if (strcoll(file_list->file, ".") != 0 &&
				strcoll(file_list->file, "..") != 0)
			{
				printf("%s", file_list->file);
				if (file_list->next != NULL)
					putchar(format);
			}
		}
		else
		{
			if (file_list->file[0] != '.')
			{
				printf("%s", file_list->file);
				if (file_list->next != NULL)
					putchar(format);
			}
		}
		file_list = file_list->next;
	}
	if (format == '\t')
		printf("\n");

	return 0;
}
