#include "../internal/stdio_impl.h"
#include <fcntl.h>

static unsigned char stdin_buf[BUFSIZ + UNGET];
FILE __stdin_FILE = {
	.fd = 0,
    .buf = stdin_buf + UNGET,
	.buf_size = BUFSIZ,
	.flags = F_PERM | F_NOWR,
	.read = __mlibc_read,
	.seek = __mlibc_lseek,
	.close = __mlibc_close,
};
FILE *stdin = &__stdin_FILE;