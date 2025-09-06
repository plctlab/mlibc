/*
 * test_colors.h - ANSI color codes for test output
 */

#ifndef TEST_COLORS_H
#define TEST_COLORS_H

/* ANSI color codes */
#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_YELLOW  "\033[0;33m"
#define COLOR_BLUE    "\033[0;34m"
#define COLOR_MAGENTA "\033[0;35m"
#define COLOR_CYAN    "\033[0;36m"
#define COLOR_RESET   "\033[0m"

/* Bold colors */
#define COLOR_BOLD_RED     "\033[1;31m"
#define COLOR_BOLD_GREEN   "\033[1;32m"
#define COLOR_BOLD_YELLOW  "\033[1;33m"
#define COLOR_BOLD_BLUE    "\033[1;34m"
#define COLOR_BOLD_MAGENTA "\033[1;35m"
#define COLOR_BOLD_CYAN    "\033[1;36m"

/* Test status indicators */
#define TEST_PASS COLOR_GREEN "✓" COLOR_RESET
#define TEST_FAIL COLOR_RED "✗" COLOR_RESET

/* Test output macros */
#define TEST_START(name) printf(COLOR_BLUE "Testing %s..." COLOR_RESET "\n", name)
#define TEST_PASSED(name) printf("  %s %s tests passed!\n", TEST_PASS, name)
#define TEST_FAILED(name) printf("  %s %s tests failed!\n", TEST_FAIL, name)
#define TEST_SUITE_PASSED(name) printf(COLOR_BOLD_GREEN "\n%s All %s tests passed!\n" COLOR_RESET, TEST_PASS, name)
#define TEST_SUITE_FAILED(name) printf(COLOR_BOLD_RED "\n%s Some %s tests failed!\n" COLOR_RESET, TEST_FAIL, name)

#endif /* TEST_COLORS_H */