#include "../internal/stdio_impl.h"
#include <fcntl.h>

static unsigned char stderr_buf[UNGET];
FILE __stderr_FILE = {
	.fd = 0,
    .buf = stderr_buf + UNGET,
	.buf_size = 0,
	.flags = F_PERM | F_NORD,
    .lbf = -1,
	.write = __mlibc_write,
	.seek = __mlibc_lseek,
	.close = __mlibc_close,
};
FILE *stderr = &__stderr_FILE;