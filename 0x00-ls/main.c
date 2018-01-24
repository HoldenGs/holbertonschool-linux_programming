#include "header.h"

int errno;

/**
 * main - main ls function
 *
 * Return: 0, or the errno of the error
 */
int main(int argc, char **argv)
{
	char dir[400], error_message[400], format, hidden;
	int i, j, dir_count, max_src_bytes = 397;
	dir_list_t *dir_list, *dir_node;
	DIR *dirp;

	dir_count = 0;

	strcpy(dir, ".");
	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			for (j = 1; argv[i][j]; j++)
			{
				if (argv[i][j] == '1')
					format = '1';
				if (argv[i][j] == 'l')
					format = 'l';
				if (argv[i][j] == 'a')
					hidden = 'a';
				else if (argv[i][j] == 'A')
					hidden = 'A';
			}
		}
		else
		{
			memset(dir, 0, strlen(dir));
			strcpy(dir, argv[i]);
			append_dir_list(&dir_list, dir);
			dir_count++;
		}
	}


	for (dir_node = dir_list; dir_node != NULL; dir_node = dir_node->next)
	{
		dirp = opendir(dir_node->dir);
		if (dirp == NULL)
		{
			strcpy(error_message, "hls: cannot access ");
			max_src_bytes = 381;
			perror(strncat(error_message, dir_node->dir, max_src_bytes));
			return (errno);
		}

		if (dir_count > 1)
			printf("%s:\n", dir_node->dir);
		switch(format)
		{
			case '1':
				print_ls(hidden, '\n', dirp);
				break;
			case 'l':
				print_ls(hidden, '\n', dirp);
				break;
			default:
				print_ls(hidden, '\t', dirp);
		}
		if (dir_node->next != NULL)
			putchar('\n');
	}

	free_dir_list(&dir_list);
	closedir(dirp);
	return (0);
}


/**
 * print_ls - print contents in the default ls format, i.e. columns
 *
 * Return: 0 for success, 1 for failure
 */
int print_ls(char hidden, char format, DIR *dirp)
{
	struct dirent *read;


	while ((read = readdir(dirp)) != NULL)
	{
		if (hidden == 'a')
		{
			printf("%s", read->d_name);
			if (read != NULL)
				putchar(format);
		}
		else if (hidden == 'A')
		{
			if (strcmp(read->d_name, ".") != 0 &&
				strcmp(read->d_name, "..") != 0)
			{
				printf("%s", read->d_name);
				if (read != NULL)
					putchar(format);
			}
		}
		else
		{
			if (read->d_name[0] != '.')
			{
				printf("%s", read->d_name);
				if (read != NULL)
					putchar(format);
			}
		}
	}
	if (format == '\t')
		printf("\n");

	return (0);
}
