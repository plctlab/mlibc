/*
 * test_string_misc.c - PSE51 string.h miscellaneous function test suite
 * Tests: strtok, strtok_r, strxfrm, strerror, strerror_r
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for miscellaneous string functions
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <locale.h>
#include <pthread.h>

#include "../test_colors.h"
#define BUFFER_SIZE 1024

/* Test strtok() function */
void test_strtok(void) {
    char str[BUFFER_SIZE];
    char *token;
    int count;
    
    TEST_START("strtok()");
    
    /* Test basic tokenization */
    strcpy(str, "Hello,World,Test");
    
    token = strtok(str, ",");
    assert(token != NULL);
    assert(strcmp(token, "Hello") == 0);
    
    token = strtok(NULL, ",");
    assert(token != NULL);
    assert(strcmp(token, "World") == 0);
    
    token = strtok(NULL, ",");
    assert(token != NULL);
    assert(strcmp(token, "Test") == 0);
    
    token = strtok(NULL, ",");
    assert(token == NULL);
    
    /* Test multiple delimiters */
    strcpy(str, "One,Two;Three:Four");
    
    token = strtok(str, ",;:");
    assert(strcmp(token, "One") == 0);
    
    token = strtok(NULL, ",;:");
    assert(strcmp(token, "Two") == 0);
    
    token = strtok(NULL, ",;:");
    assert(strcmp(token, "Three") == 0);
    
    token = strtok(NULL, ",;:");
    assert(strcmp(token, "Four") == 0);
    
    token = strtok(NULL, ",;:");
    assert(token == NULL);
    
    /* Test consecutive delimiters */
    strcpy(str, "A,,B,,,C");
    
    token = strtok(str, ",");
    assert(strcmp(token, "A") == 0);
    
    token = strtok(NULL, ",");
    assert(strcmp(token, "B") == 0);
    
    token = strtok(NULL, ",");
    assert(strcmp(token, "C") == 0);
    
    /* Test whitespace delimiters */
    strcpy(str, "  Word1  \t Word2 \n Word3  ");
    
    token = strtok(str, " \t\n");
    assert(strcmp(token, "Word1") == 0);
    
    token = strtok(NULL, " \t\n");
    assert(strcmp(token, "Word2") == 0);
    
    token = strtok(NULL, " \t\n");
    assert(strcmp(token, "Word3") == 0);
    
    token = strtok(NULL, " \t\n");
    assert(token == NULL);
    
    /* Test empty string */
    strcpy(str, "");
    token = strtok(str, ",");
    assert(token == NULL);
    
    /* Test string with only delimiters */
    strcpy(str, ",,,");
    token = strtok(str, ",");
    assert(token == NULL);
    
    /* Test single token */
    strcpy(str, "SingleToken");
    token = strtok(str, ",");
    assert(strcmp(token, "SingleToken") == 0);
    
    token = strtok(NULL, ",");
    assert(token == NULL);
    
    /* Test changing delimiters */
    strcpy(str, "A,B;C");
    
    token = strtok(str, ",");
    assert(strcmp(token, "A") == 0);
    
    token = strtok(NULL, ";");
    assert(strcmp(token, "B") == 0);
    
    token = strtok(NULL, ",");
    assert(strcmp(token, "C") == 0);
    
    /* Test that original string is modified */
    strcpy(str, "Test,String");
    token = strtok(str, ",");
    assert(str[4] == '\0'); /* Comma replaced with null */
    
    /* Test parsing path */
    strcpy(str, "/usr/local/bin");
    count = 0;
    
    token = strtok(str, "/");
    while (token != NULL) {
        count++;
        token = strtok(NULL, "/");
    }
    assert(count == 3); /* usr, local, bin */
    
    TEST_PASSED("strtok()");
}

/* Test strtok_r() function */
void test_strtok_r(void) {
    char str1[BUFFER_SIZE];
    char str2[BUFFER_SIZE];
    char *token;
    char *saveptr1, *saveptr2;
    
    TEST_START("strtok_r()");
    
    /* Test basic usage */
    strcpy(str1, "A,B,C");
    
    token = strtok_r(str1, ",", &saveptr1);
    assert(strcmp(token, "A") == 0);
    
    token = strtok_r(NULL, ",", &saveptr1);
    assert(strcmp(token, "B") == 0);
    
    token = strtok_r(NULL, ",", &saveptr1);
    assert(strcmp(token, "C") == 0);
    
    token = strtok_r(NULL, ",", &saveptr1);
    assert(token == NULL);
    
    /* Test multiple simultaneous tokenizations */
    strcpy(str1, "1,2,3");
    strcpy(str2, "A;B;C");
    
    char *t1 = strtok_r(str1, ",", &saveptr1);
    char *t2 = strtok_r(str2, ";", &saveptr2);
    
    assert(strcmp(t1, "1") == 0);
    assert(strcmp(t2, "A") == 0);
    
    t1 = strtok_r(NULL, ",", &saveptr1);
    t2 = strtok_r(NULL, ";", &saveptr2);
    
    assert(strcmp(t1, "2") == 0);
    assert(strcmp(t2, "B") == 0);
    
    t1 = strtok_r(NULL, ",", &saveptr1);
    t2 = strtok_r(NULL, ";", &saveptr2);
    
    assert(strcmp(t1, "3") == 0);
    assert(strcmp(t2, "C") == 0);
    
    /* Test nested tokenization */
    strcpy(str1, "name=John,age=30,city=NYC");
    
    token = strtok_r(str1, ",", &saveptr1);
    while (token != NULL) {
        char *key = strtok_r(token, "=", &saveptr2);
        char *value = strtok_r(NULL, "=", &saveptr2);
        
        assert(key != NULL);
        assert(value != NULL);
        
        if (strcmp(key, "name") == 0) {
            assert(strcmp(value, "John") == 0);
        } else if (strcmp(key, "age") == 0) {
            assert(strcmp(value, "30") == 0);
        } else if (strcmp(key, "city") == 0) {
            assert(strcmp(value, "NYC") == 0);
        }
        
        token = strtok_r(NULL, ",", &saveptr1);
    }
    
    /* Test edge cases */
    strcpy(str1, "");
    token = strtok_r(str1, ",", &saveptr1);
    assert(token == NULL);
    
    strcpy(str1, ",,,");
    token = strtok_r(str1, ",", &saveptr1);
    assert(token == NULL);
    
    /* Test saveptr preservation */
    strcpy(str1, "A,B,C");
    token = strtok_r(str1, ",", &saveptr1);
    assert(strcmp(token, "A") == 0);
    
    /* saveptr1 should now point to "B,C" */
    assert(saveptr1 != NULL);
    assert(strcmp(saveptr1, "B,C") == 0);
    
    /* Test whitespace handling */
    strcpy(str1, "  Word1  Word2  ");
    
    token = strtok_r(str1, " ", &saveptr1);
    assert(strcmp(token, "Word1") == 0);
    
    token = strtok_r(NULL, " ", &saveptr1);
    assert(strcmp(token, "Word2") == 0);
    
    token = strtok_r(NULL, " ", &saveptr1);
    assert(token == NULL);
    
    TEST_PASSED("strtok_r()");
}

/* Test strxfrm() function */
void test_strxfrm(void) {
    char dst[BUFFER_SIZE];
    size_t len;
    char *old_locale;
    
    TEST_START("strxfrm()");
    
    /* Save current locale */
    old_locale = setlocale(LC_COLLATE, NULL);
    
    /* Test in "C" locale */
    setlocale(LC_COLLATE, "C");
    
    /* Test basic transformation */
    len = strxfrm(dst, "Hello", BUFFER_SIZE);
    assert(len > 0);
    assert(len < BUFFER_SIZE);
    
    /* In C locale, strxfrm often just copies */
    assert(strcmp(dst, "Hello") == 0);
    
    /* Test length calculation (dst = NULL) */
    len = strxfrm(NULL, "Test String", 0);
    assert(len > 0); /* Should return needed length */
    
    /* Test with exact buffer size */
    len = strxfrm(dst, "Test", 5);
    assert(len >= 4); /* May need more than strlen */
    
    /* Test buffer too small */
    len = strxfrm(dst, "Long String", 5);
    /* Return value should be >= 11 (needed size) */
    assert(len >= 11);
    
    /* Test empty string */
    len = strxfrm(dst, "", BUFFER_SIZE);
    assert(len == 0);
    assert(dst[0] == '\0');
    
    /* Test that transformed strings maintain order */
    char trans1[BUFFER_SIZE], trans2[BUFFER_SIZE];
    
    strxfrm(trans1, "ABC", BUFFER_SIZE);
    strxfrm(trans2, "XYZ", BUFFER_SIZE);
    
    /* Comparing transformed strings should give same order */
    assert(strcmp(trans1, trans2) < 0);
    
    /* Test with numbers */
    strxfrm(trans1, "123", BUFFER_SIZE);
    strxfrm(trans2, "456", BUFFER_SIZE);
    assert(strcmp(trans1, trans2) < 0);
    
    /* Try different locale if available */
    if (setlocale(LC_COLLATE, "en_US.UTF-8") != NULL ||
        setlocale(LC_COLLATE, "en_US") != NULL) {
        
        /* Locale-specific transformations may differ */
        len = strxfrm(dst, "café", BUFFER_SIZE);
        /* Transformation may handle accented characters specially */
    }
    
    /* Restore locale */
    setlocale(LC_COLLATE, old_locale);
    
    /* Test buffer boundary */
    len = strxfrm(dst, "Test", 4);
    /* Should transform but may truncate */
    
    /* Test consistency */
    char s1[] = "String1";
    char s2[] = "String2";
    char t1[BUFFER_SIZE], t2[BUFFER_SIZE];
    
    strxfrm(t1, s1, BUFFER_SIZE);
    strxfrm(t2, s2, BUFFER_SIZE);
    
    /* Order should be preserved */
    int cmp_orig = strcmp(s1, s2);
    int cmp_trans = strcmp(t1, t2);
    
    assert((cmp_orig < 0 && cmp_trans < 0) ||
           (cmp_orig > 0 && cmp_trans > 0) ||
           (cmp_orig == 0 && cmp_trans == 0));
    
    TEST_PASSED("strxfrm()");
}

/* Test strerror() function */
void test_strerror(void) {
    char *msg;
    int saved_errno;
    
    TEST_START("strerror()");
    
    /* Save errno */
    saved_errno = errno;
    
    /* Test known error codes */
    msg = strerror(0);
    assert(msg != NULL);
    /* Often "Success" or similar */
    
    msg = strerror(EINVAL);
    assert(msg != NULL);
    assert(strlen(msg) > 0);
    /* Often "Invalid argument" */
    
    msg = strerror(ENOENT);
    assert(msg != NULL);
    assert(strlen(msg) > 0);
    /* Often "No such file or directory" */
    
    msg = strerror(ENOMEM);
    assert(msg != NULL);
    /* Often "Out of memory" or "Cannot allocate memory" */
    
    msg = strerror(EACCES);
    assert(msg != NULL);
    /* Often "Permission denied" */
    
    /* Test that strerror doesn't change errno */
    errno = EINVAL;
    msg = strerror(ENOENT);
    assert(errno == EINVAL);
    
    /* Test invalid error number */
    msg = strerror(99999);
    assert(msg != NULL);
    /* Should return some message, often "Unknown error" */
    
    /* Test negative error number */
    msg = strerror(-1);
    assert(msg != NULL);
    
    /* Test that different errors give different messages */
    char *msg1 = strerror(EINVAL);
    char *msg2 = strerror(ENOENT);
    
    if (EINVAL != ENOENT) {
        /* Messages should be different */
        assert(strcmp(msg1, msg2) != 0);
    }
    
    /* Test sequential calls */
    msg1 = strerror(EINVAL);
    msg2 = strerror(EINVAL);
    assert(strcmp(msg1, msg2) == 0); /* Same error = same message */
    
    /* Restore errno */
    errno = saved_errno;
    
    TEST_PASSED("strerror()");
}

/* Test strerror_r() function */
void test_strerror_r(void) {
    char buffer[BUFFER_SIZE];
    int result;
    int saved_errno;
    
    TEST_START("strerror_r()");
    
    /* Save errno */
    saved_errno = errno;
    
    /* Test known error codes */
    result = strerror_r(EINVAL, buffer, BUFFER_SIZE);
    if (result == -1 && errno == ENOSYS) {
        printf("  strerror_r() not implemented (ENOSYS)\n");
        errno = saved_errno;
        return;
    }
    
    /* POSIX version returns 0 on success */
    assert(result == 0);
    assert(strlen(buffer) > 0);
    
    /* Test different error */
    result = strerror_r(ENOENT, buffer, BUFFER_SIZE);
    assert(result == 0);
    assert(strlen(buffer) > 0);
    
    /* Test buffer too small */
    result = strerror_r(EINVAL, buffer, 1);
    assert(result == ERANGE);
    
    /* Test with reasonable small buffer */
    result = strerror_r(0, buffer, 10);
    /* May succeed if message fits, or fail with ERANGE */
    
    /* Test invalid error number */
    result = strerror_r(99999, buffer, BUFFER_SIZE);
    /* Should either succeed with "Unknown error" or return EINVAL */
    assert(result == 0 || result == EINVAL);
    
    /* Test that errno is preserved */
    errno = EACCES;
    result = strerror_r(EINVAL, buffer, BUFFER_SIZE);
    if (result == 0) {
        assert(errno == EACCES); /* Should not change */
    }
    
    /* Test thread safety by using multiple buffers */
    char buf1[BUFFER_SIZE], buf2[BUFFER_SIZE];
    
    result = strerror_r(EINVAL, buf1, BUFFER_SIZE);
    assert(result == 0);
    
    result = strerror_r(ENOENT, buf2, BUFFER_SIZE);
    assert(result == 0);
    
    /* Buffers should contain different messages */
    assert(strcmp(buf1, buf2) != 0);
    
    /* Test empty buffer */
    buffer[0] = 'X'; /* Mark buffer */
    result = strerror_r(EINVAL, buffer, 0);
    assert(result == ERANGE);
    assert(buffer[0] == 'X'); /* Should not be modified */
    
    /* Test minimum buffer sizes */
    size_t size;
    for (size = 1; size < 100; size++) {
        result = strerror_r(EINVAL, buffer, size);
        if (result == 0) {
            /* Found minimum size */
            assert(strlen(buffer) < size);
            break;
        }
        assert(result == ERANGE);
    }
    
    /* Restore errno */
    errno = saved_errno;
    
    TEST_PASSED("strerror_r()");
}

/* Test edge cases */
void test_misc_edge_cases(void) {
    char buffer[BUFFER_SIZE];
    char *token;
    char *saveptr;
    
    TEST_START("miscellaneous edge cases");
    
    /* Test strtok with changing string */
    strcpy(buffer, "A,B,C");
    token = strtok(buffer, ",");
    assert(strcmp(token, "A") == 0);
    
    /* Modify the string (undefined behavior but test robustness) */
    /* Don't actually do this in real code! */
    
    /* Test strtok_r with NULL saveptr initially */
    strcpy(buffer, "Test");
    saveptr = NULL;
    token = strtok_r(buffer, ",", &saveptr);
    assert(token != NULL);
    assert(saveptr != NULL);
    
    /* Test strxfrm with overlapping buffers (undefined) */
    /* Don't test this as it's undefined behavior */
    
    /* Test strerror with all standard error codes */
    int errors[] = {
        E2BIG, EACCES, EADDRINUSE, EADDRNOTAVAIL, EAFNOSUPPORT,
        EAGAIN, EALREADY, EBADF, EBADMSG, EBUSY, ECANCELED,
        ECHILD, ECONNABORTED, ECONNREFUSED, ECONNRESET, EDEADLK,
        EDESTADDRREQ, EDOM, EDQUOT, EEXIST, EFAULT, EFBIG,
        EHOSTUNREACH, EIDRM, EILSEQ, EINPROGRESS, EINTR, EINVAL,
        EIO, EISCONN, EISDIR, ELOOP, EMFILE, EMLINK, EMSGSIZE,
        EMULTIHOP, ENAMETOOLONG, ENETDOWN, ENETRESET, ENETUNREACH,
        ENFILE, ENOBUFS, ENODATA, ENODEV, ENOENT, ENOEXEC,
        ENOLCK, ENOLINK, ENOMEM, ENOMSG, ENOPROTOOPT, ENOSPC,
        ENOSR, ENOSTR, ENOSYS, ENOTCONN, ENOTDIR, ENOTEMPTY,
        ENOTSOCK, ENOTSUP, ENOTTY, ENXIO, EOPNOTSUPP, EOVERFLOW,
        EPERM, EPIPE, EPROTO, EPROTONOSUPPORT, EPROTOTYPE,
        ERANGE, EROFS, ESPIPE, ESRCH, ESTALE, ETIME, ETIMEDOUT,
        ETXTBSY, EWOULDBLOCK, EXDEV
    };
    
    size_t i;
    for (i = 0; i < sizeof(errors)/sizeof(errors[0]); i++) {
        char *msg = strerror(errors[i]);
        assert(msg != NULL);
        assert(strlen(msg) > 0);
    }
    
    TEST_PASSED("Edge case");
}

/* Test common patterns */
void test_misc_patterns(void) {
    char buffer[BUFFER_SIZE];
    char *token;
    char *saveptr;
    
    TEST_START("miscellaneous patterns");
    
    /* Pattern 1: CSV parsing */
    strcpy(buffer, "Name,Age,City");
    char *fields[10];
    int field_count = 0;
    
    token = strtok_r(buffer, ",", &saveptr);
    while (token != NULL && field_count < 10) {
        fields[field_count++] = token;
        token = strtok_r(NULL, ",", &saveptr);
    }
    
    assert(field_count == 3);
    assert(strcmp(fields[0], "Name") == 0);
    assert(strcmp(fields[1], "Age") == 0);
    assert(strcmp(fields[2], "City") == 0);
    
    /* Pattern 2: Command line parsing */
    strcpy(buffer, "command -flag1 arg1 -flag2 arg2");
    
    token = strtok_r(buffer, " ", &saveptr);
    assert(strcmp(token, "command") == 0);
    
    while ((token = strtok_r(NULL, " ", &saveptr)) != NULL) {
        if (token[0] == '-') {
            /* Flag */
            char *arg = strtok_r(NULL, " ", &saveptr);
            assert(arg != NULL);
        }
    }
    
    /* Pattern 3: Path parsing */
    strcpy(buffer, "/home/user/documents/file.txt");
    char *components[10];
    int comp_count = 0;
    
    token = strtok_r(buffer, "/", &saveptr);
    while (token != NULL && comp_count < 10) {
        components[comp_count++] = token;
        token = strtok_r(NULL, "/", &saveptr);
    }
    
    assert(comp_count == 4);
    assert(strcmp(components[3], "file.txt") == 0);
    
    /* Pattern 4: Error handling with strerror */
    FILE *fp = fopen("/nonexistent/file.txt", "r");
    if (fp == NULL) {
        char *error_msg = strerror(errno);
        assert(error_msg != NULL);
        /* Would typically log: "Error: <error_msg>" */
    }
    
    /* Pattern 5: Safe error message formatting */
    char error_buffer[256];
    int err = ENOENT;
    
    if (strerror_r(err, error_buffer, sizeof(error_buffer)) == 0) {
        /* Use error_buffer safely */
        assert(strlen(error_buffer) > 0);
    }
    
    /* Pattern 6: Locale-aware sorting preparation */
    const char *strings[] = {"apple", "Banana", "cherry"};
    char transformed[3][BUFFER_SIZE];
    size_t i;
    
    for (i = 0; i < 3; i++) {
        strxfrm(transformed[i], strings[i], BUFFER_SIZE);
    }
    /* Can now use strcmp on transformed strings for locale-aware sort */
    
    /* Pattern 7: Configuration file parsing */
    strcpy(buffer, "key1=value1;key2=value2;key3=value3");
    
    token = strtok_r(buffer, ";", &saveptr);
    while (token != NULL) {
        char *equals = strchr(token, '=');
        if (equals != NULL) {
            *equals = '\0';
            char *key = token;
            char *value = equals + 1;
            /* Process key-value pair */
            assert(key != NULL && value != NULL);
        }
        token = strtok_r(NULL, ";", &saveptr);
    }
    
    TEST_PASSED("Pattern");
}

void run_tests(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 string.h Miscellaneous Test Suite ===" COLOR_RESET "\n\n");
    
    /* Run tests */
    test_strtok();
    test_strtok_r();
    test_strxfrm();
    test_strerror();
    test_strerror_r();
    test_misc_edge_cases();
    test_misc_patterns();
    
    TEST_SUITE_PASSED("miscellaneous string");
}
