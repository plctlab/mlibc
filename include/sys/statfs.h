#ifndef MLIBC_SYS_STATFS_H__
#define MLIBC_SYS_STATFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>
#include <sys/statvfs.h>

typedef struct __fsid_t {
    int __val[2];
} fsid_t;

struct statfs {
    unsigned long f_type, f_bsize, f_frsize;
    fsblkcnt_t f_blocks, f_bfree;
    fsfilcnt_t f_files, f_ffree;
    fsblkcnt_t f_bavail;
    fsid_t f_fsid;
    unsigned long f_namelen, f_flags, f_spare[5];
};

int statfs (const char *, struct statfs *);
int fstatfs (int, struct statfs *);

#ifdef __cplusplus
}
#endif

#endif
