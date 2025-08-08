/*
 * test_unistd.c - PSE51 unistd.h system call test suite
 * Tests: access, alarm, chdir, close, dup, dup2, execve, _exit, fork,
 *        getcwd, getegid, geteuid, getgid, getpid, getppid, getuid,
 *        link, lseek, pathconf, pause, pipe, read, rmdir, sleep,
 *        sysconf, unlink, write
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for system calls
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <limits.h>

#include "../test_colors.h"
#define TEST_FILE "test_unistd.tmp"
#define TEST_FILE2 "test_unistd2.tmp"
#define TEST_DIR "test_unistd_dir"
#define TEST_LINK "test_link.tmp"
#define TEST_FIFO "test_fifo.tmp"

/* Clean up test files */
void cleanup_files(void) {
    unlink(TEST_FILE);
    unlink(TEST_FILE2);
    unlink(TEST_LINK);
    unlink(TEST_FIFO);
    rmdir(TEST_DIR);
}

/* Test read() and write() functions */
void test_read_write(void) {
    int fd;
    char write_buf[] = "Hello, World!";
    char read_buf[100];
    ssize_t n;
    
    TEST_START("read() and write()");
    
    /* Create test file */
    fd = open(TEST_FILE, O_CREAT | O_RDWR | O_TRUNC, 0644);
    assert(fd >= 0);
    
    /* Test write */
    n = write(fd, write_buf, strlen(write_buf));
    assert(n == (ssize_t)strlen(write_buf));
    
    /* Test lseek to beginning */
    off_t pos = lseek(fd, 0, SEEK_SET);
    assert(pos == 0);
    
    /* Test read */
    memset(read_buf, 0, sizeof(read_buf));
    n = read(fd, read_buf, sizeof(read_buf));
    assert(n == (ssize_t)strlen(write_buf));
    assert(strcmp(read_buf, write_buf) == 0);
    
    /* Test partial read */
    lseek(fd, 0, SEEK_SET);
    n = read(fd, read_buf, 5);
    assert(n == 5);
    assert(strncmp(read_buf, "Hello", 5) == 0);
    
    /* Test EOF */
    lseek(fd, 0, SEEK_END);
    n = read(fd, read_buf, 10);
    assert(n == 0); /* EOF */
    
    /* Test write with invalid fd */
    n = write(-1, "test", 4);
    assert(n == -1);
    assert(errno == EBADF);
    
    /* Test read with invalid fd */
    n = read(-1, read_buf, 10);
    assert(n == -1);
    assert(errno == EBADF);
    
    close(fd);
    unlink(TEST_FILE);
    TEST_PASSED("read() and write()");
}

/* Test lseek() function */
void test_lseek(void) {
    int fd;
    off_t pos;
    char buf[100] = "0123456789";
    
    TEST_START("lseek()");
    
    fd = open(TEST_FILE, O_CREAT | O_RDWR | O_TRUNC, 0644);
    assert(fd >= 0);
    
    /* Write test data */
    write(fd, buf, 10);
    
    /* Test SEEK_SET */
    pos = lseek(fd, 5, SEEK_SET);
    assert(pos == 5);
    
    /* Verify position */
    read(fd, buf, 1);
    assert(buf[0] == '5');
    
    /* Test SEEK_CUR */
    pos = lseek(fd, 2, SEEK_CUR);
    assert(pos == 8);
    
    read(fd, buf, 1);
    assert(buf[0] == '8');
    
    /* Test SEEK_END */
    pos = lseek(fd, -3, SEEK_END);
    assert(pos == 7);
    
    read(fd, buf, 1);
    assert(buf[0] == '7');
    
    /* Test seeking past EOF */
    pos = lseek(fd, 20, SEEK_SET);
    assert(pos == 20);
    
    /* Write at new position creates hole */
    write(fd, "X", 1);
    
    /* Verify file size */
    pos = lseek(fd, 0, SEEK_END);
    assert(pos == 21);
    
    /* Test negative offset with SEEK_SET (should fail) */
    pos = lseek(fd, -5, SEEK_SET);
    assert(pos == -1);
    assert(errno == EINVAL);
    
    /* Test with pipe (should fail) */
    int pipefd[2];
    if (pipe(pipefd) == 0) {
        pos = lseek(pipefd[0], 0, SEEK_CUR);
        assert(pos == -1);
        assert(errno == ESPIPE);
        close(pipefd[0]);
        close(pipefd[1]);
    }
    
    close(fd);
    unlink(TEST_FILE);
    TEST_PASSED("lseek()");
}

/* Test close() function */
void test_close(void) {
    int fd;
    int ret;
    
    TEST_START("close()");
    
    /* Test normal close */
    fd = open(TEST_FILE, O_CREAT | O_RDWR, 0644);
    assert(fd >= 0);
    
    ret = close(fd);
    assert(ret == 0);
    
    /* Test double close (should fail) */
    ret = close(fd);
    assert(ret == -1);
    assert(errno == EBADF);
    
    /* Test invalid fd */
    ret = close(-1);
    assert(ret == -1);
    assert(errno == EBADF);
    
    ret = close(9999);
    assert(ret == -1);
    assert(errno == EBADF);
    
    unlink(TEST_FILE);
    TEST_PASSED("close()");
}

/* Test dup() and dup2() functions */
void test_dup_dup2(void) {
    int fd, fd2, fd3;
    char buf[100];
    ssize_t n;
    
    TEST_START("dup() and dup2()");
    
    /* Create test file */
    fd = open(TEST_FILE, O_CREAT | O_RDWR | O_TRUNC, 0644);
    assert(fd >= 0);
    write(fd, "test", 4);
    
    /* Test dup() */
    fd2 = dup(fd);
    assert(fd2 >= 0);
    assert(fd2 != fd);
    
    /* Both fds share file offset */
    lseek(fd, 0, SEEK_SET);
    read(fd, buf, 2);
    
    n = read(fd2, buf, 2);
    assert(n == 2);
    assert(strncmp(buf, "st", 2) == 0); /* Continued from fd's position */
    
    /* Test dup2() */
    fd3 = dup2(fd, 100);
    assert(fd3 == 100);
    
    /* Test dup2 with same fd */
    fd3 = dup2(fd, fd);
    assert(fd3 == fd);
    
    /* Test dup2 closing existing fd */
    int fd4 = open(TEST_FILE2, O_CREAT | O_WRONLY, 0644);
    assert(fd4 >= 0);
    
    fd3 = dup2(fd, fd4);
    assert(fd3 == fd4);
    
    /* Original fd4 is closed, can't write to TEST_FILE2 */
    
    /* Test with invalid fd */
    fd3 = dup(-1);
    assert(fd3 == -1);
    assert(errno == EBADF);
    
    fd3 = dup2(-1, 10);
    assert(fd3 == -1);
    assert(errno == EBADF);
    
    close(fd);
    close(fd2);
    close(fd3);
    unlink(TEST_FILE);
    unlink(TEST_FILE2);
    TEST_PASSED("dup() and dup2()");
}

/* Test access() function */
void test_access(void) {
    int ret;
    
    TEST_START("access()");
    
    /* Create test file */
    int fd = open(TEST_FILE, O_CREAT | O_WRONLY, 0644);
    assert(fd >= 0);
    close(fd);
    
    /* Test F_OK (existence) */
    ret = access(TEST_FILE, F_OK);
    assert(ret == 0);
    
    /* Test R_OK (read permission) */
    ret = access(TEST_FILE, R_OK);
    assert(ret == 0);
    
    /* Test W_OK (write permission) */
    ret = access(TEST_FILE, W_OK);
    assert(ret == 0);
    
    /* Test X_OK (execute permission) */
    ret = access(TEST_FILE, X_OK);
    assert(ret == -1); /* No execute permission */
    assert(errno == EACCES);
    
    /* Test multiple permissions */
    ret = access(TEST_FILE, R_OK | W_OK);
    assert(ret == 0);
    
    /* Test non-existent file */
    ret = access("nonexistent.file", F_OK);
    assert(ret == -1);
    assert(errno == ENOENT);
    
    /* Make file executable */
    chmod(TEST_FILE, 0755);
    ret = access(TEST_FILE, X_OK);
    assert(ret == 0);
    
    /* Test directory */
    mkdir(TEST_DIR, 0755);
    ret = access(TEST_DIR, F_OK);
    assert(ret == 0);
    
    ret = access(TEST_DIR, R_OK | X_OK);
    assert(ret == 0);
    
    rmdir(TEST_DIR);
    unlink(TEST_FILE);
    TEST_PASSED("access()");
}

/* Test unlink() function */
void test_unlink(void) {
    int ret;
    int fd;
    
    TEST_START("unlink()");
    
    /* Create test file */
    fd = open(TEST_FILE, O_CREAT | O_WRONLY, 0644);
    assert(fd >= 0);
    write(fd, "test", 4);
    close(fd);
    
    /* Verify file exists */
    assert(access(TEST_FILE, F_OK) == 0);
    
    /* Test unlink */
    ret = unlink(TEST_FILE);
    assert(ret == 0);
    
    /* Verify file is gone */
    assert(access(TEST_FILE, F_OK) == -1);
    assert(errno == ENOENT);
    
    /* Test unlinking non-existent file */
    ret = unlink(TEST_FILE);
    assert(ret == -1);
    assert(errno == ENOENT);
    
    /* Test unlinking open file */
    fd = open(TEST_FILE, O_CREAT | O_RDWR, 0644);
    assert(fd >= 0);
    write(fd, "test", 4);
    
    ret = unlink(TEST_FILE);
    assert(ret == 0);
    
    /* File is unlinked but still accessible through fd */
    lseek(fd, 0, SEEK_SET);
    char buf[10];
    assert(read(fd, buf, 4) == 4);
    
    close(fd);
    /* Now file is really gone */
    
    /* Test unlinking directory (should fail) */
    mkdir(TEST_DIR, 0755);
    ret = unlink(TEST_DIR);
    assert(ret == -1);
    assert(errno == EISDIR || errno == EPERM);
    
    rmdir(TEST_DIR);
    TEST_PASSED("unlink()");
}

/* Test link() function */
void test_link(void) {
    int ret;
    struct stat st1, st2;
    
    TEST_START("link()");
    
    /* Create source file */
    int fd = open(TEST_FILE, O_CREAT | O_WRONLY, 0644);
    assert(fd >= 0);
    write(fd, "test data", 9);
    close(fd);
    
    /* Create hard link */
    ret = link(TEST_FILE, TEST_LINK);
    if (ret == -1 && errno == ENOSYS) {
        printf("  link() not implemented (ENOSYS)\n");
        unlink(TEST_FILE);
        return;
    }
    assert(ret == 0);
    
    /* Verify both files exist */
    assert(access(TEST_FILE, F_OK) == 0);
    assert(access(TEST_LINK, F_OK) == 0);
    
    /* Verify they're the same file (same inode) */
    assert(stat(TEST_FILE, &st1) == 0);
    assert(stat(TEST_LINK, &st2) == 0);
    assert(st1.st_ino == st2.st_ino);
    assert(st1.st_nlink == 2);
    
    /* Verify content is the same */
    fd = open(TEST_LINK, O_RDONLY);
    assert(fd >= 0);
    char buf[20];
    assert(read(fd, buf, 9) == 9);
    buf[9] = '\0';
    assert(strcmp(buf, "test data") == 0);
    close(fd);
    
    /* Test linking to existing file (should fail) */
    ret = link(TEST_FILE, TEST_LINK);
    assert(ret == -1);
    assert(errno == EEXIST);
    
    /* Remove one link */
    ret = unlink(TEST_FILE);
    assert(ret == 0);
    
    /* Other link still exists */
    assert(access(TEST_LINK, F_OK) == 0);
    assert(stat(TEST_LINK, &st1) == 0);
    assert(st1.st_nlink == 1);
    
    /* Test linking non-existent file */
    ret = link("nonexistent", "newlink");
    assert(ret == -1);
    assert(errno == ENOENT);
    
    /* Test linking directory (should fail) */
    mkdir(TEST_DIR, 0755);
    ret = link(TEST_DIR, "dirlink");
    assert(ret == -1);
    assert(errno == EPERM || errno == EISDIR);
    
    rmdir(TEST_DIR);
    unlink(TEST_LINK);
    TEST_PASSED("link()");
}

/* Test rmdir() function */
void test_rmdir(void) {
    int ret;
    
    TEST_START("rmdir()");
    
    /* Create directory */
    ret = mkdir(TEST_DIR, 0755);
    assert(ret == 0);
    
    /* Verify it exists */
    assert(access(TEST_DIR, F_OK) == 0);
    
    /* Test rmdir */
    ret = rmdir(TEST_DIR);
    assert(ret == 0);
    
    /* Verify it's gone */
    assert(access(TEST_DIR, F_OK) == -1);
    assert(errno == ENOENT);
    
    /* Test removing non-existent directory */
    ret = rmdir(TEST_DIR);
    assert(ret == -1);
    assert(errno == ENOENT);
    
    /* Test removing non-empty directory */
    mkdir(TEST_DIR, 0755);
    int fd = open(TEST_DIR "/file", O_CREAT | O_WRONLY, 0644);
    assert(fd >= 0);
    close(fd);
    
    ret = rmdir(TEST_DIR);
    assert(ret == -1);
    assert(errno == ENOTEMPTY || errno == EEXIST);
    
    /* Clean up */
    unlink(TEST_DIR "/file");
    rmdir(TEST_DIR);
    
    /* Test removing regular file (should fail) */
    fd = open(TEST_FILE, O_CREAT | O_WRONLY, 0644);
    assert(fd >= 0);
    close(fd);
    
    ret = rmdir(TEST_FILE);
    assert(ret == -1);
    assert(errno == ENOTDIR);
    
    unlink(TEST_FILE);
    TEST_PASSED("rmdir()");
}

/* Test chdir() and getcwd() functions */
void test_chdir_getcwd(void) {
    char cwd[PATH_MAX];
    char original_cwd[PATH_MAX];
    char *ret_str;
    int ret;
    
    TEST_START("chdir() and getcwd()");
    
    /* Save original directory */
    ret_str = getcwd(original_cwd, sizeof(original_cwd));
    assert(ret_str != NULL);
    assert(ret_str == original_cwd);
    
    /* Create test directory */
    mkdir(TEST_DIR, 0755);
    
    /* Change to test directory */
    ret = chdir(TEST_DIR);
    assert(ret == 0);
    
    /* Verify we're in new directory */
    ret_str = getcwd(cwd, sizeof(cwd));
    assert(ret_str != NULL);
    assert(strlen(cwd) > strlen(original_cwd));
    assert(strstr(cwd, TEST_DIR) != NULL);
    
    /* Test getcwd with NULL buffer (GNU extension) */
    ret_str = getcwd(NULL, 0);
    if (ret_str != NULL) {
        /* Dynamically allocated */
        assert(strstr(ret_str, TEST_DIR) != NULL);
        free(ret_str);
    }
    
    /* Test getcwd with small buffer */
    ret_str = getcwd(cwd, 5);
    assert(ret_str == NULL);
    assert(errno == ERANGE);
    
    /* Change back to original directory */
    ret = chdir(original_cwd);
    assert(ret == 0);
    
    /* Verify we're back */
    ret_str = getcwd(cwd, sizeof(cwd));
    assert(ret_str != NULL);
    assert(strcmp(cwd, original_cwd) == 0);
    
    /* Test chdir to non-existent directory */
    ret = chdir("/nonexistent/directory");
    assert(ret == -1);
    assert(errno == ENOENT);
    
    /* Test chdir to file (should fail) */
    int fd = open(TEST_FILE, O_CREAT | O_WRONLY, 0644);
    assert(fd >= 0);
    close(fd);
    
    ret = chdir(TEST_FILE);
    assert(ret == -1);
    assert(errno == ENOTDIR);
    
    unlink(TEST_FILE);
    rmdir(TEST_DIR);
    TEST_PASSED("chdir() and getcwd()");
}

/* Test pipe() function */
void test_pipe(void) {
    int pipefd[2];
    int ret;
    char write_buf[] = "Hello pipe!";
    char read_buf[100];
    ssize_t n;
    
    TEST_START("pipe()");
    
    /* Create pipe */
    ret = pipe(pipefd);
    assert(ret == 0);
    assert(pipefd[0] >= 0);
    assert(pipefd[1] >= 0);
    assert(pipefd[0] != pipefd[1]);
    
    /* Write to pipe */
    n = write(pipefd[1], write_buf, strlen(write_buf));
    assert(n == (ssize_t)strlen(write_buf));
    
    /* Read from pipe */
    memset(read_buf, 0, sizeof(read_buf));
    n = read(pipefd[0], read_buf, sizeof(read_buf));
    assert(n == (ssize_t)strlen(write_buf));
    assert(strcmp(read_buf, write_buf) == 0);
    
    /* Test non-blocking read on empty pipe */
    fcntl(pipefd[0], F_SETFL, O_NONBLOCK);
    n = read(pipefd[0], read_buf, sizeof(read_buf));
    assert(n == -1);
    assert(errno == EAGAIN || errno == EWOULDBLOCK);
    
    /* Test closing write end */
    close(pipefd[1]);
    n = read(pipefd[0], read_buf, sizeof(read_buf));
    assert(n == 0); /* EOF */
    
    close(pipefd[0]);
    
    /* Test multiple pipes */
    int pipe1[2], pipe2[2];
    assert(pipe(pipe1) == 0);
    assert(pipe(pipe2) == 0);
    
    /* Pipes should be independent */
    write(pipe1[1], "pipe1", 5);
    write(pipe2[1], "pipe2", 5);
    
    memset(read_buf, 0, sizeof(read_buf));
    read(pipe2[0], read_buf, 5);
    assert(strcmp(read_buf, "pipe2") == 0);
    
    memset(read_buf, 0, sizeof(read_buf));
    read(pipe1[0], read_buf, 5);
    assert(strcmp(read_buf, "pipe1") == 0);
    
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);
    
    TEST_PASSED("pipe()");
}

/* Test process ID functions */
void test_process_ids(void) {
    pid_t pid, ppid;
    uid_t uid, euid;
    gid_t gid, egid;
    
    TEST_START("process ID functions");
    
    /* Test getpid() */
    pid = getpid();
    assert(pid > 0);
    
    /* PID should be consistent */
    assert(getpid() == pid);
    
    /* Test getppid() */
    ppid = getppid();
    assert(ppid > 0);
    assert(ppid != pid); /* Parent should be different */
    
    /* Test getuid() and geteuid() */
    uid = getuid();
    euid = geteuid();
    assert(uid >= 0);
    assert(euid >= 0);
    
    /* Test getgid() and getegid() */
    gid = getgid();
    egid = getegid();
    assert(gid >= 0);
    assert(egid >= 0);
    
    printf("  Process IDs: pid=%d, ppid=%d, uid=%d, euid=%d, gid=%d, egid=%d\n",
           pid, ppid, uid, euid, gid, egid);
    
    TEST_PASSED("Process ID");
}

/* Test fork() and _exit() functions */
void test_fork_exit(void) {
    pid_t pid;
    int status;
    
    TEST_START("fork() and _exit()");
    
    /* Test basic fork */
    pid = fork();
    
    if (pid == -1) {
        if (errno == ENOSYS) {
            printf("  fork() not implemented (ENOSYS)\n");
            return;
        }
        assert(0); /* Unexpected error */
    }
    
    if (pid == 0) {
        /* Child process */
        /* Verify we're the child */
        assert(getppid() > 0);
        
        /* Exit with specific code */
        _exit(42);
    }
    
    /* Parent process */
    assert(pid > 0);
    
    /* Wait for child */
    pid_t waited = waitpid(pid, &status, 0);
    assert(waited == pid);
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 42);
    
    /* Test fork with shared file descriptors */
    int fd = open(TEST_FILE, O_CREAT | O_RDWR | O_TRUNC, 0644);
    assert(fd >= 0);
    
    pid = fork();
    assert(pid >= 0);
    
    if (pid == 0) {
        /* Child writes to file */
        write(fd, "child", 5);
        close(fd);
        _exit(0);
    }
    
    /* Parent writes to file */
    write(fd, "parent", 6);
    
    /* Wait for child */
    waitpid(pid, &status, 0);
    
    /* Check file content */
    lseek(fd, 0, SEEK_SET);
    char buf[20];
    int n = read(fd, buf, sizeof(buf));
    buf[n] = '\0';
    
    /* Both parent and child wrote */
    assert(n == 11);
    assert(strstr(buf, "parent") != NULL);
    assert(strstr(buf, "child") != NULL);
    
    close(fd);
    unlink(TEST_FILE);
    
    TEST_PASSED("fork() and _exit()");
}

/* Test execve() function */
void test_execve(void) {
    pid_t pid;
    int status;
    
    TEST_START("execve()");
    
    pid = fork();
    if (pid == -1) {
        if (errno == ENOSYS) {
            printf("  execve() test skipped (fork not available)\n");
            return;
        }
    }
    
    if (pid == 0) {
        /* Child process */
        char *argv[] = {"/bin/true", NULL};
        char *envp[] = {NULL};
        
        execve("/bin/true", argv, envp);
        
        /* If we get here, execve failed */
        if (errno == ENOENT) {
            /* /bin/true doesn't exist, try /usr/bin/true */
            argv[0] = "/usr/bin/true";
            execve("/usr/bin/true", argv, envp);
        }
        
        /* Still failed */
        _exit(errno);
    }
    
    /* Parent waits */
    waitpid(pid, &status, 0);
    
    if (WIFEXITED(status)) {
        int code = WEXITSTATUS(status);
        if (code == ENOENT) {
            printf("  execve() test skipped (true command not found)\n");
        } else if (code == 0) {
            TEST_PASSED("execve() basic");
        } else {
            printf("  execve() failed with error %d\n", code);
        }
    }
}

/* Test sleep() and alarm() functions */
void test_sleep_alarm(void) {
    time_t start, end;
    unsigned int remaining;
    
    TEST_START("sleep() and alarm()");
    
    /* Test sleep for 1 second */
    time(&start);
    remaining = sleep(1);
    time(&end);
    
    assert(remaining == 0);
    assert(end - start >= 1);
    
    /* Test interrupted sleep */
    /* Note: Proper test would require signal handling */
    
    /* Test alarm */
    unsigned int prev = alarm(5);
    assert(prev == 0); /* No previous alarm */
    
    /* Cancel alarm */
    prev = alarm(0);
    assert(prev > 0 && prev <= 5);
    
    TEST_PASSED("sleep() and alarm()");
}

/* Test pause() function */
static volatile int pause_interrupted = 0;

void pause_handler(int sig) {
    (void)sig;
    pause_interrupted = 1;
}

void test_pause(void) {
    TEST_START("pause()");
    
    /* Set up signal handler */
    struct sigaction sa;
    sa.sa_handler = pause_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        printf("  pause() test skipped (cannot set signal handler)\n");
        return;
    }
    
    /* Set alarm to interrupt pause */
    pause_interrupted = 0;
    alarm(1);
    
    /* This should block until signal */
    int ret = pause();
    
    assert(ret == -1);
    assert(errno == EINTR);
    assert(pause_interrupted == 1);
    
    TEST_PASSED("pause()");
}

/* Test pathconf() function */
void test_pathconf(void) {
    long value;
    
    TEST_START("pathconf()");
    
    /* Create test file */
    int fd = open(TEST_FILE, O_CREAT | O_WRONLY, 0644);
    assert(fd >= 0);
    close(fd);
    
    /* Test various pathconf values */
    value = pathconf(TEST_FILE, _PC_LINK_MAX);
    if (value == -1 && errno == ENOSYS) {
        printf("  pathconf() not implemented (ENOSYS)\n");
        unlink(TEST_FILE);
        return;
    }
    
    if (value != -1) {
        printf("  _PC_LINK_MAX = %ld\n", value);
        assert(value > 0);
    }
    
    value = pathconf(TEST_FILE, _PC_NAME_MAX);
    if (value != -1) {
        printf("  _PC_NAME_MAX = %ld\n", value);
        assert(value >= 14); /* POSIX minimum */
    }
    
    value = pathconf(TEST_FILE, _PC_PATH_MAX);
    if (value != -1) {
        printf("  _PC_PATH_MAX = %ld\n", value);
        assert(value >= 256); /* POSIX minimum */
    }
    
    value = pathconf(TEST_FILE, _PC_PIPE_BUF);
    if (value != -1) {
        printf("  _PC_PIPE_BUF = %ld\n", value);
        assert(value >= 512); /* POSIX minimum */
    }
    
    /* Test with directory */
    mkdir(TEST_DIR, 0755);
    
    value = pathconf(TEST_DIR, _PC_NAME_MAX);
    if (value != -1) {
        assert(value >= 14);
    }
    
    rmdir(TEST_DIR);
    unlink(TEST_FILE);
    TEST_PASSED("pathconf()");
}

/* Test sysconf() function */
void test_sysconf(void) {
    long value;
    
    TEST_START("sysconf()");
    
    /* Test various sysconf values */
    value = sysconf(_SC_ARG_MAX);
    if (value == -1 && errno == ENOSYS) {
        printf("  sysconf() not implemented (ENOSYS)\n");
        return;
    }
    
    if (value != -1) {
        printf("  _SC_ARG_MAX = %ld\n", value);
        assert(value >= 4096); /* POSIX minimum */
    }
    
    value = sysconf(_SC_CHILD_MAX);
    if (value != -1) {
        printf("  _SC_CHILD_MAX = %ld\n", value);
        assert(value >= 25); /* POSIX minimum */
    }
    
    value = sysconf(_SC_CLK_TCK);
    if (value != -1) {
        printf("  _SC_CLK_TCK = %ld\n", value);
        assert(value > 0);
    }
    
    value = sysconf(_SC_OPEN_MAX);
    if (value != -1) {
        printf("  _SC_OPEN_MAX = %ld\n", value);
        assert(value >= 20); /* POSIX minimum */
    }
    
    value = sysconf(_SC_PAGESIZE);
    if (value != -1) {
        printf("  _SC_PAGESIZE = %ld\n", value);
        assert(value > 0);
        assert((value & (value - 1)) == 0); /* Power of 2 */
    }
    
    value = sysconf(_SC_VERSION);
    if (value != -1) {
        printf("  _SC_VERSION = %ld\n", value);
        assert(value >= 200112L); /* POSIX.1-2001 */
    }
    
    /* Test invalid argument */
    errno = 0;
    value = sysconf(999999);
    assert(value == -1);
    assert(errno == EINVAL || errno == 0);
    
    TEST_PASSED("sysconf()");
}

int main(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 unistd.h Test Suite ===" COLOR_RESET "\n\n");
    
    /* Clean up any leftover files */
    cleanup_files();
    
    /* Run tests */
    test_read_write();
    test_lseek();
    test_close();
    test_dup_dup2();
    test_access();
    test_unlink();
    test_link();
    test_rmdir();
    test_chdir_getcwd();
    test_pipe();
    test_process_ids();
    test_fork_exit();
    test_execve();
    test_sleep_alarm();
    test_pause();
    test_pathconf();
    test_sysconf();
    
    /* Final cleanup */
    cleanup_files();
    
    TEST_SUITE_PASSED("unistd.h");
    return 0;
}
