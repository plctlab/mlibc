#include "../internal/stdio_impl.h"
#include <fcntl.h>
#include <string.h>

int __mlibc_fmode_to_flags(const char *mode)
{
    int flags = 0;

	if (strchr(mode, '+'))
        flags = O_RDWR;
	else if (*mode == 'r')
        flags = O_RDONLY;
	else
        flags = O_WRONLY;

	if (*mode != 'r') flags |= O_CREAT;

	if (*mode == 'w') flags |= O_TRUNC;

	if (*mode == 'a') flags |= O_APPEND;

	return flags;
}