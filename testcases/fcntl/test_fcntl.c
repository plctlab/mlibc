/*
 * test_fcntl.c - PSE51 fcntl.h file control test suite
 * Tests: fcntl, open, creat
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for file control operations
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../test_colors.h"
#define TEST_FILE "test_fcntl.tmp"
#define TEST_FILE2 "test_fcntl2.tmp"
#define TEST_DIR "test_fcntl_dir"

/* Clean up test files */
void cleanup_files(void) {
    unlink(TEST_FILE);
    unlink(TEST_FILE2);
    rmdir(TEST_DIR);
}

/* Test open() function */
void test_open(void) {
    int fd;
    char buf[100];
    ssize_t n;
    
    TEST_START("open()");
    
    /* Clean up first */
    cleanup_files();
    
    /* Test creating new file with O_CREAT */
    fd = open(TEST_FILE, O_CREAT | O_WRONLY, 0644);
    assert(fd >= 0);
    close(fd);
    
    /* Verify file exists */
    fd = open(TEST_FILE, O_RDONLY);
    assert(fd >= 0);
    close(fd);
    
    /* Test O_EXCL - should fail if file exists */
    fd = open(TEST_FILE, O_CREAT | O_EXCL | O_WRONLY, 0644);
    assert(fd == -1);
    assert(errno == EEXIST);
    
    /* Test O_TRUNC */
    fd = open(TEST_FILE, O_WRONLY | O_TRUNC);
    assert(fd >= 0);
    write(fd, "Hello", 5);
    close(fd);
    
    /* Verify truncation */
    struct stat st;
    assert(stat(TEST_FILE, &st) == 0);
    assert(st.st_size == 5);
    
    /* Test O_APPEND */
    fd = open(TEST_FILE, O_WRONLY | O_APPEND);
    assert(fd >= 0);
    write(fd, " World", 6);
    close(fd);
    
    /* Verify append */
    fd = open(TEST_FILE, O_RDONLY);
    assert(fd >= 0);
    n = read(fd, buf, sizeof(buf));
    assert(n == 11);
    buf[n] = '\0';
    assert(strcmp(buf, "Hello World") == 0);
    close(fd);
    
    /* Test read-write mode */
    fd = open(TEST_FILE, O_RDWR);
    assert(fd >= 0);
    
    /* Read */
    n = read(fd, buf, 5);
    assert(n == 5);
    
    /* Write at current position */
    n = write(fd, "TEST", 4);
    assert(n == 4);
    
    /* Seek and read to verify */
    lseek(fd, 0, SEEK_SET);
    n = read(fd, buf, sizeof(buf));
    assert(n == 11);
    buf[n] = '\0';
    assert(strcmp(buf, "HelloTESTld") == 0);
    close(fd);
    
    /* Test opening non-existent file without O_CREAT */
    unlink(TEST_FILE);
    fd = open(TEST_FILE, O_RDONLY);
    assert(fd == -1);
    assert(errno == ENOENT);
    
    /* Test opening directory */
    mkdir(TEST_DIR, 0755);
    fd = open(TEST_DIR, O_RDONLY);
    if (fd >= 0) {
        /* Some systems allow opening directories */
        close(fd);
    }
    
    /* Test invalid flags */
    fd = open(TEST_FILE, O_RDONLY | O_WRONLY); /* Both read and write */
    assert(fd == -1);
    assert(errno == EINVAL || errno == ENOENT);
    
    /* Test mode with O_CREAT */
    fd = open(TEST_FILE, O_CREAT | O_WRONLY, 0600);
    assert(fd >= 0);
    close(fd);
    
    /* Verify permissions */
    assert(stat(TEST_FILE, &st) == 0);
    assert((st.st_mode & 0777) == 0600);
    
    /* Test O_NONBLOCK (if supported) */
    fd = open(TEST_FILE, O_RDONLY | O_NONBLOCK);
    if (fd >= 0) {
        /* O_NONBLOCK supported */
        close(fd);
    }
    
    cleanup_files();
    TEST_PASSED("open()");
}

/* Test creat() function */
void test_creat(void) {
    int fd;
    struct stat st;
    
    TEST_START("creat()");
    
    /* Clean up first */
    cleanup_files();
    
    /* Test basic file creation */
    fd = creat(TEST_FILE, 0644);
    assert(fd >= 0);
    
    /* creat() opens for writing only */
    assert(write(fd, "test", 4) == 4);
    close(fd);
    
    /* Verify file exists with correct permissions */
    assert(stat(TEST_FILE, &st) == 0);
    assert((st.st_mode & 0777) == 0644);
    assert(st.st_size == 4);
    
    /* Test creat on existing file (should truncate) */
    fd = creat(TEST_FILE, 0600);
    assert(fd >= 0);
    close(fd);
    
    /* Verify truncation and permission change */
    assert(stat(TEST_FILE, &st) == 0);
    assert((st.st_mode & 0777) == 0600);
    assert(st.st_size == 0);
    
    /* Test creat with various permissions */
    unlink(TEST_FILE);
    
    fd = creat(TEST_FILE, 0777);
    assert(fd >= 0);
    close(fd);
    assert(stat(TEST_FILE, &st) == 0);
    assert((st.st_mode & 0777) == 0777);
    
    unlink(TEST_FILE);
    fd = creat(TEST_FILE, 0000);
    assert(fd >= 0);
    close(fd);
    assert(stat(TEST_FILE, &st) == 0);
    assert((st.st_mode & 0777) == 0000);
    
    /* Test error cases */
    /* Create in non-existent directory */
    fd = creat("/nonexistent/dir/file", 0644);
    assert(fd == -1);
    assert(errno == ENOENT);
    
    cleanup_files();
    TEST_PASSED("creat()");
}

/* Test fcntl() function */
void test_fcntl(void) {
    int fd, fd2;
    int flags;
    int ret;
    struct flock fl;
    
    TEST_START("fcntl()");
    
    /* Create test file */
    fd = open(TEST_FILE, O_CREAT | O_RDWR, 0644);
    assert(fd >= 0);
    write(fd, "test data", 9);
    
    /* Test F_DUPFD - duplicate file descriptor */
    fd2 = fcntl(fd, F_DUPFD, 10);
    if (fd2 == -1 && errno == ENOSYS) {
        printf("  fcntl() not implemented (ENOSYS)\n");
        close(fd);
        cleanup_files();
        return;
    }
    assert(fd2 >= 10);
    
    /* Verify duplicate works */
    char buf[10];
    lseek(fd2, 0, SEEK_SET);
    assert(read(fd2, buf, 9) == 9);
    buf[9] = '\0';
    assert(strcmp(buf, "test data") == 0);
    close(fd2);
    
    /* Test F_GETFD/F_SETFD - file descriptor flags */
    flags = fcntl(fd, F_GETFD);
    assert(flags >= 0);
    
    /* Set FD_CLOEXEC */
    ret = fcntl(fd, F_SETFD, FD_CLOEXEC);
    assert(ret == 0);
    
    /* Verify it was set */
    flags = fcntl(fd, F_GETFD);
    assert(flags == FD_CLOEXEC);
    
    /* Clear FD_CLOEXEC */
    ret = fcntl(fd, F_SETFD, 0);
    assert(ret == 0);
    
    flags = fcntl(fd, F_GETFD);
    assert(flags == 0);
    
    /* Test F_GETFL/F_SETFL - file status flags */
    flags = fcntl(fd, F_GETFL);
    assert(flags >= 0);
    assert((flags & O_ACCMODE) == O_RDWR);
    
    /* Add O_APPEND */
    ret = fcntl(fd, F_SETFL, flags | O_APPEND);
    assert(ret == 0);
    
    /* Verify O_APPEND is set */
    flags = fcntl(fd, F_GETFL);
    assert(flags & O_APPEND);
    
    /* Test that writes append */
    lseek(fd, 0, SEEK_SET);
    write(fd, "X", 1); /* Should append despite seek */
    
    lseek(fd, 0, SEEK_SET);
    char buf2[20];
    int n = read(fd, buf2, sizeof(buf2));
    assert(n == 10);
    buf2[n] = '\0';
    assert(strcmp(buf2, "test dataX") == 0);
    
    /* Clear O_APPEND */
    flags &= ~O_APPEND;
    ret = fcntl(fd, F_SETFL, flags);
    assert(ret == 0);
    
    /* Test F_GETLK/F_SETLK/F_SETLKW - record locking */
    memset(&fl, 0, sizeof(fl));
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 10;
    
    /* Try to set write lock */
    ret = fcntl(fd, F_SETLK, &fl);
    if (ret == 0) {
        /* Lock succeeded */
        
        /* Test F_GETLK from another fd */
        int fd3 = open(TEST_FILE, O_RDWR);
        assert(fd3 >= 0);
        
        struct flock fl2;
        memset(&fl2, 0, sizeof(fl2));
        fl2.l_type = F_WRLCK;
        fl2.l_whence = SEEK_SET;
        fl2.l_start = 0;
        fl2.l_len = 5;
        
        ret = fcntl(fd3, F_GETLK, &fl2);
        assert(ret == 0);
        /* fl2.l_type should be F_WRLCK if locked */
        /* fl2.l_pid should be our PID */
        
        /* Try to set conflicting lock (should fail) */
        ret = fcntl(fd3, F_SETLK, &fl2);
        assert(ret == -1);
        assert(errno == EAGAIN || errno == EACCES);
        
        close(fd3);
        
        /* Release lock */
        fl.l_type = F_UNLCK;
        ret = fcntl(fd, F_SETLK, &fl);
        assert(ret == 0);
    }
    
    /* Test with invalid fd */
    ret = fcntl(-1, F_GETFD);
    assert(ret == -1);
    assert(errno == EBADF);
    
    ret = fcntl(9999, F_GETFD);
    assert(ret == -1);
    assert(errno == EBADF);
    
    close(fd);
    cleanup_files();
    TEST_PASSED("fcntl()");
}

/* Test open flags combinations */
void test_open_flags(void) {
    int fd;
    
    TEST_START("open() flag combinations");
    
    cleanup_files();
    
    /* Test O_CREAT with O_RDONLY */
    fd = open(TEST_FILE, O_CREAT | O_RDONLY, 0644);
    assert(fd >= 0);
    
    /* Should not be able to write */
    assert(write(fd, "test", 4) == -1);
    assert(errno == EBADF);
    close(fd);
    
    /* Test O_CREAT with O_WRONLY */
    unlink(TEST_FILE);
    fd = open(TEST_FILE, O_CREAT | O_WRONLY, 0644);
    assert(fd >= 0);
    
    /* Should not be able to read */
    char buf[10];
    assert(read(fd, buf, 10) == -1);
    assert(errno == EBADF);
    
    /* Should be able to write */
    assert(write(fd, "test", 4) == 4);
    close(fd);
    
    /* Test O_TRUNC with O_RDONLY (should fail) */
    fd = open(TEST_FILE, O_RDONLY | O_TRUNC);
    if (fd == -1) {
        /* Some implementations don't allow this */
        assert(errno == EINVAL || errno == EACCES);
    } else {
        close(fd);
    }
    
    /* Test multiple opens */
    fd = open(TEST_FILE, O_RDONLY);
    assert(fd >= 0);
    
    int fd2 = open(TEST_FILE, O_RDONLY);
    assert(fd2 >= 0);
    assert(fd2 != fd);
    
    close(fd);
    close(fd2);
    
    cleanup_files();
    TEST_PASSED("Open flags");
}

/* Test file locking patterns */
void test_locking_patterns(void) {
    int fd;
    struct flock fl;
    int ret;
    
    TEST_START("file locking patterns");
    
    fd = open(TEST_FILE, O_CREAT | O_RDWR, 0644);
    assert(fd >= 0);
    write(fd, "0123456789", 10);
    
    /* Pattern 1: Exclusive write lock */
    memset(&fl, 0, sizeof(fl));
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0; /* Entire file */
    
    ret = fcntl(fd, F_SETLK, &fl);
    if (ret == -1 && errno == ENOSYS) {
        printf("  File locking not supported\n");
        close(fd);
        cleanup_files();
        return;
    }
    
    /* Pattern 2: Partial read lock */
    fl.l_type = F_RDLCK;
    fl.l_start = 5;
    fl.l_len = 5;
    
    ret = fcntl(fd, F_SETLK, &fl);
    /* May fail if write lock is held */
    
    /* Pattern 3: Non-blocking lock attempt */
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_len = 1;
    
    ret = fcntl(fd, F_SETLK, &fl);
    /* Returns immediately with error if can't lock */
    
    /* Pattern 4: Blocking lock (F_SETLKW) */
    /* Don't test blocking in automated tests */
    
    /* Release all locks */
    fl.l_type = F_UNLCK;
    fl.l_start = 0;
    fl.l_len = 0;
    fcntl(fd, F_SETLK, &fl);
    
    close(fd);
    cleanup_files();
    TEST_PASSED("Locking pattern");
}

/* Test edge cases */
void test_edge_cases(void) {
    int fd;
    
    TEST_START("edge cases");
    
    /* Test opening with empty filename */
    fd = open("", O_RDONLY);
    assert(fd == -1);
    assert(errno == ENOENT);
    
    /* Test opening with very long filename */
    char longname[1024];
    memset(longname, 'a', sizeof(longname) - 1);
    longname[sizeof(longname) - 1] = '\0';
    
    fd = open(longname, O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        assert(errno == ENAMETOOLONG);
    } else {
        close(fd);
        unlink(longname);
    }
    
    /* Test creat with umask */
    mode_t old_umask = umask(0077);
    fd = creat(TEST_FILE, 0777);
    assert(fd >= 0);
    close(fd);
    
    struct stat st;
    assert(stat(TEST_FILE, &st) == 0);
    assert((st.st_mode & 0777) == 0700); /* 0777 & ~0077 */
    
    umask(old_umask);
    
    cleanup_files();
    TEST_PASSED("Edge case");
}

int main(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 fcntl.h Test Suite ===" COLOR_RESET "\n\n");
    
    /* Run tests */
    test_open();
    test_creat();
    test_fcntl();
    test_open_flags();
    test_locking_patterns();
    test_edge_cases();
    
    /* Final cleanup */
    cleanup_files();
    
    TEST_SUITE_PASSED("fcntl.h");
    return 0;
}
