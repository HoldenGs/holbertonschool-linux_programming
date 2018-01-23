#include "header.h"

#define ERROR 1

int errno;

/**
 * main - main ls function
 *
 * Return: 0
 */
int main(int argc, char **argv)
{
	char dir[400], format, hidden;
	int i, j, max_src_bytes = 397;
	DIR *dirp;


	strcpy(dir, ".");
	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] != '-')
		{
			if (argv[i][0] != '/')
			{
				strcpy(dir, "./");
				strncat(dir, argv[i], max_src_bytes);
			}
			else
				strcpy(dir, argv[i]);
		}
		else
		{
			for (j = 1; argv[i][j]; j++)
			{
				if (argv[i][j] == '1')
					format = '1';
				if (argv[i][j] == 'l')
					format = 'l';
				if (argv[i][j] == 'a')
					hidden = 'a';
			}
		}
	}

	printf("%s\n", dir);
	dirp = opendir(dir);
	if (dirp == NULL)
	{
		perror("");
		printf("%i\n", errno);
		return (ERROR);
	}

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
