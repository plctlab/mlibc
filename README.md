[中文](README_zh.md) | **English**

# Embedded libc

mlibc is a small C library for embedded systems, RTOS integration, and bare-metal programs. It focuses on simple code, low resource usage, and cross-compilation for ARM, AArch64, and RISC-V targets.

## Mlibc Features

+ Lightweight libc for small systems
+ MIT licensed and open source
+ Supports ARM, AArch64, RISC-V 32-bit, and RISC-V 64-bit builds
+ Provides common C library headers and implementations such as `stdio`, `stdlib`, `string`, `ctype`, `time`, and small POSIX-style interfaces
+ Works with Make for standalone builds and SCons for RT-Thread package integration
+ Includes QEMU bare-metal hello-world examples, PSE51 testcases, and GCC toolchain build scripts

## File Structure

[mlibc File Structure](ARCH.md)

```
├── arch                 -- Architecture-specific SCons hook and optimized implementation area
├── crt                  -- Startup code for ARM, AArch64, RISC-V 32/64, and common crt code
├── include              -- Public headers
│   └── sys              -- System and POSIX-style headers, usually included as <sys/name.h>
├── src                  -- Library source files
│   ├── dummy            -- Weak/stub platform hooks such as errno, lock, POSIX, and sbrk
│   ├── internal         -- Internal headers used by mlibc
│   ├── misc             -- Small modules such as string, assert, env, uname, setjmp, and inttypes
│   ├── stdio            -- Standard I/O implementation
│   ├── stdlib           -- Standard library, TLSF allocator, qsort, and process helpers
│   └── time             -- Time functions
├── helloworld           -- Bare-metal hello-world example
│   └── qemu             -- QEMU board link scripts and UART headers
├── mkconfigs            -- Make build fragments
│   └── qemu             -- QEMU board build configuration
├── testcases            -- PSE51/POSIX.13-oriented tests
└── toolchain            -- GCC/Binutils build scripts, Docker files, and patches
```

## Background

### Our Expectations

● mlibc targets embedded systems that need a small, understandable libc instead of a full hosted libc.

● It can be used with bare-metal programs and RTOS environments such as RT-Thread.

● The current build supports ARM, AArch64, RISC-V 32-bit, and RISC-V 64-bit. RISC-V remains an important target.

● The repository keeps standalone Make builds, RT-Thread SCons integration, QEMU examples, tests, and toolchain scripts together.

## Our Plans

● QEMU bare-metal hello world is available for five boards.

● Basic crt, string, stdio, stdlib, time, and platform stub code is in place.

● Ongoing work is to improve libc coverage, tighten PSE51 compatibility, and make more embedded tests pass on real QEMU targets.

# Quick Start

## mlibc Library Compilation

### Development Environment

For a standalone library build, you need `make` and a cross toolchain. `MLIBC_TOOLCHAIN` must be the compiler prefix, including the final dash.

Example:

```bash
export MLIBC_TOOLCHAIN=/path/to/arm-linux-eabi_for_x86_64-pc-linux-gnu/bin/arm-linux-eabi-
```

If `MLIBC_TOOLCHAIN` is not set, the Makefile defaults to `./toolchain/arm-linux-eabi_for_x86_64-pc-linux-gnu/bin/arm-linux-eabi-`.

### Compilation Steps

**Compile the C library**

```bash
# Build the default ARM library
make mlibc ARCH=arm

# Other supported values:
# ARCH=aarch64
# ARCH=riscv32
# ARCH=riscv64
```

The static library is generated as `build/<ARCH>/libmlibc.a`. When installing it into a toolchain sysroot, it is normally copied or renamed to `libc.a`.

**Compile crt objects**

```bash
make crt ARCH=arm
```

The crt objects are generated in `build/<ARCH>/crtobj/`. For ARM and AArch64 this includes `crt0.o`, `crti.o`, and `crtn.o`; for RISC-V targets the current crt object is `crt0.o`.

**Build both**

```bash
make ARCH=arm
```

## Mlibc Development/Test Environment Setup

### PSE51 Testcases

The `testcases/` directory contains tests for common PSE51/POSIX.13 headers such as `assert.h`, `ctype.h`, `fcntl.h`, `locale.h`, `signal.h`, `stat.h`, `stdio.h`, `stdlib.h`, `string.h`, `time.h`, `unistd.h`, and `utsname.h`.

```bash
cd testcases

# Build PC tests
make pc-build

# Run PC tests
make pc-test

# Use the helper scripts
./build pc
./test pc
```

Embedded test support is being prepared in the common Makefile, but the current per-test directories only provide PC test Makefiles. See [testcases/README.md](testcases/README.md) and [testcases/PSE51.md](testcases/PSE51.md) for details.

### Running RT-Thread on QEMU

#### Development Environment

Follow the current RT-Thread documentation for the Env tool, packages, BSP setup, and QEMU usage:

- RT-Thread website: https://www.rt-thread.org/
- RT-Thread repository: https://github.com/RT-Thread/rt-thread

mlibc is integrated as an RT-Thread package through `SConscript` when `PKG_USING_MLIBC` is enabled.

### vexpress-a9 + RT-Thread

#### Macro Configuration

In `rt-thread/bsp/qemu-vexpress-a9`, open Env and run `menuconfig`.

Recommended configuration:

- RT-Thread Components
  - DFS: device virtual file system
    - Select DFS v1.0 if your RT-Thread version still provides this option

Enable the mlibc package:

- RT-Thread online packages
  - system packages
    - `mlibc: Embedded libc, especially for RISC-V`

Then download packages and build:

```bash
. ~/.env/env.sh
pkgs --update
scons -j12
```

RT-Thread package menus and BSP options may change over time. If a menu item is missing, use the current RT-Thread package documentation as the source of truth.

### Spark + RT-Thread

#### Development Environment

Use the RT-Thread `stm32/stm32f407-rt-spark` BSP and run `menuconfig` from Env.

#### Macro Configuration

Optional file system setup:

- Hardware Drivers Config
  - Onboard Peripheral Drivers
    - Enable File System

Optional FatFs setup:

- RT-Thread Components
  - DFS: device virtual file system
    - Enable elm-chan FatFs
      - Set maximum sector size to 4096 when required by the board storage

Enable and download the mlibc package:

```bash
. ~/.env/env.sh
pkgs --update
scons -j12
```

### QEMU-Bare Metal Development

#### Development Environment

> make + QEMU + cross toolchain

The repository currently has QEMU bare-metal hello-world build support for five boards:

| QEMU Board | Hardware Architecture | Expected Toolchain Prefix |
| ---------- | --------------------- | ------------------------- |
| qemu-vexpress-a9 | ARM Cortex-A9 | `arm-linux-eabi-` |
| qemu-mps3-an536 | ARM Cortex-R52 | `arm-linux-eabi-` |
| qemu-virt-aarch64 | AArch64 Cortex-A53 | `aarch64-linux-gnu-` |
| qemu-virt-riscv32 | RISC-V 32-bit | `riscv32-unknown-elf-` |
| qemu-virt-riscv64 | RISC-V 64-bit | `riscv64-unknown-elf-` |

#### Usage Steps

1. Configure the toolchain prefix.

Linux:

```bash
export MLIBC_TOOLCHAIN=/path/to/toolchain/bin/arm-linux-eabi-
```

Windows PowerShell:

```powershell
[System.Environment]::SetEnvironmentVariable("MLIBC_TOOLCHAIN", "C:\path\to\toolchain\bin\arm-linux-eabi-", "User")
```

Restart the terminal after changing the Windows user environment.

2. Build the example from the repository root.

```bash
make qemu-hello QEMU_BOARD=qemu-vexpress-a9 ARCH=arm
```

The ELF is generated under `build/<ARCH>/qemu/<QEMU_BOARD>/<QEMU_BOARD>.elf`, for example `build/arm/qemu/qemu-vexpress-a9/qemu-vexpress-a9.elf`.

3. Run it with the matching `qemu-system-*` command for your board. The repository currently provides board link scripts and UART headers, but no checked-in QEMU run script.

Board build commands:

| QEMU Board | Virtual Device | Build Command |
| ---------- | -------------- | ------------- |
| qemu-vexpress-a9 | vexpress-a9 | `make qemu-hello QEMU_BOARD=qemu-vexpress-a9 ARCH=arm` |
| qemu-mps3-an536 | mps3-an536 | `make qemu-hello QEMU_BOARD=qemu-mps3-an536 ARCH=arm` |
| qemu-virt-aarch64 | virt-aarch64 | `make qemu-hello QEMU_BOARD=qemu-virt-aarch64 ARCH=aarch64` |
| qemu-virt-riscv32 | virt-riscv32 | `make qemu-hello QEMU_BOARD=qemu-virt-riscv32 ARCH=riscv32` |
| qemu-virt-riscv64 | virt-riscv64 | `make qemu-hello QEMU_BOARD=qemu-virt-riscv64 ARCH=riscv64` |

## Pre-built GCC Toolchains

Pre-built GCC toolchains with mlibc are published from GitHub Actions when release workflows are run.

### Downloading Pre-built Toolchains

Visit the [Releases page](https://github.com/plctlab/mlibc/releases) and download the package for your target:

- **ARM**: `arm-linux-eabi_for_x86_64-pc-linux-gnu-*.tar.gz`
- **RISC-V 32-bit**: `riscv32-unknown-elf_for_x86_64-pc-linux-gnu-*.tar.gz`
- **RISC-V 64-bit**: `riscv64-unknown-elf_for_x86_64-pc-linux-gnu-*.tar.gz`
- **AArch64**: `aarch64-linux-gnu_for_x86_64-pc-linux-gnu-*.tar.gz`

### Installation

```bash
tar -xzf <toolchain-tarball>.tar.gz
export PATH=/path/to/toolchain/bin:$PATH

# Example
arm-linux-eabi-gcc --version
```

### Building Toolchains Yourself

The toolchain build uses GCC 12.2.0 and Binutils 2.39, with patches stored under `toolchain/patches/`.

#### Using Docker (Recommended)

See [toolchain/README.md](toolchain/README.md) for Docker-based local build steps.

#### Using GitHub Actions

The repository includes workflows for source releases and toolchain builds:

- `release.yml`: create a source archive release
- `build-toolchain.yml`: build all or selected toolchains
- `build-single-toolchain.yml`: build one toolchain for testing

For more information, see [.github/workflows/README.md](.github/workflows/README.md).

# License Agreement

mlibc is released under the MIT License. It can be used and modified in commercial or non-commercial projects as long as the MIT license notice is preserved.

# Contribution

+ How to

  1. Fork the repository
  1. Create a `Feat_xxx` branch
  1. Commit your changes
  1. Create a Pull Request

# License

mlibc follows the MIT License free software license. See [LICENSE](LICENSE) for the full license text.
