# Makefile for stdio tests (multiple test files)

include ../common/Makefile.common

# Test programs
TESTS = test_file_operations \
        test_formatted_io \
        test_char_io \
        test_string_io \
        test_misc_stdio

PC_TARGETS = $(TESTS:%=$(TEST_RESULTS_DIR)/%)
EMBEDDED_TARGETS = $(TESTS:%=%_embedded.elf)

# Default target
all: pc

pc: $(TEST_RESULTS_DIR) $(PC_TARGETS)

embedded: $(BUILD_DIR) $(EMBEDDED_TARGETS)

# Build rules for each PC test with main.c
$(TEST_RESULTS_DIR)/%: %.c $(COMMON_DIR)/main.c
	@echo "Building $@..."
	$(PC_CC) $(PC_CFLAGS) -o $@ $^ $(PC_LDFLAGS)
	@echo "$(GREEN)✓ PC test $@ built$(NC)"

# Embedded build rules for each test
$(BUILD_DIR)/embedded_startup.o: $(COMMON_DIR)/embedded_startup.c
	@echo "Building embedded startup..."
	$(EMBEDDED_CC) $(EMBEDDED_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/main.o: $(COMMON_DIR)/main.c
	@echo "Building main.o..."
	$(EMBEDDED_CC) $(EMBEDDED_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: %.c
	@echo "Building $@..."
	$(EMBEDDED_CC) $(EMBEDDED_CFLAGS) -c -o $@ $<

%_embedded.elf: $(BUILD_DIR)/embedded_startup.o $(BUILD_DIR)/%.o $(BUILD_DIR)/main.o
	@echo "Linking $@..."
	$(EMBEDDED_CC) $(EMBEDDED_CFLAGS) -o $@ $^ $(EMBEDDED_LDFLAGS)
	@echo "$(GREEN)✓ Embedded test $@ built$(NC)"

# Run all PC tests
test: $(PC_TARGETS)
	@echo "Running stdio tests..."
	@for test in $(PC_TARGETS); do \
		echo "Running $$test..."; \
		$$test || true; \
	done

# Run specific embedded test
run: test_file_operations_embedded.elf
	@echo "Running embedded test in QEMU ($(EMBEDDED_ARCH))..."
	@$(QEMU_SYSTEM) $(QEMU_ARGS) -kernel $<

# Clean - use parent clean rule
clean:
	@rm -rf $(TEST_RESULTS_DIR) $(BUILD_DIR)
	@rm -f *.o *.out *.elf
	@rm -f test_*.txt test_*.tmp
	@echo "$(GREEN)✓ stdio clean completed$(NC)"

.PHONY: all pc embedded test run clean