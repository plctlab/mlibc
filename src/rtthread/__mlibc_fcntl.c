#include "../internal/syscall.h"
#include <fcntl.h>
#include <errno.h>
#include <dfs_file.h>
#include <rtthread.h>

int __mlibc_fcntl(int fd, int cmd, ...);

static long do_fcntl(int fd, unsigned int cmd, unsigned int cmd, unsigned long arg, 
        struct dfs_file* filp)
{
    
}










