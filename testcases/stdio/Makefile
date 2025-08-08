# Makefile for PSE51 stdio.h test suite
# This makefile builds and runs all stdio tests for mlibc

# Compiler settings
CC ?= gcc
CFLAGS = -Wall -Wextra -g -O0
CFLAGS += -I../../include
# For PC tests, use system libc
LDFLAGS =

# Architecture (default: current system)
ARCH ?= $(shell uname -m)

# Test programs
TESTS = test_file_operations \
        test_formatted_io \
        test_char_io \
        test_string_io \
        test_misc_stdio

TEST_OBJS = $(TESTS:%=%.o)
TEST_BINS = $(TESTS:%=%.out)

# Test output directory
TEST_OUTPUT_DIR = test_results

.PHONY: all clean test run help

# Default target
all: $(TEST_BINS)

# Build rules
%.out: %.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Run all tests
test: $(TEST_BINS)
	@echo "=== Running PSE51 stdio.h Test Suite ==="
	@mkdir -p $(TEST_OUTPUT_DIR)
	@failed=0; \
	for test in $(TEST_BINS); do \
		echo "\n--- Running $$test ---"; \
		if ./$$test > $(TEST_OUTPUT_DIR)/$$test.log 2>&1; then \
			echo "✓ $$test PASSED"; \
			cat $(TEST_OUTPUT_DIR)/$$test.log | grep "=== All"; \
		else \
			echo "✗ $$test FAILED (see $(TEST_OUTPUT_DIR)/$$test.log)"; \
			failed=$$((failed + 1)); \
		fi; \
	done; \
	echo "\n=== Test Summary ==="; \
	echo "Total tests: $(words $(TEST_BINS))"; \
	echo "Failed: $$failed"; \
	if [ $$failed -eq 0 ]; then \
		echo "All tests PASSED!"; \
	else \
		echo "Some tests FAILED!"; \
		exit 1; \
	fi

# Run individual tests
run-%: %.out
	@echo "Running $<..."
	./$<

# Clean up
clean:
	rm -f $(TEST_OBJS) $(TEST_BINS)
	rm -rf $(TEST_OUTPUT_DIR)
	rm -f test_*.txt  # Clean up test files

# Help
help:
	@echo "PSE51 stdio.h Test Suite Makefile"
	@echo ""
	@echo "Usage:"
	@echo "  make            - Build all tests"
	@echo "  make test       - Build and run all tests"
	@echo "  make run-NAME   - Run specific test (e.g., make run-test_file_operations)"
	@echo "  make clean      - Remove all build artifacts"
	@echo "  make help       - Show this help message"
	@echo ""
	@echo "Variables:"
	@echo "  CC=compiler     - Set C compiler (default: gcc)"
	@echo "  ARCH=arch       - Set architecture (default: current system)"
	@echo ""
	@echo "Available tests:"
	@for test in $(TESTS); do echo "  - $$test"; done

# Individual test targets for convenience
test-file: test_file_operations.out
	./test_file_operations.out

test-format: test_formatted_io.out
	./test_formatted_io.out

test-char: test_char_io.out
	./test_char_io.out

test-string: test_string_io.out
	./test_string_io.out

test-misc: test_misc_stdio.out
	./test_misc_stdio.out

# Verbose test run (shows all output)
test-verbose: $(TEST_BINS)
	@echo "=== Running PSE51 stdio.h Test Suite (Verbose) ==="
	@for test in $(TEST_BINS); do \
		echo "\n--- Running $$test ---"; \
		./$$test || true; \
	done

# Dependencies
test_file_operations.o: test_file_operations.c
test_formatted_io.o: test_formatted_io.c
test_char_io.o: test_char_io.c
test_string_io.o: test_string_io.c
test_misc_stdio.o: test_misc_stdio.c