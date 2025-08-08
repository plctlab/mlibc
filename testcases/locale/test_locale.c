/*
 * test_locale.c - PSE51 locale.h locale handling test suite
 * Tests: setlocale, localeconv
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for locale support
 */

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <errno.h>

#include "../test_colors.h"
/* Test setlocale() function */
void test_setlocale(void) {
    char *result;
    char *saved_locale;
    
    TEST_START("setlocale()");
    
    /* Query current locale */
    result = setlocale(LC_ALL, NULL);
    assert(result != NULL);
    printf("  Current locale: %s\n", result);
    
    /* Save current locale */
    saved_locale = strdup(result);
    assert(saved_locale != NULL);
    
    /* Test setting C locale */
    result = setlocale(LC_ALL, "C");
    assert(result != NULL);
    assert(strcmp(result, "C") == 0);
    
    /* Verify locale was changed */
    result = setlocale(LC_ALL, NULL);
    assert(strcmp(result, "C") == 0);
    
    /* Test setting POSIX locale (equivalent to C) */
    result = setlocale(LC_ALL, "POSIX");
    if (result != NULL) {
        assert(strcmp(result, "POSIX") == 0 || strcmp(result, "C") == 0);
    }
    
    /* Test individual categories */
    result = setlocale(LC_CTYPE, "C");
    assert(result != NULL);
    
    result = setlocale(LC_NUMERIC, "C");
    assert(result != NULL);
    
    result = setlocale(LC_TIME, "C");
    assert(result != NULL);
    
    result = setlocale(LC_COLLATE, "C");
    assert(result != NULL);
    
    result = setlocale(LC_MONETARY, "C");
    assert(result != NULL);
    
    result = setlocale(LC_MESSAGES, "C");
    if (result == NULL && errno == ENOSYS) {
        printf("  LC_MESSAGES not supported\n");
    }
    
    /* Test empty string (implementation-defined native locale) */
    result = setlocale(LC_ALL, "");
    if (result != NULL) {
        printf("  Native locale: %s\n", result);
    }
    
    /* Test invalid locale */
    result = setlocale(LC_ALL, "invalid_locale_name_12345");
    if (result == NULL) {
        /* Failed to set invalid locale - good */
        /* Locale should remain unchanged */
        result = setlocale(LC_ALL, NULL);
        assert(result != NULL);
    }
    
    /* Test querying individual categories */
    result = setlocale(LC_CTYPE, NULL);
    assert(result != NULL);
    
    result = setlocale(LC_NUMERIC, NULL);
    assert(result != NULL);
    
    /* Restore original locale */
    setlocale(LC_ALL, saved_locale);
    free(saved_locale);
    
    TEST_PASSED("setlocale()");
}

/* Test localeconv() function */
void test_localeconv(void) {
    struct lconv *lc;
    char *old_locale;
    
    TEST_START("localeconv()");
    
    /* Save current locale */
    old_locale = setlocale(LC_ALL, NULL);
    old_locale = strdup(old_locale);
    
    /* Set to C locale for predictable results */
    setlocale(LC_ALL, "C");
    
    /* Get locale conventions */
    lc = localeconv();
    assert(lc != NULL);
    
    /* Test numeric formatting in C locale */
    assert(lc->decimal_point != NULL);
    assert(strcmp(lc->decimal_point, ".") == 0);
    
    assert(lc->thousands_sep != NULL);
    assert(strcmp(lc->thousands_sep, "") == 0);
    
    assert(lc->grouping != NULL);
    
    /* Test monetary formatting in C locale */
    assert(lc->currency_symbol != NULL);
    assert(strcmp(lc->currency_symbol, "") == 0);
    
    assert(lc->mon_decimal_point != NULL);
    assert(strcmp(lc->mon_decimal_point, "") == 0);
    
    assert(lc->mon_thousands_sep != NULL);
    assert(strcmp(lc->mon_thousands_sep, "") == 0);
    
    assert(lc->mon_grouping != NULL);
    
    assert(lc->positive_sign != NULL);
    assert(strcmp(lc->positive_sign, "") == 0);
    
    assert(lc->negative_sign != NULL);
    assert(strcmp(lc->negative_sign, "") == 0);
    
    /* Test integer monetary values */
    assert(lc->int_frac_digits == CHAR_MAX);
    assert(lc->frac_digits == CHAR_MAX);
    assert(lc->p_cs_precedes == CHAR_MAX);
    assert(lc->p_sep_by_space == CHAR_MAX);
    assert(lc->n_cs_precedes == CHAR_MAX);
    assert(lc->n_sep_by_space == CHAR_MAX);
    assert(lc->p_sign_posn == CHAR_MAX);
    assert(lc->n_sign_posn == CHAR_MAX);
    
    /* Additional POSIX fields */
    assert(lc->int_curr_symbol != NULL);
    assert(strcmp(lc->int_curr_symbol, "") == 0);
    
    assert(lc->int_p_cs_precedes == CHAR_MAX);
    assert(lc->int_n_cs_precedes == CHAR_MAX);
    assert(lc->int_p_sep_by_space == CHAR_MAX);
    assert(lc->int_n_sep_by_space == CHAR_MAX);
    assert(lc->int_p_sign_posn == CHAR_MAX);
    assert(lc->int_n_sign_posn == CHAR_MAX);
    
    /* Test that localeconv returns same pointer on subsequent calls */
    struct lconv *lc2 = localeconv();
    assert(lc2 == lc);
    
    /* Try other locales if available */
    if (setlocale(LC_ALL, "en_US.UTF-8") != NULL ||
        setlocale(LC_ALL, "en_US") != NULL) {
        
        lc = localeconv();
        assert(lc != NULL);
        
        /* In US locale, some values might be different */
        printf("  US locale decimal point: '%s'\n", lc->decimal_point);
        printf("  US locale thousands separator: '%s'\n", lc->thousands_sep);
        printf("  US locale currency symbol: '%s'\n", lc->currency_symbol);
        
        /* Decimal point should still be "." in US locale */
        assert(strcmp(lc->decimal_point, ".") == 0);
        
        /* Currency symbol might be "$" */
        if (strlen(lc->currency_symbol) > 0) {
            printf("  Currency symbol is set: '%s'\n", lc->currency_symbol);
        }
    }
    
    /* Restore original locale */
    setlocale(LC_ALL, old_locale);
    free(old_locale);
    
    TEST_PASSED("localeconv()");
}

/* Test locale categories */
void test_locale_categories(void) {
    char *result;
    
    TEST_START("locale categories");
    
    /* Set different locales for different categories */
    setlocale(LC_ALL, "C");
    
    /* Test LC_CTYPE */
    result = setlocale(LC_CTYPE, "C");
    assert(result != NULL);
    
    /* Test LC_NUMERIC */
    result = setlocale(LC_NUMERIC, "C");
    assert(result != NULL);
    
    /* Test LC_TIME */
    result = setlocale(LC_TIME, "C");
    assert(result != NULL);
    
    /* Test LC_COLLATE */
    result = setlocale(LC_COLLATE, "C");
    assert(result != NULL);
    
    /* Test LC_MONETARY */
    result = setlocale(LC_MONETARY, "C");
    assert(result != NULL);
    
    /* Test LC_MESSAGES (might not be supported) */
    result = setlocale(LC_MESSAGES, "C");
    if (result == NULL) {
        printf("  LC_MESSAGES not supported\n");
    }
    
    /* Query mixed locale */
    result = setlocale(LC_ALL, NULL);
    assert(result != NULL);
    /* Result might be "C" or a composite like "C/C/C/C/C/C" */
    
    TEST_PASSED("Locale category");
}

/* Test locale-dependent behavior */
void test_locale_behavior(void) {
    struct lconv *lc;
    char buf[100];
    
    TEST_START("locale-dependent behavior");
    
    /* Set C locale */
    setlocale(LC_ALL, "C");
    
    /* Test numeric formatting */
    lc = localeconv();
    sprintf(buf, "%.2f", 1234.56);
    /* In C locale, should use "." as decimal point */
    assert(strchr(buf, '.') != NULL);
    
    /* Test thousands grouping */
    /* Note: Standard sprintf doesn't apply grouping */
    
    /* Test that changing numeric locale affects decimal point */
    /* This would require a locale with different decimal point */
    
    TEST_PASSED("Locale behavior");
}

/* Test thread safety considerations */
void test_locale_thread_safety(void) {
    char *locale1, *locale2;
    
    TEST_START("locale thread safety considerations");
    
    /* Note: setlocale is NOT thread-safe */
    /* In multi-threaded programs, should use uselocale() if available */
    
    /* Test that setlocale affects global state */
    setlocale(LC_ALL, "C");
    locale1 = setlocale(LC_ALL, NULL);
    
    setlocale(LC_ALL, "POSIX");
    locale2 = setlocale(LC_ALL, NULL);
    
    /* Both queries should show the current global locale */
    
    TEST_PASSED("Thread safety consideration");
}

/* Test edge cases */
void test_locale_edge_cases(void) {
    char *result;
    struct lconv *lc;
    
    TEST_START("locale edge cases");
    
    /* Test NULL locale string */
    result = setlocale(LC_ALL, NULL);
    assert(result != NULL); /* Should query current locale */
    
    /* Test empty category array */
    setlocale(LC_ALL, "C");
    
    /* Test very long locale name */
    char long_name[1024];
    memset(long_name, 'X', sizeof(long_name) - 1);
    long_name[sizeof(long_name) - 1] = '\0';
    
    result = setlocale(LC_ALL, long_name);
    /* Should fail */
    assert(result == NULL);
    
    /* Verify locale unchanged */
    result = setlocale(LC_ALL, NULL);
    assert(strcmp(result, "C") == 0);
    
    /* Test invalid category */
    result = setlocale(99999, "C");
    assert(result == NULL);
    
    /* Test localeconv with all categories set differently */
    setlocale(LC_NUMERIC, "C");
    setlocale(LC_MONETARY, "C");
    
    lc = localeconv();
    assert(lc != NULL);
    
    /* Values should still be consistent */
    assert(lc->decimal_point != NULL);
    assert(lc->currency_symbol != NULL);
    
    TEST_PASSED("Edge case");
}

/* Test common locale patterns */
void test_locale_patterns(void) {
    char *saved_locale;
    
    TEST_START("common locale patterns");
    
    /* Pattern 1: Save and restore locale */
    saved_locale = setlocale(LC_ALL, NULL);
    saved_locale = strdup(saved_locale);
    
    /* Do some work in C locale */
    setlocale(LC_ALL, "C");
    /* ... work ... */
    
    /* Restore original locale */
    setlocale(LC_ALL, saved_locale);
    free(saved_locale);
    
    /* Pattern 2: Temporary locale change */
    saved_locale = setlocale(LC_NUMERIC, NULL);
    saved_locale = strdup(saved_locale);
    
    setlocale(LC_NUMERIC, "C");
    /* Do numeric processing with "." as decimal point */
    
    setlocale(LC_NUMERIC, saved_locale);
    free(saved_locale);
    
    /* Pattern 3: Check if locale is available */
    if (setlocale(LC_ALL, "en_US.UTF-8") != NULL) {
        printf("  UTF-8 locale is available\n");
        setlocale(LC_ALL, "C"); /* Reset */
    } else {
        printf("  UTF-8 locale not available\n");
    }
    
    /* Pattern 4: Initialize to native locale */
    setlocale(LC_ALL, "");
    /* Program now uses system's native locale */
    
    /* Reset to C for predictable behavior */
    setlocale(LC_ALL, "C");
    
    TEST_PASSED("Locale pattern");
}

/* Test locale string formats */
void test_locale_strings(void) {
    char *locale;
    
    TEST_START("locale string formats");
    
    /* Test standard locale names */
    struct {
        const char *name;
        int should_work;
    } test_locales[] = {
        {"C", 1},
        {"POSIX", 1},
        {"", 1}, /* Native locale */
        {"en_US", 0}, /* Might work */
        {"en_US.UTF-8", 0}, /* Might work */
        {"en_US.ISO-8859-1", 0}, /* Might work */
        {"ja_JP.UTF-8", 0}, /* Might work */
        {"invalid", 0}, /* Should not work */
        {NULL, 0}
    };
    
    for (int i = 0; test_locales[i].name != NULL; i++) {
        locale = setlocale(LC_ALL, test_locales[i].name);
        
        if (test_locales[i].should_work) {
            assert(locale != NULL);
            printf("  Locale '%s' set successfully\n", test_locales[i].name);
        } else {
            if (locale != NULL) {
                printf("  Optional locale '%s' is available\n", test_locales[i].name);
            } else {
                printf("  Optional locale '%s' not available\n", test_locales[i].name);
            }
        }
    }
    
    /* Reset to C locale */
    setlocale(LC_ALL, "C");
    
    TEST_PASSED("Locale string");
}

int main(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 locale.h Test Suite ===" COLOR_RESET "\n\n");
    
    /* Run tests */
    test_setlocale();
    test_localeconv();
    test_locale_categories();
    test_locale_behavior();
    test_locale_thread_safety();
    test_locale_edge_cases();
    test_locale_patterns();
    test_locale_strings();
    
    TEST_SUITE_PASSED("locale.h");
    return 0;
}
