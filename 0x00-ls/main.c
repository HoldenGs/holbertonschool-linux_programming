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
	char dir[400], error_message[400], format, hidden, sort, reverse;
	int i, j, dir_count, max_src_bytes = 397;
	dir_list_t *dir_list, *dir_head;
	file_list_t *file_list;
	DIR *dirp;
	int errno;

	format = reverse = ' ';
	hidden = ' ';
	sort = ' ';
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
				if (argv[i][j] == 'S')
					sort = 'S';
				else if (argv[i][j] == 't')
					sort = 't';
				if (argv[i][j] == 'r')
					reverse = 'r';
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
		dir_count++;
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
		{
			switch (hidden)
			{
				case 'a':
					file_list = add_file_node(&file_list, read, sort, dir_list);
					break;
				case 'A':
					if (strcmp(read->d_name, ".") != 0 && strcmp(read->d_name, "..") != 0)
						file_list = add_file_node(&file_list, read, sort, dir_list);
					break;
				case ' ':
					if (read->d_name[0] != '.')
						file_list = add_file_node(&file_list, read, sort, dir_list);
					break;
			}
		}
		if (sort == 'S' || sort == 't')
			cocktail_sort_by_int(&file_list);
		else
			cocktail_sort_by_name(&file_list);

		if (reverse == 'r')
			reverse_file_list(&file_list);


		print_ls(format, dir_list, file_list);

		if (dir_list->next != NULL)
			putchar('\n');

		free_file_list(&file_list);
		closedir(dirp);
		dir_list = dir_list->next;
	}

	free_dir_list(&dir_head);
	return (0);
}


/**
 * print_ls - print contents in the default ls format, i.e. columns
 *
 * @format: printing format parameter
 * @curr_dir: current directory to be printed
 * @file_node: struct for the file data
 *
 * Return: 0 for success, 1 for failure
 */
int print_ls(char format, dir_list_t *curr_dir, file_list_t *file_node)
{
	struct stat *buf;
	char path[400], dir[400], perms[11], *usr, *grp, *_time;

	while (file_node != NULL)
	{
		buf = NULL;
		if (format == 'l')
		{
			buf = malloc(sizeof(struct stat));
			strncpy(path, strncat(strcat(strcpy(dir, curr_dir->dir), "/"), file_node->file->d_name, 400 - strlen(curr_dir->dir)), 399);
			lstat(path, buf);
			usr = getpwuid(buf->st_uid)->pw_name;
			grp = getgrgid(buf->st_gid)->gr_name;
			_time = ctime(&(buf->st_ctime));
			_time[strlen(_time) - 1] = '\0';
			file_perms(buf->st_mode, perms);
			printf("%s %2i %s %s %5i %s ", perms, (unsigned)buf->st_nlink, usr, grp, (unsigned)buf->st_size, _time);
			free(buf);
		}
		printf("%s", file_node->file->d_name);
		if (file_node->next != NULL)
		{
			switch (format)
			{
				case 'l':
					putchar('\n');
					break;
				case '1':
					putchar('\n');
					break;
				default:
					putchar('\t');
			}
		}
		file_node = file_node->next;
	}
	putchar('\n');

	return (0);
}


/**
 * file_type - get long format file type
 *
 * @mode: file mode information
 *
 * Return: character corresponding to file type
 */
int file_type(mode_t mode)
{
	int c;

	if (S_ISREG(mode))
			c = '-';
	else if (S_ISDIR(mode))
			c = 'd';
	else if (S_ISLNK(mode))
			c = 'l';
	else if (S_ISSOCK(mode))
			c = 's';
	else if (S_ISBLK(mode))
			c = 'b';
	else if (S_ISCHR(mode))
			c = 'c';
	else if (S_ISFIFO(mode))
			c = 'p';

	return (c);
}


/**
 * file_perms - add long format file perms from @buf to bits to be printed
 *
 * @mode: file mode information
 * @perms: character array to print
 */
void file_perms(mode_t mode, char perms[11])
{
	static const char *rwx[] = {"---", "--x", "-w-", "r--", "-wx", "r-x", "rw-", "rwx"};

	perms[0] = file_type(mode);
	strcpy(&perms[1], rwx[(mode >> 6) & 7]);
	strcpy(&perms[4], rwx[(mode >> 3) & 7]);
	strcpy(&perms[7], rwx[mode & 7]);
	if (mode & S_ISUID)
		perms[1] = (mode & S_IXUSR) ? 's' : 'S';
	if (mode & S_ISGID)
		perms[4] = (mode & S_IXGRP) ? 's' : 'l';
	if (mode & S_ISVTX)
		perms[7] = (mode & S_IXOTH) ? 't' : 'T';
	perms[10] = '\0';
}
