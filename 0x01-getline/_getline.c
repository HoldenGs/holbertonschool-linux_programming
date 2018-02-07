#include "_getline.h"


/**
 * _getline - returns a line from a file descriptor, can be called multiple times on the same file descriptor
 *
 * @fd: file descriptor int
 *
 * Return: char *line. If there's an error or no more lines to return, returns NULL.
 */
char *_getline(const int fd)
{
	static char *leftover_str;
	char *buf, *line;
	int leftover_len = 0, multiplier = 1, i, bytes_read;

	if (leftover_str != NULL)
		leftover_len = strlen(leftover_str);

	/* if there's no newline in the leftover string, read into buffer */
	if (check_for_newline(leftover_str, leftover_len) == 0)
	{
		buf = malloc(sizeof(char) * (READ_SIZE + leftover_len));
		bytes_read = read(fd, buf + leftover_len + 1, READ_SIZE);
		if (bytes_read < 1)
		{
			free(buf);
			return (NULL);
		}
	}
	else
		buf = malloc(sizeof(char) * (leftover_len));
	memcpy(buf, leftover_str, leftover_len);
	buf[leftover_len] = '\0';
	free(leftover_str);

	/* find the end of the line */
	for (i = 0; buf[i] != '\n'; i++)
	{
		/* if no newline has been found yet, increase buffer size */
		if (i == (READ_SIZE * multiplier) + leftover_len)
		{
			multiplier++;
			buf = add_buffer_space(i, buf);
			if (buf == NULL)
			{
				free(buf);
				return (NULL);
			}
			bytes_read = read(fd, buf + i, READ_SIZE);
			if (bytes_read < 1)
			{
				free(buf);
				return (NULL);
			}
		}
	}

	/* return line */
	line = malloc(sizeof(char) * (i + 1));
	memcpy(line, buf, i);
	line[i] = '\0';
	line = remove_null_bytes(line, i);
	leftover_str = strdup(buf + i + 1);
	free(buf);
	return (line);
}


/**
 * remove_null_bytes - removes all null bytes except the for the last byte
 *
 * @line: string to strip
 *
 * Return: pointer to string, NULL, on error
 */
char *remove_null_bytes(char *line, int len)
{
	int i, null_count = 0;
	char *stripped_line, *new_line;

	stripped_line = malloc(sizeof(char) * (len + 1));
	memcpy(stripped_line, line, len);
	stripped_line[len] = '\0';
	for (i = 0; i < len; i++)
	{
		if (line[i] == '\0')
			null_count++;
		if (null_count > 0 && i + null_count < len)
			stripped_line[i] = line[i + null_count];
		else if (null_count > 0)
			stripped_line[i] = '\0';
	}
	new_line = malloc(sizeof(char) * (i + 2 - null_count));
	memcpy(new_line, stripped_line, i + 1 - null_count); /* make a new line without extra null bytes at the end */
	free(line);
	free(new_line);

	return (stripped_line);
}


/**
 * add_buffer_space - add space to an existing buffer without destroying the information within
 *
 * @old_size: original size of the buffer
 * @buf: pointer to the buffer
 *
 * Return: resized buffer, or NULL if there's an error
 */
char *add_buffer_space(int old_size, char *buf)
{
	char *new_buf;

	new_buf = malloc(sizeof(char) * (old_size + READ_SIZE));
	memcpy(new_buf, buf, old_size);
	new_buf[old_size] = '\0';
	free(buf);
	return (new_buf);
}


/**
 * check_for_newline - check if there is a newline in the string
 *
 * @buf: string to check
 * @size: size of string
 *
 * Return: 0 if no newline, 1 if a newline was found
 */
int check_for_newline(char *buf, int size)
{
	int i;

	for (i = 0; i < size; i++)
		if (buf[i] == '\n')
			return (1);
	return (0);
}
