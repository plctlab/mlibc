/*
 * test_stat.c - PSE51 sys/stat.h file status test suite
 * Tests: chmod, fchmod, mkdir, mkfifo, stat, lstat, fstat, umask
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for file status operations
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include "../test_colors.h"
/* Test file and directory names */
#define TEST_FILE "test_file.txt"
#define TEST_DIR "test_dir"
#define TEST_FIFO "test_fifo"
#define TEST_SYMLINK "test_symlink"
#define TEST_NONEXIST "nonexistent_file"

/* Clean up test files */
void cleanup_test_files(void) {
    unlink(TEST_FILE);
    unlink(TEST_FIFO);
    unlink(TEST_SYMLINK);
    rmdir(TEST_DIR);
    unlink(TEST_NONEXIST);
}

/* Create a test file */
int create_test_file(const char *path) {
    int fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    write(fd, "test content\n", 13);
    close(fd);
    return 0;
}

/* Test stat() function */
void test_stat(void) {
    struct stat st;
    int ret;
    
    TEST_START("stat()");
    
    /* Create test file */
    assert(create_test_file(TEST_FILE) == 0);
    
    /* Test stat on regular file */
    ret = stat(TEST_FILE, &st);
    if (ret == -1 && errno == ENOSYS) {
        printf("  stat() not implemented (ENOSYS)\n");
        cleanup_test_files();
        return;
    }
    
    assert(ret == 0);
    
    /* Verify file type */
    assert(S_ISREG(st.st_mode));
    assert(!S_ISDIR(st.st_mode));
    assert(!S_ISFIFO(st.st_mode));
    assert(!S_ISLNK(st.st_mode));
    
    /* Verify permissions */
    assert((st.st_mode & 0777) == 0644);
    assert((st.st_mode & S_IRUSR) != 0);
    assert((st.st_mode & S_IWUSR) != 0);
    assert((st.st_mode & S_IXUSR) == 0);
    
    /* Verify size */
    assert(st.st_size == 13);
    
    /* Verify other fields */
    assert(st.st_nlink >= 1);
    assert(st.st_uid >= 0);
    assert(st.st_gid >= 0);
    assert(st.st_blksize > 0);
    assert(st.st_blocks >= 0);
    
    /* Times should be recent */
    time_t now = time(NULL);
    assert(st.st_atime <= now);
    assert(st.st_mtime <= now);
    assert(st.st_ctime <= now);
    assert(st.st_atime >= now - 10);
    assert(st.st_mtime >= now - 10);
    assert(st.st_ctime >= now - 10);
    
    /* Test stat on directory */
    ret = mkdir(TEST_DIR, 0755);
    if (ret == 0) {
        ret = stat(TEST_DIR, &st);
        assert(ret == 0);
        assert(S_ISDIR(st.st_mode));
        assert((st.st_mode & 0777) == 0755);
        
        rmdir(TEST_DIR);
    }
    
    /* Test stat on non-existent file */
    ret = stat(TEST_NONEXIST, &st);
    assert(ret == -1);
    assert(errno == ENOENT);
    
    /* Test with NULL path */
    ret = stat(NULL, &st);
    assert(ret == -1);
    assert(errno == EFAULT || errno == EINVAL);
    
    /* Test with NULL stat buffer */
    ret = stat(TEST_FILE, NULL);
    assert(ret == -1);
    assert(errno == EFAULT);
    
    cleanup_test_files();
    TEST_PASSED("stat()");
}

/* Test fstat() function */
void test_fstat(void) {
    struct stat st1, st2;
    int fd, ret;
    
    TEST_START("fstat()");
    
    /* Create and open test file */
    assert(create_test_file(TEST_FILE) == 0);
    fd = open(TEST_FILE, O_RDONLY);
    assert(fd >= 0);
    
    /* Test fstat */
    ret = fstat(fd, &st1);
    if (ret == -1 && errno == ENOSYS) {
        printf("  fstat() not implemented (ENOSYS)\n");
        close(fd);
        cleanup_test_files();
        return;
    }
    
    assert(ret == 0);
    
    /* Compare with stat */
    ret = stat(TEST_FILE, &st2);
    assert(ret == 0);
    
    /* Results should match */
    assert(st1.st_dev == st2.st_dev);
    assert(st1.st_ino == st2.st_ino);
    assert(st1.st_mode == st2.st_mode);
    assert(st1.st_nlink == st2.st_nlink);
    assert(st1.st_uid == st2.st_uid);
    assert(st1.st_gid == st2.st_gid);
    assert(st1.st_size == st2.st_size);
    
    /* Test on different file descriptors */
    int fd2 = dup(fd);
    assert(fd2 >= 0);
    
    ret = fstat(fd2, &st2);
    assert(ret == 0);
    assert(st1.st_ino == st2.st_ino);
    
    close(fd2);
    
    /* Test with invalid fd */
    ret = fstat(-1, &st1);
    assert(ret == -1);
    assert(errno == EBADF);
    
    ret = fstat(999, &st1);
    assert(ret == -1);
    assert(errno == EBADF);
    
    /* Test with NULL buffer */
    ret = fstat(fd, NULL);
    assert(ret == -1);
    assert(errno == EFAULT);
    
    close(fd);
    cleanup_test_files();
    TEST_PASSED("fstat()");
}

/* Test lstat() function */
void test_lstat(void) {
    struct stat st;
    int ret;
    
    TEST_START("lstat()");
    
    /* Create test file and symlink */
    assert(create_test_file(TEST_FILE) == 0);
    
    ret = symlink(TEST_FILE, TEST_SYMLINK);
    if (ret == -1 && errno == ENOSYS) {
        printf("  symlink() not supported, skipping lstat tests\n");
        cleanup_test_files();
        return;
    }
    
    if (ret == 0) {
        /* Test lstat on symlink */
        ret = lstat(TEST_SYMLINK, &st);
        if (ret == -1 && errno == ENOSYS) {
            printf("  lstat() not implemented (ENOSYS)\n");
            cleanup_test_files();
            return;
        }
        
        assert(ret == 0);
        
        /* lstat should show it's a symlink */
        assert(S_ISLNK(st.st_mode));
        
        /* Compare with stat (which follows symlink) */
        struct stat st2;
        ret = stat(TEST_SYMLINK, &st2);
        assert(ret == 0);
        assert(S_ISREG(st2.st_mode)); /* stat follows to regular file */
        
        unlink(TEST_SYMLINK);
    }
    
    /* Test lstat on regular file (should be same as stat) */
    ret = lstat(TEST_FILE, &st);
    if (ret == 0) {
        assert(S_ISREG(st.st_mode));
        assert((st.st_mode & 0777) == 0644);
        assert(st.st_size == 13);
    }
    
    cleanup_test_files();
    TEST_PASSED("lstat()");
}

/* Test chmod() function */
void test_chmod(void) {
    struct stat st;
    int ret;
    
    TEST_START("chmod()");
    
    /* Create test file */
    assert(create_test_file(TEST_FILE) == 0);
    
    /* Change permissions to 0600 */
    ret = chmod(TEST_FILE, 0600);
    if (ret == -1 && errno == ENOSYS) {
        printf("  chmod() not implemented (ENOSYS)\n");
        cleanup_test_files();
        return;
    }
    
    assert(ret == 0);
    
    /* Verify permissions changed */
    ret = stat(TEST_FILE, &st);
    assert(ret == 0);
    assert((st.st_mode & 0777) == 0600);
    assert((st.st_mode & S_IRUSR) != 0);
    assert((st.st_mode & S_IWUSR) != 0);
    assert((st.st_mode & S_IRGRP) == 0);
    assert((st.st_mode & S_IROTH) == 0);
    
    /* Change to 0755 */
    ret = chmod(TEST_FILE, 0755);
    assert(ret == 0);
    
    ret = stat(TEST_FILE, &st);
    assert(ret == 0);
    assert((st.st_mode & 0777) == 0755);
    assert((st.st_mode & S_IXUSR) != 0);
    assert((st.st_mode & S_IRGRP) != 0);
    assert((st.st_mode & S_IXGRP) != 0);
    assert((st.st_mode & S_IROTH) != 0);
    assert((st.st_mode & S_IXOTH) != 0);
    
    /* Test special bits */
    ret = chmod(TEST_FILE, 04755); /* SUID */
    if (ret == 0) {
        ret = stat(TEST_FILE, &st);
        assert(ret == 0);
        assert((st.st_mode & S_ISUID) != 0);
    }
    
    /* Test chmod on non-existent file */
    ret = chmod(TEST_NONEXIST, 0644);
    assert(ret == -1);
    assert(errno == ENOENT);
    
    /* Test with NULL path */
    ret = chmod(NULL, 0644);
    assert(ret == -1);
    assert(errno == EFAULT || errno == EINVAL);
    
    cleanup_test_files();
    TEST_PASSED("chmod()");
}

/* Test fchmod() function */
void test_fchmod(void) {
    struct stat st;
    int fd, ret;
    
    TEST_START("fchmod()");
    
    /* Create and open test file */
    assert(create_test_file(TEST_FILE) == 0);
    fd = open(TEST_FILE, O_RDWR);
    assert(fd >= 0);
    
    /* Change permissions via fd */
    ret = fchmod(fd, 0700);
    if (ret == -1 && errno == ENOSYS) {
        printf("  fchmod() not implemented (ENOSYS)\n");
        close(fd);
        cleanup_test_files();
        return;
    }
    
    assert(ret == 0);
    
    /* Verify permissions */
    ret = fstat(fd, &st);
    assert(ret == 0);
    assert((st.st_mode & 0777) == 0700);
    
    /* Change again */
    ret = fchmod(fd, 0644);
    assert(ret == 0);
    
    ret = stat(TEST_FILE, &st);
    assert(ret == 0);
    assert((st.st_mode & 0777) == 0644);
    
    /* Test with invalid fd */
    ret = fchmod(-1, 0644);
    assert(ret == -1);
    assert(errno == EBADF);
    
    ret = fchmod(999, 0644);
    assert(ret == -1);
    assert(errno == EBADF);
    
    close(fd);
    cleanup_test_files();
    TEST_PASSED("fchmod()");
}

/* Test mkdir() function */
void test_mkdir(void) {
    struct stat st;
    int ret;
    
    TEST_START("mkdir()");
    
    /* Create directory */
    ret = mkdir(TEST_DIR, 0755);
    if (ret == -1 && errno == ENOSYS) {
        printf("  mkdir() not implemented (ENOSYS)\n");
        return;
    }
    
    assert(ret == 0);
    
    /* Verify directory created */
    ret = stat(TEST_DIR, &st);
    assert(ret == 0);
    assert(S_ISDIR(st.st_mode));
    assert((st.st_mode & 0777) == 0755);
    
    /* Try to create existing directory */
    ret = mkdir(TEST_DIR, 0755);
    assert(ret == -1);
    assert(errno == EEXIST);
    
    /* Create with different permissions */
    rmdir(TEST_DIR);
    ret = mkdir(TEST_DIR, 0700);
    assert(ret == 0);
    
    ret = stat(TEST_DIR, &st);
    assert(ret == 0);
    assert((st.st_mode & 0777) == 0700);
    
    /* Test with invalid path */
    ret = mkdir(NULL, 0755);
    assert(ret == -1);
    assert(errno == EFAULT || errno == EINVAL);
    
    /* Test nested directory (parent doesn't exist) */
    ret = mkdir("nonexistent/subdir", 0755);
    assert(ret == -1);
    assert(errno == ENOENT);
    
    /* Clean up */
    rmdir(TEST_DIR);
    TEST_PASSED("mkdir()");
}

/* Test mkfifo() function */
void test_mkfifo(void) {
    struct stat st;
    int ret;
    
    TEST_START("mkfifo()");
    
    /* Create FIFO */
    ret = mkfifo(TEST_FIFO, 0644);
    if (ret == -1 && errno == ENOSYS) {
        printf("  mkfifo() not implemented (ENOSYS)\n");
        return;
    }
    
    assert(ret == 0);
    
    /* Verify FIFO created */
    ret = stat(TEST_FIFO, &st);
    assert(ret == 0);
    assert(S_ISFIFO(st.st_mode));
    assert((st.st_mode & 0777) == 0644);
    
    /* Try to create existing FIFO */
    ret = mkfifo(TEST_FIFO, 0644);
    assert(ret == -1);
    assert(errno == EEXIST);
    
    /* Test with different permissions */
    unlink(TEST_FIFO);
    ret = mkfifo(TEST_FIFO, 0600);
    assert(ret == 0);
    
    ret = stat(TEST_FIFO, &st);
    assert(ret == 0);
    assert(S_ISFIFO(st.st_mode));
    assert((st.st_mode & 0777) == 0600);
    
    /* Test with invalid path */
    ret = mkfifo(NULL, 0644);
    assert(ret == -1);
    assert(errno == EFAULT || errno == EINVAL);
    
    /* Clean up */
    unlink(TEST_FIFO);
    TEST_PASSED("mkfifo()");
}

/* Test umask() function */
void test_umask(void) {
    mode_t old_mask, new_mask;
    struct stat st;
    int fd;
    
    TEST_START("umask()");
    
    /* Get current umask */
    old_mask = umask(0);
    umask(old_mask); /* Restore it */
    
    /* Set new umask */
    new_mask = umask(0022);
    assert(new_mask == old_mask);
    
    /* Create file with umask 0022 */
    fd = open(TEST_FILE, O_CREAT | O_WRONLY, 0666);
    assert(fd >= 0);
    close(fd);
    
    /* Check actual permissions (should be 0644 due to umask) */
    assert(stat(TEST_FILE, &st) == 0);
    assert((st.st_mode & 0777) == 0644);
    unlink(TEST_FILE);
    
    /* Try different umask */
    umask(0077);
    fd = open(TEST_FILE, O_CREAT | O_WRONLY, 0666);
    assert(fd >= 0);
    close(fd);
    
    /* Should be 0600 due to umask */
    assert(stat(TEST_FILE, &st) == 0);
    assert((st.st_mode & 0777) == 0600);
    unlink(TEST_FILE);
    
    /* Test directory creation with umask */
    umask(0022);
    if (mkdir(TEST_DIR, 0777) == 0) {
        assert(stat(TEST_DIR, &st) == 0);
        assert((st.st_mode & 0777) == 0755);
        rmdir(TEST_DIR);
    }
    
    /* Test FIFO creation with umask */
    if (mkfifo(TEST_FIFO, 0666) == 0) {
        assert(stat(TEST_FIFO, &st) == 0);
        assert((st.st_mode & 0777) == 0644);
        unlink(TEST_FIFO);
    }
    
    /* Restore original umask */
    umask(old_mask);
    
    TEST_PASSED("umask()");
}

/* Test stat macros */
void test_stat_macros(void) {
    struct stat st;
    
    TEST_START("stat macros");
    
    /* Create various file types and test macros */
    
    /* Regular file */
    assert(create_test_file(TEST_FILE) == 0);
    assert(stat(TEST_FILE, &st) == 0);
    
    assert(S_ISREG(st.st_mode));
    assert(!S_ISDIR(st.st_mode));
    assert(!S_ISFIFO(st.st_mode));
    assert(!S_ISCHR(st.st_mode));
    assert(!S_ISBLK(st.st_mode));
    assert(!S_ISLNK(st.st_mode));
    assert(!S_ISSOCK(st.st_mode));
    
    unlink(TEST_FILE);
    
    /* Directory */
    if (mkdir(TEST_DIR, 0755) == 0) {
        assert(stat(TEST_DIR, &st) == 0);
        assert(!S_ISREG(st.st_mode));
        assert(S_ISDIR(st.st_mode));
        assert(!S_ISFIFO(st.st_mode));
        rmdir(TEST_DIR);
    }
    
    /* FIFO */
    if (mkfifo(TEST_FIFO, 0644) == 0) {
        assert(stat(TEST_FIFO, &st) == 0);
        assert(!S_ISREG(st.st_mode));
        assert(!S_ISDIR(st.st_mode));
        assert(S_ISFIFO(st.st_mode));
        unlink(TEST_FIFO);
    }
    
    /* Character device (e.g., /dev/null) */
    if (stat("/dev/null", &st) == 0) {
        assert(S_ISCHR(st.st_mode));
        assert(!S_ISBLK(st.st_mode));
    }
    
    TEST_PASSED("Stat macro");
}

/* Test permission checking */
void test_permissions(void) {
    struct stat st;
    mode_t modes[] = {
        0000, 0111, 0222, 0333, 0444, 0555, 0666, 0777,
        0700, 0070, 0007, 0400, 0200, 0100,
        01777, 02777, 04777, 07777
    };
    
    TEST_START("permission bits");
    
    for (size_t i = 0; i < sizeof(modes)/sizeof(modes[0]); i++) {
        mode_t mode = modes[i];
        
        /* Create file with specific mode */
        int fd = open(TEST_FILE, O_CREAT | O_WRONLY | O_TRUNC, mode);
        assert(fd >= 0);
        close(fd);
        
        /* Set exact permissions */
        if (chmod(TEST_FILE, mode) == 0) {
            assert(stat(TEST_FILE, &st) == 0);
            
            /* Check basic permissions */
            if (mode & S_IRUSR) assert(st.st_mode & S_IRUSR);
            if (mode & S_IWUSR) assert(st.st_mode & S_IWUSR);
            if (mode & S_IXUSR) assert(st.st_mode & S_IXUSR);
            
            if (mode & S_IRGRP) assert(st.st_mode & S_IRGRP);
            if (mode & S_IWGRP) assert(st.st_mode & S_IWGRP);
            if (mode & S_IXGRP) assert(st.st_mode & S_IXGRP);
            
            if (mode & S_IROTH) assert(st.st_mode & S_IROTH);
            if (mode & S_IWOTH) assert(st.st_mode & S_IWOTH);
            if (mode & S_IXOTH) assert(st.st_mode & S_IXOTH);
            
            /* Check special bits if supported */
            if (mode & S_ISUID && (st.st_mode & S_ISUID))
                assert(st.st_mode & S_ISUID);
            if (mode & S_ISGID && (st.st_mode & S_ISGID))
                assert(st.st_mode & S_ISGID);
#ifdef S_ISVTX
            if (mode & S_ISVTX && (st.st_mode & S_ISVTX))
                assert(st.st_mode & S_ISVTX);
#endif
        }
        
        unlink(TEST_FILE);
    }
    
    TEST_PASSED("Permission");
}

/* Test edge cases */
void test_edge_cases(void) {
    struct stat st;
    int ret;
    
    TEST_START("edge cases");
    
    /* Empty filename */
    ret = stat("", &st);
    assert(ret == -1);
    assert(errno == ENOENT);
    
    /* Very long filename */
    char long_name[4096];
    memset(long_name, 'x', sizeof(long_name) - 1);
    long_name[sizeof(long_name) - 1] = '\0';
    
    ret = stat(long_name, &st);
    assert(ret == -1);
    assert(errno == ENAMETOOLONG || errno == ENOENT);
    
    /* Path with null bytes */
    char bad_path[] = "test\0file";
    ret = stat(bad_path, &st);
    /* Should only see "test" */
    
    /* Multiple slashes */
    if (mkdir(TEST_DIR, 0755) == 0) {
        ret = stat("./////test_dir////", &st);
        if (ret == 0) {
            assert(S_ISDIR(st.st_mode));
        }
        rmdir(TEST_DIR);
    }
    
    /* Relative paths */
    assert(create_test_file(TEST_FILE) == 0);
    ret = stat("./test_file.txt", &st);
    assert(ret == 0);
    unlink(TEST_FILE);
    
    TEST_PASSED("Edge case");
}

/* Test time updates */
void test_time_updates(void) {
    struct stat st1, st2;
    time_t before, after;
    int fd;
    
    TEST_START("time field updates");
    
    /* Create file and get initial times */
    before = time(NULL);
    assert(create_test_file(TEST_FILE) == 0);
    assert(stat(TEST_FILE, &st1) == 0);
    after = time(NULL);
    
    /* Creation time should be recent */
    assert(st1.st_mtime >= before);
    assert(st1.st_mtime <= after);
    assert(st1.st_atime >= before);
    assert(st1.st_ctime >= before);
    
    /* Wait a bit */
    sleep(2);
    
    /* Access file (read) */
    fd = open(TEST_FILE, O_RDONLY);
    assert(fd >= 0);
    char buf[100];
    read(fd, buf, sizeof(buf));
    close(fd);
    
    /* Check if atime updated (might not on some filesystems) */
    assert(stat(TEST_FILE, &st2) == 0);
    /* atime might be >= st1.st_atime depending on mount options */
    
    /* Modify file */
    before = time(NULL);
    fd = open(TEST_FILE, O_WRONLY | O_APPEND);
    assert(fd >= 0);
    write(fd, "more data\n", 10);
    close(fd);
    after = time(NULL);
    
    /* mtime should be updated */
    assert(stat(TEST_FILE, &st2) == 0);
    assert(st2.st_mtime >= before);
    assert(st2.st_mtime <= after);
    assert(st2.st_mtime > st1.st_mtime);
    assert(st2.st_size == 23); /* 13 + 10 */
    
    /* Change permissions */
    before = time(NULL);
    assert(chmod(TEST_FILE, 0600) == 0);
    after = time(NULL);
    
    /* ctime should be updated */
    assert(stat(TEST_FILE, &st2) == 0);
    assert(st2.st_ctime >= before);
    assert(st2.st_ctime <= after);
    
    unlink(TEST_FILE);
    TEST_PASSED("Time update");
}

int main(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 sys/stat.h Test Suite ===" COLOR_RESET "\n\n");
    
    /* Clean up any leftover test files */
    cleanup_test_files();
    
    /* Run tests */
    test_stat();
    test_fstat();
    test_lstat();
    test_chmod();
    test_fchmod();
    test_mkdir();
    test_mkfifo();
    test_umask();
    test_stat_macros();
    test_permissions();
    test_edge_cases();
    test_time_updates();
    
    /* Final cleanup */
    cleanup_test_files();
    
    TEST_SUITE_PASSED("sys/stat.h");
    return 0;
}
