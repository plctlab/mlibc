/*
 * test_environment_utils.c - PSE51 stdlib.h environment and utility test suite
 * Tests: getenv, setenv, unsetenv, system, qsort, bsearch
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for environment and utility functions
 */

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../test_colors.h"
/* Comparison function for qsort/bsearch testing */
int int_compare(const void *a, const void *b) {
    const int *ia = (const int *)a;
    const int *ib = (const int *)b;
    return (*ia - *ib);
}

/* Reverse comparison function */
int int_compare_reverse(const void *a, const void *b) {
    const int *ia = (const int *)a;
    const int *ib = (const int *)b;
    return (*ib - *ia);
}

/* String comparison function */
int string_compare(const void *a, const void *b) {
    const char **sa = (const char **)a;
    const char **sb = (const char **)b;
    return strcmp(*sa, *sb);
}

/* Structure for testing */
struct test_struct {
    int key;
    char value[32];
};

int struct_compare(const void *a, const void *b) {
    const struct test_struct *sa = (const struct test_struct *)a;
    const struct test_struct *sb = (const struct test_struct *)b;
    return sa->key - sb->key;
}

/* Test getenv() function */
void test_getenv(void) {
    char *value;
    
    TEST_START("getenv()");
    
    /* Test getting PATH (usually set) */
    value = getenv("PATH");
    /* PATH may or may not be set */
    if (value != NULL) {
        assert(strlen(value) > 0);
    }
    
    /* Test getting HOME */
    value = getenv("HOME");
    /* HOME may or may not be set */
    
    /* Test non-existent variable */
    value = getenv("THIS_VARIABLE_SHOULD_NOT_EXIST_12345");
    assert(value == NULL);
    
    /* Test empty name */
    value = getenv("");
    assert(value == NULL);
    
    /* Test NULL name (if implementation allows) */
    /* Note: This might crash on some implementations */
    /* value = getenv(NULL); */
    
    /* Test case sensitivity */
    value = getenv("PATH");
    char *value2 = getenv("path");
    /* On POSIX systems, these should be different */
    
    /* Test with equals sign in name (invalid) */
    value = getenv("VAR=VALUE");
    assert(value == NULL);
    
    TEST_PASSED("getenv()");
}

/* Test setenv() function */
void test_setenv(void) {
    int ret;
    char *value;
    
    TEST_START("setenv()");
    
    /* Test basic setenv */
    ret = setenv("TEST_VAR", "test_value", 1);
    if (ret == -1 && errno == ENOSYS) {
        printf("  setenv() not implemented (ENOSYS)\n");
        return;
    }
    assert(ret == 0);
    
    value = getenv("TEST_VAR");
    assert(value != NULL);
    assert(strcmp(value, "test_value") == 0);
    
    /* Test overwrite = 0 (don't overwrite) */
    ret = setenv("TEST_VAR", "new_value", 0);
    assert(ret == 0);
    
    value = getenv("TEST_VAR");
    assert(value != NULL);
    assert(strcmp(value, "test_value") == 0); /* Should not change */
    
    /* Test overwrite = 1 (do overwrite) */
    ret = setenv("TEST_VAR", "new_value", 1);
    assert(ret == 0);
    
    value = getenv("TEST_VAR");
    assert(value != NULL);
    assert(strcmp(value, "new_value") == 0); /* Should change */
    
    /* Test empty value */
    ret = setenv("EMPTY_VAR", "", 1);
    assert(ret == 0);
    
    value = getenv("EMPTY_VAR");
    assert(value != NULL);
    assert(strcmp(value, "") == 0);
    
    /* Test invalid names */
    errno = 0;
    ret = setenv("", "value", 1);
    assert(ret == -1);
    assert(errno == EINVAL);
    
    errno = 0;
    ret = setenv("VAR=NAME", "value", 1);
    assert(ret == -1);
    assert(errno == EINVAL);
    
    /* Test very long value */
    char long_value[1024];
    memset(long_value, 'A', sizeof(long_value) - 1);
    long_value[sizeof(long_value) - 1] = '\0';
    
    ret = setenv("LONG_VAR", long_value, 1);
    assert(ret == 0);
    
    value = getenv("LONG_VAR");
    assert(value != NULL);
    assert(strlen(value) == sizeof(long_value) - 1);
    
    /* Test special characters in value */
    ret = setenv("SPECIAL_VAR", "value with spaces and $pecial ch@rs!", 1);
    assert(ret == 0);
    
    value = getenv("SPECIAL_VAR");
    assert(value != NULL);
    assert(strcmp(value, "value with spaces and $pecial ch@rs!") == 0);
    
    TEST_PASSED("setenv()");
}

/* Test unsetenv() function */
void test_unsetenv(void) {
    int ret;
    char *value;
    
    TEST_START("unsetenv()");
    
    /* Set a variable first */
    ret = setenv("UNSET_TEST", "value", 1);
    if (ret == -1 && errno == ENOSYS) {
        printf("  unsetenv() test skipped (setenv not implemented)\n");
        return;
    }
    assert(ret == 0);
    
    /* Verify it's set */
    value = getenv("UNSET_TEST");
    assert(value != NULL);
    
    /* Unset it */
    ret = unsetenv("UNSET_TEST");
    if (ret == -1 && errno == ENOSYS) {
        printf("  unsetenv() not implemented (ENOSYS)\n");
        return;
    }
    assert(ret == 0);
    
    /* Verify it's gone */
    value = getenv("UNSET_TEST");
    assert(value == NULL);
    
    /* Test unsetting non-existent variable */
    ret = unsetenv("NON_EXISTENT_VAR_12345");
    assert(ret == 0); /* Should succeed */
    
    /* Test invalid names */
    errno = 0;
    ret = unsetenv("");
    assert(ret == -1);
    assert(errno == EINVAL);
    
    errno = 0;
    ret = unsetenv("VAR=NAME");
    assert(ret == -1);
    assert(errno == EINVAL);
    
    /* Test multiple unset */
    setenv("UNSET1", "value1", 1);
    setenv("UNSET2", "value2", 1);
    setenv("UNSET3", "value3", 1);
    
    ret = unsetenv("UNSET1");
    assert(ret == 0);
    ret = unsetenv("UNSET2");
    assert(ret == 0);
    ret = unsetenv("UNSET3");
    assert(ret == 0);
    
    assert(getenv("UNSET1") == NULL);
    assert(getenv("UNSET2") == NULL);
    assert(getenv("UNSET3") == NULL);
    
    TEST_PASSED("unsetenv()");
}

/* Test system() function */
void test_system(void) {
    int ret;
    
    TEST_START("system()");
    
    /* Test if shell is available */
    ret = system(NULL);
    if (ret == 0) {
        printf("  No shell available\n");
        return;
    }
    
    /* Test simple command */
    ret = system("true");
    if (ret == -1) {
        if (errno == ENOSYS) {
            printf("  system() not implemented (ENOSYS)\n");
            return;
        }
        perror("system");
        return;
    }
    assert(WIFEXITED(ret));
    assert(WEXITSTATUS(ret) == 0);
    
    /* Test command with non-zero exit */
    ret = system("false");
    assert(ret != -1);
    assert(WIFEXITED(ret));
    assert(WEXITSTATUS(ret) != 0);
    
    /* Test command with output redirection */
    ret = system("echo 'test output' > test_system.tmp");
    assert(ret != -1);
    assert(WIFEXITED(ret));
    assert(WEXITSTATUS(ret) == 0);
    
    /* Verify file was created */
    FILE *fp = fopen("test_system.tmp", "r");
    if (fp) {
        char buf[100];
        assert(fgets(buf, sizeof(buf), fp) != NULL);
        assert(strncmp(buf, "test output", 11) == 0);
        fclose(fp);
        remove("test_system.tmp");
    }
    
    /* Test command with exit code */
    ret = system("exit 42");
    assert(ret != -1);
    assert(WIFEXITED(ret));
    assert(WEXITSTATUS(ret) == 42);
    
    TEST_PASSED("system()");
}

/* Test qsort() function */
void test_qsort(void) {
    int i;
    
    TEST_START("qsort()");
    
    /* Test sorting integers */
    int arr1[] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    size_t n1 = sizeof(arr1) / sizeof(arr1[0]);
    
    qsort(arr1, n1, sizeof(int), int_compare);
    
    for (i = 0; i < n1; i++) {
        assert(arr1[i] == i);
    }
    
    /* Test reverse sort */
    qsort(arr1, n1, sizeof(int), int_compare_reverse);
    
    for (i = 0; i < n1; i++) {
        assert(arr1[i] == (n1 - 1 - i));
    }
    
    /* Test sorting strings */
    const char *strings[] = {"zebra", "apple", "banana", "cherry", "date"};
    size_t n2 = sizeof(strings) / sizeof(strings[0]);
    
    qsort(strings, n2, sizeof(char *), string_compare);
    
    assert(strcmp(strings[0], "apple") == 0);
    assert(strcmp(strings[1], "banana") == 0);
    assert(strcmp(strings[2], "cherry") == 0);
    assert(strcmp(strings[3], "date") == 0);
    assert(strcmp(strings[4], "zebra") == 0);
    
    /* Test sorting structures */
    struct test_struct structs[] = {
        {5, "five"},
        {2, "two"},
        {8, "eight"},
        {1, "one"},
        {3, "three"}
    };
    size_t n3 = sizeof(structs) / sizeof(structs[0]);
    
    qsort(structs, n3, sizeof(struct test_struct), struct_compare);
    
    assert(structs[0].key == 1);
    assert(structs[1].key == 2);
    assert(structs[2].key == 3);
    assert(structs[3].key == 5);
    assert(structs[4].key == 8);
    
    /* Test empty array */
    int empty[1];
    qsort(empty, 0, sizeof(int), int_compare);
    /* Should not crash */
    
    /* Test single element */
    int single[] = {42};
    qsort(single, 1, sizeof(int), int_compare);
    assert(single[0] == 42);
    
    /* Test already sorted */
    int sorted[] = {1, 2, 3, 4, 5};
    qsort(sorted, 5, sizeof(int), int_compare);
    for (i = 0; i < 5; i++) {
        assert(sorted[i] == i + 1);
    }
    
    /* Test all same elements */
    int same[] = {7, 7, 7, 7, 7};
    qsort(same, 5, sizeof(int), int_compare);
    for (i = 0; i < 5; i++) {
        assert(same[i] == 7);
    }
    
    /* Test large array */
    int *large = (int *)malloc(1000 * sizeof(int));
    assert(large != NULL);
    
    /* Fill with random values */
    for (i = 0; i < 1000; i++) {
        large[i] = 1000 - i;
    }
    
    qsort(large, 1000, sizeof(int), int_compare);
    
    /* Verify sorted */
    for (i = 0; i < 999; i++) {
        assert(large[i] <= large[i + 1]);
    }
    
    free(large);
    
    TEST_PASSED("qsort()");
}

/* Test bsearch() function */
void test_bsearch(void) {
    int sorted[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    size_t n = sizeof(sorted) / sizeof(sorted[0]);
    int key;
    int *result;
    
    TEST_START("bsearch()");
    
    /* Test finding existing elements */
    key = 7;
    result = (int *)bsearch(&key, sorted, n, sizeof(int), int_compare);
    assert(result != NULL);
    assert(*result == 7);
    assert(result == &sorted[3]);
    
    key = 1;
    result = (int *)bsearch(&key, sorted, n, sizeof(int), int_compare);
    assert(result != NULL);
    assert(*result == 1);
    assert(result == &sorted[0]);
    
    key = 19;
    result = (int *)bsearch(&key, sorted, n, sizeof(int), int_compare);
    assert(result != NULL);
    assert(*result == 19);
    assert(result == &sorted[9]);
    
    /* Test not finding elements */
    key = 0;
    result = (int *)bsearch(&key, sorted, n, sizeof(int), int_compare);
    assert(result == NULL);
    
    key = 20;
    result = (int *)bsearch(&key, sorted, n, sizeof(int), int_compare);
    assert(result == NULL);
    
    key = 8;
    result = (int *)bsearch(&key, sorted, n, sizeof(int), int_compare);
    assert(result == NULL);
    
    /* Test with strings */
    const char *str_array[] = {"apple", "banana", "cherry", "date", "elderberry"};
    size_t n2 = sizeof(str_array) / sizeof(str_array[0]);
    const char *search_key = "cherry";
    
    const char **str_result = (const char **)bsearch(&search_key, str_array, n2,
                                                     sizeof(char *), string_compare);
    assert(str_result != NULL);
    assert(strcmp(*str_result, "cherry") == 0);
    
    search_key = "grape";
    str_result = (const char **)bsearch(&search_key, str_array, n2,
                                        sizeof(char *), string_compare);
    assert(str_result == NULL);
    
    /* Test with structures */
    struct test_struct struct_array[] = {
        {1, "one"},
        {3, "three"},
        {5, "five"},
        {7, "seven"},
        {9, "nine"}
    };
    size_t n3 = sizeof(struct_array) / sizeof(struct_array[0]);
    struct test_struct search_struct = {5, ""};
    
    struct test_struct *struct_result = (struct test_struct *)bsearch(&search_struct,
                                                                     struct_array, n3,
                                                                     sizeof(struct test_struct),
                                                                     struct_compare);
    assert(struct_result != NULL);
    assert(struct_result->key == 5);
    assert(strcmp(struct_result->value, "five") == 0);
    
    /* Test empty array */
    key = 5;
    result = (int *)bsearch(&key, sorted, 0, sizeof(int), int_compare);
    assert(result == NULL);
    
    /* Test single element array */
    int single[] = {42};
    key = 42;
    result = (int *)bsearch(&key, single, 1, sizeof(int), int_compare);
    assert(result != NULL);
    assert(*result == 42);
    
    key = 41;
    result = (int *)bsearch(&key, single, 1, sizeof(int), int_compare);
    assert(result == NULL);
    
    /* Test large array */
    int *large = (int *)malloc(1000 * sizeof(int));
    assert(large != NULL);
    
    int i;
    for (i = 0; i < 1000; i++) {
        large[i] = i * 2; /* Even numbers 0, 2, 4, ... */
    }
    
    /* Find existing */
    key = 500;
    result = (int *)bsearch(&key, large, 1000, sizeof(int), int_compare);
    assert(result != NULL);
    assert(*result == 500);
    
    /* Find non-existing */
    key = 501;
    result = (int *)bsearch(&key, large, 1000, sizeof(int), int_compare);
    assert(result == NULL);
    
    free(large);
    
    TEST_PASSED("bsearch()");
}

/* Test edge cases for qsort and bsearch */
void test_sort_search_edge_cases(void) {
    TEST_START("qsort/bsearch edge cases");
    
    /* Test qsort stability (not guaranteed but check behavior) */
    struct {
        int primary;
        int secondary;
    } data[] = {
        {1, 1}, {2, 1}, {1, 2}, {2, 2}, {1, 3}
    };
    
    qsort(data, 5, sizeof(data[0]), int_compare);
    
    /* All elements with primary=1 should come before primary=2 */
    int i;
    for (i = 0; i < 3; i++) {
        assert(data[i].primary == 1);
    }
    for (i = 3; i < 5; i++) {
        assert(data[i].primary == 2);
    }
    
    /* Test NULL base with zero count (should be safe) */
    qsort(NULL, 0, sizeof(int), int_compare);
    
    int key = 5;
    void *result = bsearch(&key, NULL, 0, sizeof(int), int_compare);
    assert(result == NULL);
    
    TEST_PASSED("Edge case");
}

/* Test environment manipulation patterns */
void test_environment_patterns(void) {
    TEST_START("environment manipulation patterns");
    
    /* Save and restore pattern */
    char *old_value = getenv("TEST_PATTERN");
    char *saved = NULL;
    if (old_value) {
        saved = strdup(old_value);
    }
    
    /* Modify environment */
    setenv("TEST_PATTERN", "new_value", 1);
    assert(strcmp(getenv("TEST_PATTERN"), "new_value") == 0);
    
    /* Restore */
    if (saved) {
        setenv("TEST_PATTERN", saved, 1);
        free(saved);
    } else {
        unsetenv("TEST_PATTERN");
    }
    
    /* Verify restoration */
    char *restored = getenv("TEST_PATTERN");
    if (old_value == NULL) {
        assert(restored == NULL);
    } else {
        assert(strcmp(restored, old_value) == 0);
    }
    
    TEST_PASSED("Environment pattern");
}

int main(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 stdlib.h Environment and Utilities Test Suite ===" COLOR_RESET "\n\n");
    
    /* Run tests */
    test_getenv();
    test_setenv();
    test_unsetenv();
    test_system();
    test_qsort();
    test_bsearch();
    test_sort_search_edge_cases();
    test_environment_patterns();
    
    TEST_SUITE_PASSED("environment and utility");
    return 0;
}
