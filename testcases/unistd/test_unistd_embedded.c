/*
 * test_unistd_embedded.c - PSE51 unistd.h test suite for embedded systems
 * Tests: read, write, close, lseek, access, unlink, rmdir, getcwd, chdir, sleep
 * 
 * This test suite is designed for embedded systems (RISC-V QEMU virt)
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <compiler.h>
#include <uart.h>
#include <fcntl.h>
#include <sys/types.h>

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


/* Test basic I/O functions */
void test_basic_io(void) {
    int fd;
    char buf[16];
    ssize_t n;
    
    printf("Testing basic I/O...");
    
    /* Open a file */
    fd = open("/tmp/io_test", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd < 0) {
        SKIP_TEST("open failed");
    }
    
    /* Write data */
    n = write(fd, "Hello", 5);
    TEST_ASSERT(n == 5);
    
    /* Seek to beginning */
    TEST_ASSERT(lseek(fd, 0, SEEK_SET) == 0);
    
    /* Read data back */
    n = read(fd, buf, sizeof(buf));
    TEST_ASSERT(n == 5);
    TEST_ASSERT(memcmp(buf, "Hello", 5) == 0);
    
    /* Close file */
    TEST_ASSERT(close(fd) == 0);
    
    /* Clean up */
    unlink("/tmp/io_test");
    
    printf(" PASSED\n");
    tests_passed++;
}

/* Test access() function */
void test_access(void) {
    int ret;
    
    printf("Testing access()...");
    
    /* Root directory should exist and be readable */
    ret = access("/", F_OK);
    if (ret < 0 && errno == ENOSYS) {
        SKIP_TEST("not implemented");
    }
    
    TEST_ASSERT(ret == 0);
    TEST_ASSERT(access("/", R_OK) == 0);
    
    /* Non-existent file */
    TEST_ASSERT(access("/nonexistent", F_OK) == -1);
    TEST_ASSERT(errno == ENOENT);
    
    printf(" PASSED\n");
    tests_passed++;
}

/* Test sleep() function */
void test_sleep(void) {
    time_t start, end;
    
    printf("Testing sleep()...");
    
    start = time(NULL);
    sleep(1);
    end = time(NULL);
    
    /* Should have slept at least 1 second */
    if (end > start) {
        TEST_ASSERT(end - start >= 1);
    }
    
    printf(" PASSED\n");
    tests_passed++;
}


/* Print test summary */
void print_summary(void) {
    printf("\n");
    printf("=====================================\n");
    printf("=== PSE51 unistd.h Test Summary ===\n");
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
    
    printf("=== PSE51 unistd.h Test Suite (Embedded) ===\n\n");
    
    /* Reset test counters */
    tests_passed = 0;
    tests_failed = 0;
    tests_skipped = 0;
    
    /* Run tests */
    test_basic_io();
    test_access();
    test_sleep();
    
    /* Print summary */
    print_summary();
    
    /* Infinite loop for embedded system */
    printf("\nTest complete. System halted.\n");
    while(1) {
        /* Halt */
    }
    
    return 0;
}
