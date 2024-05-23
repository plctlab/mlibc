#include "../internal/stdio_impl.h"
#include <sys/sys_fio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <compiler.h>

FILE *fdopen(int fd, const char *mode)
{
    FILE *f;

	/* check mode */
	if(!strchr("rwa", *mode)) 
    {
        /* error operation */
		return 0;
	}

	/* allocate memory for file and buffer */
	if(!(f=malloc(sizeof(FILE) + UNGET + BUFSIZ)))
    {
        return 0;
    }

	/* zero-fill FILE */
	memset(f, 0, sizeof *f);

	if(!strchr(mode, '+'))
    {
        f->flags = (*mode == 'r') ? F_NOWR : F_NORD;
    }

	/* Set append mode on fd if opened for append */
	if (*mode == 'a') {
		int flags = __mlibc_sys_fcntl(fd, F_GETFL);
		if (!(flags & O_APPEND))
        { 
            __mlibc_sys_fcntl(fd, F_SETFL, flags | O_APPEND);
        }
		f->flags |= F_APP;
	}

	f->fd = fd;
	f->buf = (unsigned char *)f + sizeof(FILE) + UNGET;
	f->buf_size = BUFSIZ;

	f->lbf = EOF;
	if (!(f->flags & F_NOWR))
		f->lbf = '\n';

	/* Initialize op ptrs. No problem if some are unneeded. */
	f->read = __mlibc_read;
	f->write = __mlibc_write;
	f->seek = __mlibc_lseek;
	f->close = __mlibc_close;

	return mlibc_file_add(f);
}