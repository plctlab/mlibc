# Define variables
PROJECT_PATH := .

# Select architecture (you can set this manually or through environment variables)
ARCH := arm
TOOLCHAIN := arm-none-eabi
ARCH_FLAGS :=
DEFINE_FLAGS :=

# Configuration for different architectures
ifeq ($(ARCH), arm)
	TOOLCHAIN_PREFIX := arm-linux-musleabi_for_x86_64-pc-linux-gnu
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
XGCC_DIR := ../build/${TOOLCHAIN_PREFIX}/obj_gcc/gcc/
CC = $(XGCC_DIR)/xgcc -B $(XGCC_DIR)
AR := ~/work/mlibc-toolchain/build/arm-linux-musleabi_for_x86_64-pc-linux-gnu/obj_binutils/binutils/ar
CFLAGS := $(DEFINE_FLAGS) $(ARCH_FLAGS) -nostdlib -ffreestanding -nostdinc -Wl,-Map=cc.map
INCLUDES := -I $(PROJECT_PATH)/include

# Source files
SRC_FILES := $(wildcard $(PROJECT_PATH)/src/dummy/*.c) \
             $(wildcard $(PROJECT_PATH)/src/misc/*.c) \
             $(wildcard $(PROJECT_PATH)/src/stdio/*.c) \
             $(wildcard $(PROJECT_PATH)/src/stdlib/*.c) \
             $(wildcard $(PROJECT_PATH)/src/time/*.c)

CRT_FILES := $(wildcard $(PROJECT_PATH)/crt/$(ARCH)/*.c) \
             $(wildcard $(PROJECT_PATH)/crt/$(ARCH)/*.s)

# Target settings
TARGET := libmlibc.a
TARGET_DIR_ARCH := build/$(ARCH)
LIB_PATH := mlibc/lib

# Object files directory
OBJ_DIR := $(TARGET_DIR_ARCH)/obj
CRTOBJ_DIR := $(TARGET_DIR_ARCH)/crtobj
OBJ_FILES := $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRC_FILES)))
CRTOBJ_FILES := $(patsubst %.c,$(CRTOBJ_DIR)/%.o,$(notdir $(CRT_FILES)))
CRTOBJ_FILES := $(patsubst %.s,$(CRTOBJ_DIR)/%.o,$(CRTOBJ_FILES))

# Build rules
.PHONY: all clean after_build

all: $(TARGET_DIR_ARCH)/$(TARGET)

# Compile each .c file into .o in the obj directory
$(OBJ_DIR)/%.o: $(PROJECT_PATH)/src/*/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(CRTOBJ_DIR)/%.o: $(PROJECT_PATH)/crt/$(ARCH)/%.c
	@mkdir -p $(CRTOBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(CRTOBJ_DIR)/%.o: $(PROJECT_PATH)/crt/$(ARCH)/%.s
	@mkdir -p $(CRTOBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET_DIR_ARCH)/$(TARGET): $(OBJ_FILES) $(CRTOBJ_FILES)
	@mkdir -p $(TARGET_DIR_ARCH)
	$(AR) rcs $@ $(OBJ_FILES)

clean:
	@rm -rf $(TARGET_DIR_ARCH)
