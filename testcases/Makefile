# Master Makefile for mlibc PSE51 Test Suite
# Supports both PC and embedded test environments

# Test directories
TEST_DIRS := assert ctype fcntl locale signal stat stdio stdlib string time unistd utsname

# Colors for output
RED := \033[0;31m
GREEN := \033[0;32m
YELLOW := \033[1;33m
BLUE := \033[0;34m
NC := \033[0m

# Default target
.PHONY: all
all: help

# Help message
.PHONY: help
help:
	@echo "$(BLUE)=== mlibc PSE51 Test Suite ===$(NC)"
	@echo ""
	@echo "$(YELLOW)PC Environment Tests:$(NC)"
	@echo "  make pc-build      - Build all PC tests"
	@echo "  make pc-test       - Run all PC tests"
	@echo "  make pc-clean      - Clean PC test builds"
	@echo ""
	@echo "$(YELLOW)Embedded Environment Tests:$(NC)"
	@echo "  make embedded-build - Build all embedded tests"
	@echo "  make embedded-test  - Run all embedded tests in QEMU"
	@echo "  make embedded-clean - Clean embedded test builds"
	@echo ""
	@echo "$(YELLOW)Combined Commands:$(NC)"
	@echo "  make build-all     - Build both PC and embedded tests"
	@echo "  make test-all      - Run both PC and embedded tests"
	@echo "  make clean-all     - Clean all builds"
	@echo ""
	@echo "$(YELLOW)Quick Commands:$(NC)"
	@echo "  make test          - Run all PC tests (quick)"
	@echo "  make embedded      - Build and run embedded tests"
	@echo ""
	@echo "$(YELLOW)Individual Tests:$(NC)"
	@echo "  make pc-test-<dir>      - Run specific PC test (e.g., pc-test-stdio)"
	@echo "  make embedded-test-<dir> - Run specific embedded test"

# PC Environment Targets
.PHONY: pc-build
pc-build:
	@echo "$(BLUE)Building all PC tests...$(NC)"
	@for dir in $(TEST_DIRS); do \
		if [ -f $$dir/Makefile ]; then \
			echo "$(YELLOW)Building $$dir...$(NC)"; \
			$(MAKE) -C $$dir pc --no-print-directory || exit 1; \
		fi; \
	done
	@echo "$(GREEN)All PC tests built successfully!$(NC)"

.PHONY: pc-test
pc-test: pc-build
	@echo "$(BLUE)Running all PC tests...$(NC)"
	@total=0; passed=0; failed=0; \
	for dir in $(TEST_DIRS); do \
		if [ -f $$dir/Makefile ]; then \
			echo "$(YELLOW)\nTesting $$dir...$(NC)"; \
			if $(MAKE) -C $$dir test --no-print-directory; then \
				passed=$$((passed + 1)); \
			else \
				failed=$$((failed + 1)); \
				echo "$(RED)$$dir tests failed!$(NC)"; \
			fi; \
			total=$$((total + 1)); \
		fi; \
	done; \
	echo "\n$(BLUE)=== PC Test Summary ===$(NC)"; \
	echo "Total: $$total, Passed: $(GREEN)$$passed$(NC), Failed: $(RED)$$failed$(NC)"

.PHONY: pc-clean
pc-clean:
	@echo "$(BLUE)Cleaning PC test builds...$(NC)"
	@for dir in $(TEST_DIRS); do \
		if [ -f $$dir/Makefile ]; then \
			$(MAKE) -C $$dir clean --no-print-directory; \
		fi; \
	done
	@echo "$(GREEN)PC tests cleaned!$(NC)"

# Embedded Environment Targets
.PHONY: embedded-build
embedded-build: check-mlibc
	@echo "$(BLUE)Building all embedded tests...$(NC)"
	@for dir in $(TEST_DIRS); do \
		if [ -f $$dir/Makefile.embedded ]; then \
			echo "$(YELLOW)Building embedded $$dir...$(NC)"; \
			$(MAKE) -C $$dir -f Makefile.embedded --no-print-directory || exit 1; \
		fi; \
	done
	@echo "$(GREEN)All embedded tests built successfully!$(NC)"

.PHONY: embedded-test
embedded-test: embedded-build
	@echo "$(BLUE)Running embedded tests...$(NC)"
	@./run_all_embedded_tests.sh

.PHONY: embedded-clean
embedded-clean:
	@echo "$(BLUE)Cleaning embedded test builds...$(NC)"
	@for dir in $(TEST_DIRS); do \
		if [ -f $$dir/Makefile.embedded ]; then \
			$(MAKE) -C $$dir -f Makefile.embedded clean --no-print-directory; \
		fi; \
	done
	@echo "$(GREEN)Embedded tests cleaned!$(NC)"

# Combined targets
.PHONY: build-all
build-all: pc-build embedded-build

.PHONY: test-all
test-all: pc-test embedded-test

.PHONY: clean-all
clean-all: pc-clean embedded-clean
	@rm -f embedded_test_results.log embedded_test_summary.txt

# Quick shortcuts
.PHONY: test
test: pc-test

.PHONY: embedded
embedded: embedded-test

# Individual test targets
define INDIVIDUAL_TEST_RULES
.PHONY: pc-test-$(1)
pc-test-$(1):
	@echo "$(BLUE)Testing $(1) (PC)...$(NC)"
	@if [ -f $(1)/Makefile ]; then \
		$(MAKE) -C $(1) test; \
	else \
		echo "$(RED)No PC tests for $(1)$(NC)"; \
	fi

.PHONY: embedded-test-$(1)
embedded-test-$(1): check-mlibc
	@echo "$(BLUE)Testing $(1) (embedded)...$(NC)"
	@if [ -f $(1)/run_embedded_test.sh ]; then \
		cd $(1) && ./run_embedded_test.sh; \
	else \
		echo "$(RED)No embedded tests for $(1)$(NC)"; \
	fi
endef

$(foreach dir,$(TEST_DIRS),$(eval $(call INDIVIDUAL_TEST_RULES,$(dir))))

# Check if mlibc is built
.PHONY: check-mlibc
check-mlibc:
	@if [ ! -f ../build/riscv32/libmlibc.a ]; then \
		echo "$(YELLOW)Building mlibc for RISC-V 32-bit...$(NC)"; \
		$(MAKE) -C .. ARCH=riscv32 || { \
			echo "$(RED)Failed to build mlibc!$(NC)"; \
			echo "Please ensure you have the RISC-V toolchain installed."; \
			echo "Set MLIBC_TOOLCHAIN environment variable if needed."; \
			exit 1; \
		}; \
	fi

# Clean everything including logs
.PHONY: distclean
distclean: clean-all
	@rm -rf */build */test_results
	@rm -f */*.o */*.log
	@echo "$(GREEN)All test artifacts removed!$(NC)"

# Show test status
.PHONY: status
status:
	@echo "$(BLUE)=== Test Suite Status ===$(NC)"
	@echo "\n$(YELLOW)PC Tests:$(NC)"
	@for dir in $(TEST_DIRS); do \
		if [ -f $$dir/Makefile ]; then \
			if [ -d $$dir/test_results ]; then \
				echo "  $$dir: $(GREEN)Built$(NC)"; \
			else \
				echo "  $$dir: Not built"; \
			fi; \
		fi; \
	done
	@echo "\n$(YELLOW)Embedded Tests:$(NC)"
	@for dir in $(TEST_DIRS); do \
		if [ -f $$dir/Makefile.embedded ]; then \
			if [ -d $$dir/build ]; then \
				echo "  $$dir: $(GREEN)Built$(NC)"; \
			else \
				echo "  $$dir: Not built"; \
			fi; \
		fi; \
	done

# Install dependencies (informational)
.PHONY: deps
deps:
	@echo "$(BLUE)=== Dependencies ===$(NC)"
	@echo ""
	@echo "$(YELLOW)For PC tests:$(NC)"
	@echo "  - GCC or compatible C compiler"
	@echo "  - POSIX-compliant system"
	@echo ""
	@echo "$(YELLOW)For embedded tests:$(NC)"
	@echo "  - RISC-V 32-bit toolchain (riscv32-unknown-elf-gcc)"
	@echo "  - QEMU system emulator (qemu-system-riscv32)"
	@echo ""
	@echo "$(YELLOW)Ubuntu/Debian installation:$(NC)"
	@echo "  sudo apt-get install gcc-riscv64-unknown-elf qemu-system-misc"
	@echo ""
	@echo "$(YELLOW)Environment variables:$(NC)"
	@echo "  export MLIBC_TOOLCHAIN=riscv32-unknown-elf-"