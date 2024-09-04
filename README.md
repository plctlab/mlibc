[中文](README_zh.md) | **English** 

# Embedded libc

Embedded libc, a libc library adapted for embedded systems and bare metal environments.

## Mlibc Features

+ Low resource usage
+ OpenSource
+ Especially for RISC-V
+ Clear and understandable code structure
+ Scalable
+ Highly portable

## File Structure

[mlibc File Structure](ARCH.md)

```
├───arch                -- Hardware-specific optimized implementations
├───crt                 -- Hardware-specific startup code
├───include             -- Header files
│   └───sys             -- System-related header files, typically included as <sys/head.h>
├───src                 -- Source files
│   ├───internal        -- Internal header files
│   ├───misc            -- Miscellaneous, contains modules that can be implemented in a single file
│   ├───stdio           -- Standard IO module
│   └───stdlib          -- Standard utility library module
├───helloworld          -- Helloworld testcase
│	└───qemu			-- QEMU bare-metal config
│		└───qemu-device	-- Specific to the QEMU virtual machine device, related scripts and header files
├───xscript             -- Scripts related to xmake
└───toolchains          -- xmake scripts related to toolchains
```

## Background

### Our Expectations

● mlibc can support multiple embedded toolchains, including gcc (arm/risc-v) and even LLVM compilers.

● Designed for low-resource systems, providing excellent support for embedded real-time operating systems (such as RT-Thread) and bare metal.

● Optimized for RISC-V 32/64, compatible with mainstream RISC-V MCUs.

● Uses xmake and scons for building.

● Reserve

## Our Plans

● Use QEMU/RISC-V 32GC simulation to output the first "hello world" (completed).

● Add the basic crt.s, string, and printf functions for the bare-metal version of mlibc (completed).

● Improve mlibc based on this environment.

# Quick Start

## Mlibc Development/Test Environment Setup

### Running RT-Thread on QEMU

#### Development Environment

Tutorial for setting up the environment on Windows:

https://github.com/RT-Thread/rt-thread/blob/master/documentation/quick-start/quick_start_qemu/quick_start_qemu_windows.md

By following this tutorial, you can run RT-Thread in a Windows environment.

### vexpress-a9 + RT-Thread

#### Macro Configuration

Navigate to the `rt-thread\bsp\qemu-vexpress-a9` folder, open **env**, and enter **menuconfig** in the command line to enter the configuration interface.

**Switch DFS v2.0 to DFS v1.0:**

- RT-Thread Components
  - DFS: device virtual file system
    - The version of DFS (DFS v1.0)

**Download the mlibc package:**

- RT-Thread online packages
  - system packages
    - Select the sixth option from the bottom: `mlibc: Embedded libc, especially for RISC-V`

After finishing, you can exit the configuration page and enter **scons -j12** in the command line to compile.

### Spark + RT-Thread

#### Development Environment

For those unsure where to download the source code, you can refer to the above Windows environment setup tutorial.

Navigate to the `rt-thread\bsp\stm32\stm32f407-rt-spark` directory, then open **env** and enter **menuconfig** in the command line to enter the configuration interface.

#### Macro Configuration

**Enable the file system (optional):**

- Hardware Drivers Config
  - Onboard Peripheral Drivers
    - Enable File System

**Enable fatfs (optional):**

- RT-Thread Components
  - DFS: device virtual file system
    - Enable elm-chan fatfs
      - elm-chan's FatFs, Generic FAT Filesystem Module
      - Change Maximum sector size to be handled to 4096

**Download the mlibc package:**

- RT-Thread online packages
  - system packages
    - Select the sixth option from the bottom: mlibc: Embedded libc, especially for RISC-V

### QEMU-Bare Metal Development

#### Development Environment

> xmake + qemu + toolchain

Currently, five types of QEMU bare metal startup codes have been supported.

| QEMU Device                       | Hardware Architecture | Toolchain               |
| --------------------------------- | --------------------- | ----------------------- |
| vexpress-a9                       | arm                   | arm-none-eabi-gcc       |
| mps3-an536 (requires latest QEMU) | arm                   | arm-none-eabi-gcc       |
| virt-aarch64                      | aarch64               | aarch64-unknown-elf-gcc |
| virt-riscv32                      | riscv32               | riscv32-unknown-elf-gcc |
| virt-riscv64                      | riscv64               | riscv64-unknown-elf-gcc |

#### Usage Steps

1. Navigate to the `mlibc/toolchains` folder, select the script corresponding to the toolchain you want to use, and configure the toolchain path accordingly.
2. Navigate to the `mlibc/helloworld/qemu/{qemu-device}` folder and open the command line.

```
# Here, we use qemu-vexpress-a9 as an example
xmake f --qemu-board=qemu-vexpress-a9
xmake build qemu-hello
```

After executing the command, an executable file named qemu-vexpress-a9.elf will be generated in the `mlibc/helloworld/qemu/qemu-vexpress-a9` folder.

3. Run the script `qemu.bat` in the corresponding folder.

```
# Enter the following command in the command line
qemu.bat
```

Information for each virtual environment is as follows:

| Filename          | Virtual Device | Switch Command                         |
| ----------------- | -------------- | -------------------------------------- |
| qemu-vexpress-a9  | vexpress-a9    | xmake f --qemu-board=qemu-vexpress-a9  |
| qemu-mps3-an536   | mps3-an536     | xmake f --qemu-board=qemu-mps3-an536   |
| qemu-virt-aarch64 | virt-aarch64   | xmake f --qemu-board=qemu-virt-aarch64 |
| qemu-virt-riscv32 | virt-riscv32   | xmake f --qemu-board=qemu-virt-riscv32 |
| qemu-virt-riscv64 | virt-riscv64   | xmake f --qemu-board=qemu-virt-riscv64 |

> **Note:** Remember to execute the `xmake build qemu-hello` command after switching devices.

## mlibc Library Compilation

#### Development Environment

For simple C library compilation, the development environment is relatively straightforward; xmake and the appropriate toolchain are sufficient.

> xmake + toolchain

#### Compilation Steps

1. Navigate to the `mlibc/toolchains` folder, select the script corresponding to the toolchain you want to use, and configure the toolchain path accordingly.

**Compile the C library**

1. Navigate to the `mlibc` folder, open the command line, and use xmake to compile the static library.

```
# Here, we use the ARM architecture static library as an example
xmake f --mlibc-arch=arm
xmake build mlibc
```

2. A static library named `libmlibc.a` will be generated in the `mlibc/build/arm` directory. To integrate it with the toolchain, you can rename the file to `libc.a` for use.

**Compile crt0**

1. Navigate to the `mlibc` folder, open the command line, and use xmake to compile crt0.

```
# Here, we use the ARM architecture crt0 as an example
xmake f --crt-arch=arm
xmake build crt0
```

2. The generated file will be located in `mlibc/build/.objs/crt0`, named `crt0.c.o`. When integrating with the toolchain, you need to rename the file to `crt0.o` for use.

# License Agreement

mlibc is fully open-source, following the MIT license. It allows for commercial use and modifications without any concerns, provided that the MIT license is declared in the software, with no potential commercial risks.

# Contribution

+ How to

  1. Fork the repository

  1. Create Feat_xxx branch
  1. Commit your code
  1. Create Pull Request

# License

mlibc follows the MIT License free software license. It's completely open-source, can be used in commercial applications for free, does not require the disclosure of code, and has no potential commercial risk.It is only necessary to declare that the MIT protocol is used in the software.



