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
	char dir[400], format, hidden, sort, reverse, recurse;
	int i, j, r, errno, dir_count, max_src_bytes = 397;
	dir_list_t *dir_list, *dir_head;

	format = reverse = recurse = ' ';
	hidden = ' ';
	sort = ' ';
	dir_count = r = 0;
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
				if (argv[i][j] == 'R')
					recurse = 'R';
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
		r = ls(dir_list->dir, format, hidden, reverse, recurse, sort, dir_count);
		dir_list = dir_list->next;
	}

	free_dir_list(&dir_head);
	return (r);
}


/**
 * print_files - print contents in the default ls format, i.e. columns
 *
 * @format: printing format parameter
 * @curr_dir: current directory to be printed
 * @file_node: struct for the file data
 *
 * Return: 0 for success, 1 for failure
 */
int print_files(char format, char *curr_dir, file_list_t *file_node)
{
	struct stat *buf;
	unsigned int links, size;
	char path[400], dir[400], perms[11], *usr, *grp, *_time;

	while (file_node != NULL)
	{
		buf = NULL;
		if (format == 'l')
		{
			buf = malloc(sizeof(struct stat));
			strncpy(path, strncat(strcat(strcpy(dir, curr_dir), "/"), file_node->file->d_name, 400 - strlen(curr_dir)), 399);
			lstat(path, buf);
			usr = getpwuid(buf->st_uid)->pw_name;
			grp = getgrgid(buf->st_gid)->gr_name;
			_time = ctime(&(buf->st_ctime));
			_time[strlen(_time) - 1] = '\0';
			links = (unsigned int)buf->st_nlink;
			size = (unsigned int)buf->st_size;
			file_perms(buf->st_mode, perms);
			printf("%s %2i %s %s %5i %s ", perms, links, usr, grp, size, _time);
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
 * ls - lists all files in the specified directory
 *
 * @dir_name: name of current directory to ls
 * @format: format option
 * @hidden: hidden option
 * @reverse: reverse option
 * @recurse: recurse option
 * @sort: sorting option
 * @dir_count:
 *
 * Return: 0 for success, other numbers for failure
 */
int ls(char *dir_name, char format, char hidden, char reverse, char recurse, char sort, int dir_count)
{
	struct dirent *read;
	char error_message[400];
	int errno, r, max_src_bytes = 397;
	file_list_t *file_list;
	DIR *dirp;

	r = 0;
	file_list = NULL;

	dirp = opendir(dir_name);
	if (dirp == NULL)
	{
		strcpy(error_message, "hls: cannot access ");
		max_src_bytes = 381;
		perror(strncat(error_message, dir_name, max_src_bytes));
		return (errno);
	}


	while ((read = readdir(dirp)) != NULL)
	{
		if (recurse == 'R' && read->d_type & DT_DIR && (strcmp(read->d_name, ".") != 0 && strcmp(read->d_name, "..") != 0))
			ls(read->d_name, format, hidden, reverse, recurse, sort, dir_count);
		switch (hidden)
		{
			case 'a':
				file_list = add_file_node(&file_list, read, sort, dir_name);
				break;
			case 'A':
				if (strcmp(read->d_name, ".") != 0 && strcmp(read->d_name, "..") != 0)
					file_list = add_file_node(&file_list, read, sort, dir_name);
				break;
			case ' ':
				if (read->d_name[0] != '.')
					file_list = add_file_node(&file_list, read, sort, dir_name);
				break;
		}
	}
	if (sort == 'S' || sort == 't')
		cocktail_sort_by_int(&file_list);
	else
		cocktail_sort_by_name(&file_list);

	if (reverse == 'r')
		reverse_file_list(&file_list);


	if (dir_count > 1 || recurse == 'R')
		printf("%s:\n", dir_name);
	print_files(format, dir_name, file_list);
	if (dir_count > 1 || recurse == 'R')
		putchar('\n');

	free_file_list(&file_list);
	closedir(dirp);
	return (r);
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
