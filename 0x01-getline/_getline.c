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
		bytes_read = read(fd, buf + leftover_len, READ_SIZE);
		if (bytes_read < 1)
		{
			free(buf);
			return (NULL);
		}
	}
	else
		buf = malloc(sizeof(char) * (leftover_len));
	memcpy(buf, leftover_str, leftover_len);
	free(leftover_str);

	/* find the end of the line */
	for (i = 0; buf[i] != '\n'; i++)
	{
		if (i == (READ_SIZE * multiplier) + leftover_len - 1)
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
	line = malloc(sizeof(char) * i + 1);
	memcpy(line, buf, i);
	line[i] = '\0';
	leftover_str = strdup(buf + i + 1);
	free(buf);
	return (line);
}


/*
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
	free(buf);
	return (new_buf);
}


/*
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
