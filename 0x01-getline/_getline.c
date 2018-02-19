#include "_getline.h"


/**
 * _getline - returns a line from a file descriptor,
 * can be called multiple times on the same file descriptor
 *
 * @fd: file descriptor int
 *
 * Return: char *line. If there's an error or no more lines to return,
 * returns NULL.
 */
char *_getline(const int fd)
{
	int line_len = 0, fd_found = 0;
	char *buf, *n_ptr, *line;
	static fd_list_t *fd_head;
	fd_list_t *fd_list;

	buf = NULL;
	if (fd_head)
		fd_list = fd_head;
	else
		fd_list = NULL;
	/* if fd was used before, make current buffer that fd's buffer */
	while (fd_list != NULL)
	{
		if (fd_list->fd == fd)
		{
			fd_found = 1;
			buf = fd_list->buf;
			break;
		}
		fd_list = fd_list->next;
	}
	/* if fd was not used before, read all of it's contents into a buffer */
	if (!fd_found)
		buf = file_to_buffer(&fd_head, &fd_list, fd);
	/* free fd list entry if we're done reading from the file */
	if (fd_list->bytes_left < 1)
	{
		fd_head = fd_list->next;
		free(fd_list);
		return (NULL);
	}
	n_ptr = _memchr(buf, '\n', fd_list->bytes_left);
	if (n_ptr == NULL)
		line_len = fd_list->bytes_left + 1;
	else
		line_len = (n_ptr - buf) + 1;
	fd_list->bytes_left -= line_len;
	line = remove_null_bytes(buf, line_len - 1);
	buf = buf + line_len;
	fd_list->buf = buf;
	return (line);
}


/**
 * _memchr - search @len bytes of @buf for char @c
 *
 * @buf: buffer to search
 * @c: character to search for
 * @len: length of @buf to search in
 *
 * Return: pointer to place in @buf where @c was found,
 * or NULL if not found
 */
char *_memchr(char *buf, char c, int len)
{
	int i;

	for (i = 0; i < len; i++)
		if (buf[i] == c)
			return (buf + i);
	return (NULL);
}


/**
 * file_to_buffer - put entire file inside of a buffer
 *
 * @fd_head: fd_list head
 * @fd_list: fd_list current entry
 * @fd: actual file descriptor being added to fd_list
 *
 * Return: buffer with file contents, or NULL on error
 */
char *file_to_buffer(fd_list_t **fd_head, fd_list_t **fd_list, const int fd)
{
	int total_bytes_read = 0, bytes_read = 0;
	char *buf;

	buf = NULL;
	if (!fd_head)
	{
		fd_head = malloc(sizeof(*fd_head));
		(*fd_head)->next = NULL;
		(*fd_head)->fd = fd;
		*fd_list = *fd_head;
	}
	else
	{
		*fd_list = prepend_fd(fd_head, fd);
		*fd_head = *fd_list;
	}
	buf = malloc(sizeof(char) * READ_SIZE);
	while ((bytes_read = read(fd, buf + total_bytes_read, READ_SIZE)) != 0)
	{
		if (bytes_read == -1)
			return (NULL);
		total_bytes_read += bytes_read;
		buf = _realloc(buf, total_bytes_read + READ_SIZE, total_bytes_read);
	}
	buf = _realloc(buf, total_bytes_read, total_bytes_read);
	(*fd_list)->bytes_left = total_bytes_read;
	return (buf);
}


/**
 * _realloc - reallocate bytes to total_bytes_read length,
 * with extra bytes set to '\0'
 *
 * @buf: buffer to reallocate
 * @new_length: new buffer length
 * @old_length: old buffer length
 *
 * Return: new buffer
 */
char *_realloc(char *buf, int new_length, int old_length)
{
	char *new_buf;

	new_buf = malloc(sizeof(char) * new_length);
	memset(new_buf, '\0', new_length);
	memcpy(new_buf, buf, old_length);
	buf = malloc(sizeof(char) * new_length);
	memset(buf, '\0', new_length);
	memcpy(buf, new_buf, new_length);
	free(new_buf);
	return (buf);
}


/**
 * prepend_fd - add a new node at the start of a fd_list_t linked list
 *
 * @head: start of linked list
 * @fd: directory data to add to node
 *
 * Return: address of new node; NULL if failure
 */
fd_list_t *prepend_fd(fd_list_t **head, int fd)
{
	fd_list_t *node;

	node = malloc(sizeof(fd_list_t));
	if (node == NULL)
		return (NULL);
	node->fd = fd;
	node->next = *head;
	*head = node;
	return (node);
}


/**
 * remove_null_bytes - removes all null bytes except the for the last byte
 *
 * @line: string to strip
 * @len: length of @line
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
	memcpy(new_line, stripped_line, i + 1 - null_count);
	free(new_line);

	return (stripped_line);
}
