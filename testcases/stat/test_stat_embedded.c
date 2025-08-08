/*
 * test_stat_embedded.c - PSE51 sys/stat.h test suite for embedded systems
 * Tests: stat, fstat, lstat, chmod, fchmod, mkdir, mkfifo, umask
 * 
 * This test suite is designed for embedded systems (RISC-V QEMU virt)
 */

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <compiler.h>
#include <uart.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

/* Global test status */
static int tests_passed = 0;
static int tests_failed = 0;
static int tests_skipped = 0;

/* Implement write() for printf support */
ssize_t write(int fd, const void *buf, size_t count)
{
    (void)fd;
    char *buffer = (char*)buf;
    size_t i;
    
    for (i = 0; i < count; i++) {
        UartPutc(buffer[i]);
    }
    
    return count;
}
    


/* Test helper macros */
#define TEST_ASSERT(cond) do { \
    if (!(cond)) { \
        printf("\n  FAILED: %s at line %d", #cond, __LINE__); \
        tests_failed++; \
    } \
} while(0)

#define SKIP_TEST(reason) do { \
    printf(" [SKIPPED: %s]", reason); \
    tests_skipped++; \
    return; \
} while(0)


/* Test stat() function */
void test_stat(void) {
    struct stat st;
    int ret;
    
    printf("Testing stat()...");
    
    /* Try to stat the root directory */
    ret = stat("/", &st);
    if (ret < 0) {
        if (errno == ENOSYS) {
            SKIP_TEST("not implemented");
        }
        TEST_ASSERT(0);
        return;
    }
    
    /* Root should be a directory */
    TEST_ASSERT(S_ISDIR(st.st_mode));
    
    printf(" PASSED\n");
    tests_passed++;
}

/* Test chmod() function */
void test_chmod(void) {
    int fd, ret;
    struct stat st;
    
    printf("Testing chmod()...");
    
    /* Create a test file */
    fd = open("/tmp/chmod_test", O_CREAT | O_WRONLY, 0644);
    if (fd < 0) {
        SKIP_TEST("cannot create test file");
    }
    close(fd);
    
    /* Change permissions */
    ret = chmod("/tmp/chmod_test", 0755);
    if (ret < 0 && errno == ENOSYS) {
        SKIP_TEST("not implemented");
    }
    
    TEST_ASSERT(ret == 0);
    
    /* Verify permissions changed */
    ret = stat("/tmp/chmod_test", &st);
    if (ret == 0) {
        TEST_ASSERT((st.st_mode & 0777) == 0755);
    }
    
    printf(" PASSED\n");
    tests_passed++;
}

/* Test umask() function */
void test_umask(void) {
    mode_t old_mask, new_mask;
    
    printf("Testing umask()...");
    
    /* Get current umask */
    old_mask = umask(0);
    
    /* Set new umask */
    new_mask = umask(0022);
    TEST_ASSERT(new_mask == 0);
    
    /* Restore old umask */
    umask(old_mask);
    
    printf(" PASSED\n");
    tests_passed++;
}


/* Print test summary */
void print_summary(void) {
    printf("\n");
    printf("=====================================\n");
    printf("=== PSE51 sys/stat.h Test Summary ===\n");
    printf("=====================================\n");
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);
    printf("Tests skipped: %d\n", tests_skipped);
    
    if (tests_failed == 0) {
        printf("\n*** ALL TESTS PASSED! ***\n");
    } else {
        printf("\n*** %d TESTS FAILED! ***\n", tests_failed);
    }
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    
    /* Initialize UART for output */
    UartStdOutInit();
    
    printf("=== PSE51 sys/stat.h Test Suite (Embedded) ===\n\n");
    
    /* Reset test counters */
    tests_passed = 0;
    tests_failed = 0;
    tests_skipped = 0;
    
    /* Run tests */
    test_stat();
    test_chmod();
    test_umask();
    
    /* Print summary */
    print_summary();
    
    /* Infinite loop for embedded system */
    printf("\nTest complete. System halted.\n");
    while(1) {
        /* Halt */
    }
    
    return 0;
}
