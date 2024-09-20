# Define variables
PROJECT_PATH := E:\1-RTT-Workspace\Workspace\Code\libc\mlibc

# Select architecture (you can set this manually or through environment variables)
ARCH := arm
TOOLCHAIN := arm-none-eabi
ARCH_FLAGS :=
DEFINE_FLAGS :=

# Configuration for different architectures
ifeq ($(ARCH), arm)
	TOOLCHAIN := arm-none-eabi
	ARCH_FLAGS :=
	DEFINE_FLAGS :=
endif

ifeq ($(ARCH), aarch64)
	TOOLCHAIN := aarch64-none-elf
	ARCH_FLAGS := -mstrict-align
	DEFINE_FLAGS :=
endif

ifeq ($(ARCH), riscv64)
	TOOLCHAIN := riscv64-unknown-elf
	ARCH_FLAGS := -mcmodel=medany
	DEFINE_FLAGS :=
endif

ifeq ($(ARCH), riscv32)
	TOOLCHAIN := riscv32-unknown-elf
	ARCH_FLAGS := -mcmodel=medany -march=rv32imac
	DEFINE_FLAGS :=
endif

# Compiler and flags
CC := $(TOOLCHAIN)-gcc
AR := $(TOOLCHAIN)-ar
CFLAGS := $(DEFINE_FLAGS) $(ARCH_FLAGS) -nostdlib -ffreestanding -nostdinc -Wl,-Map=cc.map
INCLUDES := -I$(PROJECT_PATH)/include

# Source files
SRC_FILES := $(wildcard $(PROJECT_PATH)/src/dummy/*.c) \
             $(wildcard $(PROJECT_PATH)/src/misc/*.c) \
             $(wildcard $(PROJECT_PATH)/src/stdio/*.c) \
             $(wildcard $(PROJECT_PATH)/src/stdlib/*.c) \
             $(wildcard $(PROJECT_PATH)/src/time/*.c)

# Target settings
TARGET := libmlibc.a
TARGET_DIR_ARCH := build/$(ARCH)
LIB_PATH := mlibc/lib

# Object files directory
OBJ_DIR := $(TARGET_DIR_ARCH)/obj
OBJ_FILES := $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRC_FILES)))

# Build rules
.PHONY: all clean after_build

all: $(TARGET_DIR_ARCH)/$(TARGET)

# Compile each .c file into .o in the obj directory
$(OBJ_DIR)/%.o: $(PROJECT_PATH)/src/*/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET_DIR_ARCH)/$(TARGET): $(OBJ_FILES)
	@mkdir -p $(TARGET_DIR_ARCH)
	$(AR) rcs $@ $(OBJ_FILES)

clean:
	@rm -rf $(TARGET_DIR_ARCH)