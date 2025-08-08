/*
 * test_fcntl_embedded.c - PSE51 fcntl.h test suite for embedded systems
 * Tests: open, creat, fcntl
 * 
 * This test suite is designed for embedded systems (RISC-V QEMU virt)
 */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <compiler.h>
#include <uart.h>
#include <sys/types.h>
#include <sys/stat.h>
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


/* Test open() function */
void test_open(void) {
    int fd;
    
    printf("Testing open()...");
    
    /* Test creating a new file */
    fd = open("/tmp/test.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        if (errno == ENOSYS) {
            SKIP_TEST("not implemented");
        }
        TEST_ASSERT(0);
        return;
    }
    
    /* File descriptor should be valid */
    TEST_ASSERT(fd >= 0);
    
    /* Close the file */
    close(fd);
    
    printf(" PASSED\n");
    tests_passed++;
}

/* Test creat() function */
void test_creat(void) {
    int fd;
    
    printf("Testing creat()...");
    
    /* Test creating a file */
    fd = creat("/tmp/test2.txt", 0644);
    if (fd < 0) {
        if (errno == ENOSYS) {
            SKIP_TEST("not implemented");
        }
        TEST_ASSERT(0);
        return;
    }
    
    TEST_ASSERT(fd >= 0);
    close(fd);
    
    printf(" PASSED\n");
    tests_passed++;
}

/* Test fcntl() function */
void test_fcntl(void) {
    int fd, flags;
    
    printf("Testing fcntl()...");
    
    fd = open("/tmp/test3.txt", O_CREAT | O_RDWR, 0644);
    if (fd < 0) {
        SKIP_TEST("open failed");
    }
    
    /* Test F_GETFD */
    flags = fcntl(fd, F_GETFD);
    if (flags < 0 && errno == ENOSYS) {
        close(fd);
        SKIP_TEST("not implemented");
    }
    
    /* Test F_SETFD */
    TEST_ASSERT(fcntl(fd, F_SETFD, FD_CLOEXEC) >= 0);
    
    /* Test F_GETFL */
    flags = fcntl(fd, F_GETFL);
    TEST_ASSERT(flags >= 0);
    
    close(fd);
    
    printf(" PASSED\n");
    tests_passed++;
}


/* Print test summary */
void print_summary(void) {
    printf("\n");
    printf("=====================================\n");
    printf("=== PSE51 fcntl.h Test Summary ===\n");
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
    
    printf("=== PSE51 fcntl.h Test Suite (Embedded) ===\n\n");
    
    /* Reset test counters */
    tests_passed = 0;
    tests_failed = 0;
    tests_skipped = 0;
    
    /* Run tests */
    test_open();
    test_creat();
    test_fcntl();
    
    /* Print summary */
    print_summary();
    
    /* Infinite loop for embedded system */
    printf("\nTest complete. System halted.\n");
    while(1) {
        /* Halt */
    }
    
    return 0;
}
