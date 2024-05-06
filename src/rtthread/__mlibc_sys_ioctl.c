#include "../internal/syscall.h"
#include <stdarg.h>
#include <dfs_file.h>

/**
 * @brief  * this function is a POSIX compliant version, which shall perform a variety of
 * control functions on devices.
 * 
 * @param fildes the file description
 * @param cmd the specified command
 * @param ... represents the additional information that is needed by this
 * specific device to perform the requested function.
 *
 * @return 0 on successful completion. Otherwise, -1 shall be returned and errno
 * set to indicate the error.
 */
int __mlibc_sys_ioctl(int fd, int cmd, ...)
{
    int res = 0;
    va_list args;

    va_start(args, cmd);
    res = ioctl(fd, cmd, args);
    va_end(args);

    return res;
}
