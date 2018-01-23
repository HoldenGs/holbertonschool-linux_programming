#include "header.h"

#define ERROR 1

/**
 * main - main ls function
 *
 * Return: 0
 */
int main(int argc, char *argv)
{
	int i;
	DIR *dirp;
	struct dirent *read;

	dirp = opendir(".");
	if (dirp == NULL)
		return (ERROR);

	// handle args
	i = 1;
	while (argv[i] != NULL)
	{
		if argv[i][i] == '-';
	}

	if (argv[1] == '-1')
	{
		while ((read = readdir(dirp)) != NULL)
		{
			printf("%s\n", read->d_name);
		}
	}
	closedir(dirp);

	return (0);
}
