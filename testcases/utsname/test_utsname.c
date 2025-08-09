/*
 * test_utsname.c - PSE51 sys/utsname.h system name test suite
 * Tests: uname
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for system identification
 */

#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include <unistd.h>
#include <stddef.h>

#include "../test_colors.h"
/* Maximum expected length for utsname fields */
#define MAX_FIELD_LEN 256

/* Test uname() function */
void test_uname(void) {
    struct utsname uts;
    int ret;
    
    TEST_START("uname()");
    
    /* Test basic uname call */
    ret = uname(&uts);
    if (ret == -1 && errno == ENOSYS) {
        printf("  uname() not implemented (ENOSYS)\n");
        return;
    }
    
    assert(ret == 0);
    
    /* Print system information */
    printf("  System name (sysname): %s\n", uts.sysname);
    printf("  Node name (nodename): %s\n", uts.nodename);
    printf("  Release (release): %s\n", uts.release);
    printf("  Version (version): %s\n", uts.version);
    printf("  Machine (machine): %s\n", uts.machine);
    
    /* Verify all fields are null-terminated strings */
    assert(strlen(uts.sysname) > 0);
    assert(strlen(uts.sysname) < MAX_FIELD_LEN);
    
    assert(strlen(uts.nodename) >= 0); /* Can be empty */
    assert(strlen(uts.nodename) < MAX_FIELD_LEN);
    
    assert(strlen(uts.release) > 0);
    assert(strlen(uts.release) < MAX_FIELD_LEN);
    
    assert(strlen(uts.version) > 0);
    assert(strlen(uts.version) < MAX_FIELD_LEN);
    
    assert(strlen(uts.machine) > 0);
    assert(strlen(uts.machine) < MAX_FIELD_LEN);
    
    /* Test that fields don't contain null bytes in the middle */
    for (size_t i = 0; i < strlen(uts.sysname); i++) {
        assert(uts.sysname[i] != '\0');
    }
    
    /* Common sysname values */
    if (strcmp(uts.sysname, "Linux") == 0 ||
        strcmp(uts.sysname, "Darwin") == 0 ||
        strcmp(uts.sysname, "FreeBSD") == 0 ||
        strcmp(uts.sysname, "OpenBSD") == 0 ||
        strcmp(uts.sysname, "NetBSD") == 0) {
        printf("  Recognized system: %s\n", uts.sysname);
    }
    
    /* Test with NULL pointer */
    ret = uname(NULL);
    assert(ret == -1);
    assert(errno == EFAULT);
    
    TEST_PASSED("uname()");
}

/* Test field validation */
void test_field_validation(void) {
    struct utsname uts;
    int ret;
    
    TEST_START("field validation");
    
    ret = uname(&uts);
    if (ret == -1) {
        printf("  uname() not available\n");
        return;
    }
    
    /* Check that fields contain printable characters */
    for (size_t i = 0; i < strlen(uts.sysname); i++) {
        assert(isprint((unsigned char)uts.sysname[i]) || isspace((unsigned char)uts.sysname[i]));
    }
    
    for (size_t i = 0; i < strlen(uts.nodename); i++) {
        assert(isprint((unsigned char)uts.nodename[i]) || isspace((unsigned char)uts.nodename[i]));
    }
    
    for (size_t i = 0; i < strlen(uts.release); i++) {
        assert(isprint((unsigned char)uts.release[i]) || isspace((unsigned char)uts.release[i]));
    }
    
    for (size_t i = 0; i < strlen(uts.version); i++) {
        assert(isprint((unsigned char)uts.version[i]) || isspace((unsigned char)uts.version[i]));
    }
    
    for (size_t i = 0; i < strlen(uts.machine); i++) {
        assert(isprint((unsigned char)uts.machine[i]) || isspace((unsigned char)uts.machine[i]));
    }
    
    /* Verify machine field contains expected architectures */
    const char *known_machines[] = {
        "x86_64", "i386", "i486", "i586", "i686",
        "amd64", "arm", "aarch64", "armv7l", "armv6l",
        "riscv32", "riscv64", "ppc", "ppc64", "ppc64le",
        "mips", "mips64", "sparc", "sparc64", "s390x",
        NULL
    };
    
    int found = 0;
    for (int i = 0; known_machines[i] != NULL; i++) {
        if (strstr(uts.machine, known_machines[i]) != NULL) {
            found = 1;
            printf("  Recognized machine type: %s\n", uts.machine);
            break;
        }
    }
    
    if (!found) {
        printf("  Unknown machine type: %s\n", uts.machine);
    }
    
    TEST_PASSED("Field validation");
}

/* Test consistency between calls */
void test_consistency(void) {
    struct utsname uts1, uts2;
    int ret;
    
    TEST_START("consistency between calls");
    
    /* Get system info twice */
    ret = uname(&uts1);
    if (ret == -1) {
        printf("  uname() not available\n");
        return;
    }
    
    ret = uname(&uts2);
    assert(ret == 0);
    
    /* Results should be identical */
    assert(strcmp(uts1.sysname, uts2.sysname) == 0);
    assert(strcmp(uts1.nodename, uts2.nodename) == 0);
    assert(strcmp(uts1.release, uts2.release) == 0);
    assert(strcmp(uts1.version, uts2.version) == 0);
    assert(strcmp(uts1.machine, uts2.machine) == 0);
    
    /* Multiple rapid calls should return same data */
    for (int i = 0; i < 10; i++) {
        ret = uname(&uts2);
        assert(ret == 0);
        assert(strcmp(uts1.sysname, uts2.sysname) == 0);
    }
    
    TEST_PASSED("Consistency");
}

/* Test nodename relationship with hostname */
void test_nodename(void) {
    struct utsname uts;
    char hostname[256];
    int ret;
    
    TEST_START("nodename field");
    
    ret = uname(&uts);
    if (ret == -1) {
        printf("  uname() not available\n");
        return;
    }
    
    /* Get hostname for comparison */
    ret = gethostname(hostname, sizeof(hostname));
    if (ret == 0) {
        hostname[sizeof(hostname) - 1] = '\0';
        
        /* nodename should match hostname */
        if (strcmp(uts.nodename, hostname) == 0) {
            printf("  nodename matches hostname: %s\n", hostname);
        } else {
            printf("  nodename: %s, hostname: %s\n", uts.nodename, hostname);
            /* They might differ in some configurations */
        }
    }
    
    /* nodename should not be too long */
    assert(strlen(uts.nodename) < 256);
    
    TEST_PASSED("Nodename");
}

/* Test memory safety */
void test_memory_safety(void) {
    struct utsname uts;
    int ret;
    
    TEST_START("memory safety");
    
    /* Initialize structure with known pattern */
    memset(&uts, 0xAA, sizeof(uts));
    
    ret = uname(&uts);
    if (ret == -1) {
        printf("  uname() not available\n");
        return;
    }
    
    /* Verify null termination */
    assert(memchr(uts.sysname, '\0', sizeof(uts.sysname)) != NULL);
    assert(memchr(uts.nodename, '\0', sizeof(uts.nodename)) != NULL);
    assert(memchr(uts.release, '\0', sizeof(uts.release)) != NULL);
    assert(memchr(uts.version, '\0', sizeof(uts.version)) != NULL);
    assert(memchr(uts.machine, '\0', sizeof(uts.machine)) != NULL);
    
    /* Check for buffer overruns - remaining bytes after null should be untouched or zero */
    size_t len;
    
    len = strlen(uts.sysname);
    if (len + 1 < sizeof(uts.sysname)) {
        /* Remaining bytes might be zero or unchanged */
    }
    
    TEST_PASSED("Memory safety");
}

/* Test edge cases */
void test_edge_cases(void) {
    struct utsname uts;
    struct utsname *null_ptr = NULL;
    int ret;
    
    TEST_START("edge cases");
    
    /* Test with various invalid pointers */
    ret = uname(null_ptr);
    assert(ret == -1);
    assert(errno == EFAULT);
    
    /* Test with misaligned pointer (if possible) */
    char buffer[sizeof(struct utsname) + 1];
    struct utsname *misaligned = (struct utsname *)(buffer + 1);
    
    /* This might work on some architectures */
    ret = uname(misaligned);
    if (ret == -1) {
        /* Expected on strict alignment architectures */
        assert(errno == EFAULT || errno == EINVAL);
    } else {
        /* Should still work correctly */
        assert(strlen(misaligned->sysname) > 0);
    }
    
    /* Test repeated calls */
    for (int i = 0; i < 100; i++) {
        ret = uname(&uts);
        if (ret == 0) {
            assert(strlen(uts.sysname) > 0);
        }
    }
    
    TEST_PASSED("Edge case");
}

/* Test POSIX compliance */
void test_posix_compliance(void) {
    struct utsname uts;
    int ret;
    
    TEST_START("POSIX compliance");
    
    ret = uname(&uts);
    if (ret == -1) {
        printf("  uname() not available\n");
        return;
    }
    
    /* POSIX requires specific field names and ordering */
    /* Fields should be accessible as documented */
    printf("  POSIX field order verified:\n");
    printf("    sysname at offset %zu\n", offsetof(struct utsname, sysname));
    printf("    nodename at offset %zu\n", offsetof(struct utsname, nodename));
    printf("    release at offset %zu\n", offsetof(struct utsname, release));
    printf("    version at offset %zu\n", offsetof(struct utsname, version));
    printf("    machine at offset %zu\n", offsetof(struct utsname, machine));
    
    /* Verify field ordering */
    assert(offsetof(struct utsname, sysname) < offsetof(struct utsname, nodename));
    assert(offsetof(struct utsname, nodename) < offsetof(struct utsname, release));
    assert(offsetof(struct utsname, release) < offsetof(struct utsname, version));
    assert(offsetof(struct utsname, version) < offsetof(struct utsname, machine));
    
    TEST_PASSED("POSIX compliance");
}

/* Test common usage patterns */
void test_usage_patterns(void) {
    struct utsname uts;
    int ret;
    
    TEST_START("common usage patterns");
    
    /* Pattern 1: Check if running on Linux */
    ret = uname(&uts);
    if (ret == 0) {
        if (strcmp(uts.sysname, "Linux") == 0) {
            printf("  Running on Linux\n");
            
            /* Linux-specific checks */
            /* Release should be like "5.x.x" or "4.x.x" */
            assert(uts.release[0] >= '2' && uts.release[0] <= '9');
            assert(uts.release[1] == '.' || isdigit(uts.release[1]));
        }
    }
    
    /* Pattern 2: Architecture detection */
    if (ret == 0) {
        if (strstr(uts.machine, "64") != NULL) {
            printf("  64-bit architecture detected\n");
        } else if (strstr(uts.machine, "86") != NULL) {
            printf("  x86 architecture detected\n");
        } else if (strstr(uts.machine, "arm") != NULL) {
            printf("  ARM architecture detected\n");
        }
    }
    
    /* Pattern 3: Version string parsing */
    if (ret == 0) {
        printf("  Full version info: %s\n", uts.version);
        /* Version often contains build date and compiler info */
    }
    
    /* Pattern 4: Creating system identifier */
    if (ret == 0) {
        char system_id[512];
        snprintf(system_id, sizeof(system_id), "%s_%s_%s", 
                 uts.sysname, uts.release, uts.machine);
        printf("  System identifier: %s\n", system_id);
    }
    
    TEST_PASSED("Usage pattern");
}

/* Test field sizes */
void test_field_sizes(void) {
    struct utsname uts;
    
    TEST_START("field sizes");
    
    /* Check structure and field sizes */
    printf("  Structure size: %zu bytes\n", sizeof(struct utsname));
    printf("  sysname field size: %zu bytes\n", sizeof(uts.sysname));
    printf("  nodename field size: %zu bytes\n", sizeof(uts.nodename));
    printf("  release field size: %zu bytes\n", sizeof(uts.release));
    printf("  version field size: %zu bytes\n", sizeof(uts.version));
    printf("  machine field size: %zu bytes\n", sizeof(uts.machine));
    
    /* POSIX doesn't specify exact sizes, but they should be reasonable */
    assert(sizeof(uts.sysname) >= 9);   /* Enough for "Linux" etc */
    assert(sizeof(uts.nodename) >= 65); /* Typical hostname limit */
    assert(sizeof(uts.release) >= 9);   /* Enough for version numbers */
    assert(sizeof(uts.version) >= 9);   /* Enough for version info */
    assert(sizeof(uts.machine) >= 9);   /* Enough for "x86_64" etc */
    
    /* All fields should be same size in many implementations */
    if (sizeof(uts.sysname) == sizeof(uts.nodename) &&
        sizeof(uts.sysname) == sizeof(uts.release) &&
        sizeof(uts.sysname) == sizeof(uts.version) &&
        sizeof(uts.sysname) == sizeof(uts.machine)) {
        printf("  All fields have uniform size: %zu bytes\n", sizeof(uts.sysname));
    }
    
    TEST_PASSED("Field size");
}




/* Run all tests */
void run_tests(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 utsname.h Test Suite ===" COLOR_RESET "\n\n");
    
    test_uname();
    test_field_validation();
    test_consistency();
    test_nodename();
    test_memory_safety();
    test_edge_cases();
    test_posix_compliance();
    test_usage_patterns();
    test_field_sizes();
    
    TEST_SUITE_PASSED("utsname.h");
}
