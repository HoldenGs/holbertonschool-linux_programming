# getline function
How to use:
Invoke the _getline function by using `_getline(fd)`, fd being the file descriptor number returned from the open() function.
Example:
```c
fd = open("main.c", 0);
while ((line = _getline(fd)))
{
    printf("%s\n", line);
    free(line);
}
close(fd);
```
