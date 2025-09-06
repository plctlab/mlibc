/*
 * test_misc_stdio.c - PSE51 stdio.h miscellaneous functions test suite
 * Tests: fflush, setvbuf, perror, rename, remove, tmpfile, tmpnam,
 *        fileno, fdopen, freopen, flockfile, ftrylockfile, funlockfile
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for misc stdio functions
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "../test_colors.h"
#define TEST_FILE "test_misc.txt"
#define TEST_FILE2 "test_misc2.txt"
#define BUFFER_SIZE 1024

/* Test fflush() */
void test_fflush(void) {
    FILE *fp;
    int ret;
    char buffer[BUFFER_SIZE];
    
    TEST_START("fflush()");
    
    /* Test fflush on output stream */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    /* Write without newline (may be buffered) */
    fprintf(fp, "Buffered data");
    
    /* Flush the buffer */
    ret = fflush(fp);
    assert(ret == 0);
    
    /* Data should now be in file even without close */
    /* Note: Can't easily verify without closing, so we'll trust fflush */
    
    fclose(fp);
    
    /* Test fflush(NULL) - flushes all open output streams */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fprintf(fp, "Test data");
    
    ret = fflush(NULL);
    assert(ret == 0);
    
    fclose(fp);
    
    /* Test fflush on input stream (implementation-defined) */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    ret = fflush(fp);
    /* Result is implementation-defined for input streams */
    
    fclose(fp);
    
    /* Test fflush on closed stream (undefined behavior - skip) */
    
    /* Test fflush with stdout */
    printf("  Visual test - this should appear immediately: ");
    fflush(stdout);
    printf("FLUSHED\n");
    
    TEST_PASSED("fflush()");
}

/* Test setvbuf() */
void test_setvbuf(void) {
    FILE *fp;
    int ret;
    char buffer[BUFFER_SIZE];
    char custom_buffer[256];
    
    TEST_START("setvbuf()");
    
    /* Test full buffering */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    ret = setvbuf(fp, NULL, _IOFBF, BUFFER_SIZE);
    assert(ret == 0);
    
    /* Write data (should be buffered) */
    fprintf(fp, "Fully buffered\n");
    
    fclose(fp);
    
    /* Test line buffering */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    ret = setvbuf(fp, NULL, _IOLBF, BUFFER_SIZE);
    assert(ret == 0);
    
    /* Write data (buffered until newline) */
    fprintf(fp, "Line buffered");
    fprintf(fp, "\n"); /* This should flush */
    
    fclose(fp);
    
    /* Test no buffering */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    ret = setvbuf(fp, NULL, _IONBF, 0);
    assert(ret == 0);
    
    /* Each character written immediately */
    fprintf(fp, "Not buffered\n");
    
    fclose(fp);
    
    /* Test with custom buffer */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    ret = setvbuf(fp, custom_buffer, _IOFBF, sizeof(custom_buffer));
    assert(ret == 0);
    
    fprintf(fp, "Custom buffer\n");
    
    fclose(fp);
    
    /* Test invalid mode */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    ret = setvbuf(fp, NULL, 9999, BUFFER_SIZE); /* Invalid mode */
    assert(ret != 0);
    
    fclose(fp);
    
    /* Test setvbuf after I/O (should fail) */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    fprintf(fp, "Some data");
    ret = setvbuf(fp, NULL, _IONBF, 0);
    /* Should fail or be implementation-defined */
    
    fclose(fp);
    
    TEST_PASSED("setvbuf()");
}

/* Test perror() */
void test_perror(void) {
    FILE *fp;
    
    TEST_START("perror()");
    
    /* Set up an error condition */
    errno = 0;
    fp = fopen("/nonexistent/path/to/file.txt", "r");
    assert(fp == NULL);
    assert(errno != 0);
    
    /* Test perror with message */
    printf("  Visual test - you should see an error message:\n  ");
    perror("fopen failed");
    
    /* Test perror with NULL message */
    printf("  Visual test - you should see just the error:\n  ");
    perror(NULL);
    
    /* Test perror with empty string */
    printf("  Visual test - you should see just the error:\n  ");
    perror("");
    
    /* Test with different error */
    errno = EACCES;
    printf("  Visual test - you should see permission denied error:\n  ");
    perror("Permission test");
    
    /* Reset errno */
    errno = 0;
    
    TEST_PASSED("perror()");
}

/* Test rename() */
void test_rename(void) {
    FILE *fp;
    int ret;
    
    TEST_START("rename()");
    
    /* Create source file */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fprintf(fp, "File to be renamed\n");
    fclose(fp);
    
    /* Test successful rename */
    ret = rename(TEST_FILE, TEST_FILE2);
    if (ret != 0 && errno == ENOSYS) {
        printf("  Note: rename() not implemented, skipping tests\n");
        unlink(TEST_FILE);
        return;
    }
    assert(ret == 0);
    
    /* Verify old name doesn't exist */
    fp = fopen(TEST_FILE, "r");
    assert(fp == NULL);
    
    /* Verify new name exists */
    fp = fopen(TEST_FILE2, "r");
    assert(fp != NULL);
    fclose(fp);
    
    /* Test renaming non-existent file */
    ret = rename("nonexistent.txt", "newname.txt");
    assert(ret != 0);
    assert(errno == ENOENT);
    
    /* Test renaming to existing file (overwrites on POSIX) */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fprintf(fp, "Original content\n");
    fclose(fp);
    
    ret = rename(TEST_FILE2, TEST_FILE);
    assert(ret == 0);
    
    /* Clean up */
    unlink(TEST_FILE);
    
    TEST_PASSED("rename()");
}

/* Test remove() */
void test_remove(void) {
    FILE *fp;
    int ret;
    
    TEST_START("remove()");
    
    /* Create test file */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fprintf(fp, "File to be removed\n");
    fclose(fp);
    
    /* Test successful removal */
    ret = remove(TEST_FILE);
    if (ret != 0 && errno == ENOSYS) {
        printf("  Note: remove() not implemented, skipping tests\n");
        return;
    }
    assert(ret == 0);
    
    /* Verify file is gone */
    fp = fopen(TEST_FILE, "r");
    assert(fp == NULL);
    assert(errno == ENOENT);
    
    /* Test removing non-existent file */
    ret = remove("nonexistent.txt");
    assert(ret != 0);
    assert(errno == ENOENT);
    
    /* Test removing directory (if supported) */
    /* Note: remove() behavior on directories is implementation-defined */
    
    TEST_PASSED("remove()");
}

/* Test tmpfile() */
void test_tmpfile(void) {
    FILE *fp;
    int ret;
    
    TEST_START("tmpfile()");
    
    /* Create temporary file */
    fp = tmpfile();
    if (fp == NULL && errno == ENOSYS) {
        printf("  Note: tmpfile() not implemented, skipping tests\n");
        return;
    }
    
    if (fp != NULL) {
        /* Should be open for reading and writing */
        ret = fprintf(fp, "Temporary data\n");
        assert(ret > 0);
        
        /* Seek and read back */
        rewind(fp);
        char buffer[100];
        assert(fgets(buffer, sizeof(buffer), fp) != NULL);
        assert(strcmp(buffer, "Temporary data\n") == 0);
        
        /* File should be automatically deleted on close */
        fclose(fp);
        
        /* Create multiple temporary files */
        FILE *fp1 = tmpfile();
        FILE *fp2 = tmpfile();
        
        if (fp1 && fp2) {
            assert(fp1 != fp2); /* Should be different files */
            fclose(fp1);
            fclose(fp2);
        }
    }
    
    TEST_PASSED("tmpfile()");
}

/* Test tmpnam() */
void test_tmpnam(void) {
    char buffer[256];  /* L_tmpnam not available in minimal libc */
    char *name1, *name2;
    
    TEST_START("tmpnam()");
    
    /* Get temporary filename */
    name1 = tmpnam(NULL);
    if (name1 == NULL && errno == ENOSYS) {
        printf("  Note: tmpnam() not implemented, skipping tests\n");
        return;
    }
    
    if (name1 != NULL) {
        assert(strlen(name1) > 0);
        /* L_tmpnam not available in minimal libc */
        assert(strlen(name1) < 256);
        
        /* Get another name - should be different */
        name2 = tmpnam(NULL);
        if (name2 != NULL) {
            assert(strcmp(name1, name2) != 0);
        }
        
        /* Test with provided buffer */
        name1 = tmpnam(buffer);
        assert(name1 == buffer);
        assert(strlen(buffer) > 0);
        
        /* Note: tmpnam() is deprecated due to race conditions */
    }
    
    TEST_PASSED("tmpnam()");
}

/* Test fileno() */
void test_fileno(void) {
    FILE *fp;
    int fd;
    
    TEST_START("fileno()");
    
    /* Test standard streams */
    fd = fileno(stdin);
    assert(fd == STDIN_FILENO || fd >= 0);
    
    fd = fileno(stdout);
    assert(fd == STDOUT_FILENO || fd >= 0);
    
    fd = fileno(stderr);
    assert(fd == STDERR_FILENO || fd >= 0);
    
    /* Test regular file */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    fd = fileno(fp);
    assert(fd > STDERR_FILENO); /* Should be > 2 */
    
    fclose(fp);
    
    TEST_PASSED("fileno()");
}

/* Test fdopen() */
void test_fdopen(void) {
    FILE *fp;
    int fd;
    
    TEST_START("fdopen()");
    
    /* Create file using low-level I/O */
    fd = open(TEST_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (fd < 0) {
        printf("  Note: open() not available, skipping fdopen tests\n");
        return;
    }
    
    /* Convert to FILE* */
    fp = fdopen(fd, "w");
    if (fp == NULL && errno == ENOSYS) {
        printf("  Note: fdopen() not implemented, skipping tests\n");
        close(fd);
        return;
    }
    assert(fp != NULL);
    
    /* Use FILE* operations */
    fprintf(fp, "Written via fdopen\n");
    
    /* Close FILE* (also closes fd) */
    fclose(fp);
    
    /* Verify file content */
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    char buffer[100];
    assert(fgets(buffer, sizeof(buffer), fp) != NULL);
    assert(strcmp(buffer, "Written via fdopen\n") == 0);
    fclose(fp);
    
    TEST_PASSED("fdopen()");
}

/* Test freopen() */
void test_freopen(void) {
    FILE *fp, *fp2;
    char buffer[100];
    
    TEST_START("freopen()");
    
    /* Create initial file */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fprintf(fp, "Initial content\n");
    /* Don't close - will use freopen */
    
    /* Reopen same file for reading */
    fp2 = freopen(TEST_FILE, "r", fp);
    if (fp2 == NULL && errno == ENOSYS) {
        printf("  Note: freopen() not implemented, skipping tests\n");
        fclose(fp);
        return;
    }
    assert(fp2 == fp); /* Should return same FILE* */
    
    /* Read content */
    assert(fgets(buffer, sizeof(buffer), fp) != NULL);
    assert(strcmp(buffer, "Initial content\n") == 0);
    
    /* Reopen different file */
    fp = fopen(TEST_FILE2, "w");
    assert(fp != NULL);
    fprintf(fp, "Second file\n");
    fclose(fp);
    
    fp = fopen(TEST_FILE, "r");
    assert(fp != NULL);
    
    fp2 = freopen(TEST_FILE2, "r", fp);
    assert(fp2 == fp);
    
    /* Should now read from second file */
    assert(fgets(buffer, sizeof(buffer), fp) != NULL);
    assert(strcmp(buffer, "Second file\n") == 0);
    
    fclose(fp);
    
    /* Clean up */
    unlink(TEST_FILE2);
    
    TEST_PASSED("freopen()");
}

/* Test file locking functions */
void test_file_locking(void) {
    FILE *fp;
    
    TEST_START("file locking functions");
    
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    /* Test flockfile */
    flockfile(fp);
    /* File is now locked for thread-safe access */
    
    /* Do some I/O while locked */
    fprintf(fp, "Thread-safe write\n");
    
    /* Test funlockfile */
    funlockfile(fp);
    /* File is now unlocked */
    
    /* Test ftrylockfile */
    int ret = ftrylockfile(fp);
    if (ret == 0) {
        /* Successfully locked */
        fprintf(fp, "Got lock\n");
        funlockfile(fp);
    }
    
    fclose(fp);
    
    /* Note: In single-threaded environment, these are often no-ops */
    
    TEST_PASSED("File locking");
}

/* Test combined operations */
void test_combined_operations(void) {
    FILE *fp;
    char buffer[BUFFER_SIZE];
    int ret;
    
    TEST_START("combined operations");
    
    /* Test setvbuf + fflush */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    
    ret = setvbuf(fp, NULL, _IOFBF, BUFFER_SIZE);
    assert(ret == 0);
    
    fprintf(fp, "Buffered data");
    ret = fflush(fp);
    assert(ret == 0);
    
    fclose(fp);
    
    /* Test rename + remove */
    fp = fopen(TEST_FILE, "w");
    assert(fp != NULL);
    fclose(fp);
    
    if (rename(TEST_FILE, TEST_FILE2) == 0) {
        unlink(TEST_FILE2);
    }
    
    TEST_PASSED("Combined operation");
}

/* Clean up test files */
void cleanup_files(void) {
    unlink(TEST_FILE);
    unlink(TEST_FILE2);
}

void run_tests(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 stdio.h Miscellaneous Functions Test Suite ===" COLOR_RESET "\n\n");
    
    /* Clean up any existing test files */
    cleanup_files();
    
    /* Run tests */
    test_fflush();
    test_setvbuf();
    test_perror();
    test_rename();
    test_remove();
    test_tmpfile();
    test_tmpnam();
    test_fileno();
    test_fdopen();
    test_freopen();
    test_file_locking();
    test_combined_operations();
    
    /* Clean up test files */
    cleanup_files();
    
    TEST_SUITE_PASSED("miscellaneous stdio");
}
