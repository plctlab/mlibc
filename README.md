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
│   └───qemu            -- qemu bare-metal config
│     └───qemu-device	-- Specific to the QEMU virtual machine device, related scripts and header files
├───mkconfigs           -- Scripts related to make
    └───qemu            -- qemu configuration
```

## Background

### Our Expectations

● mlibc can support multiple embedded toolchains, including gcc (arm/risc-v) and even LLVM compilers.

● Designed for low-resource systems, providing excellent support for embedded real-time operating systems (such as RT-Thread) and bare metal.

● Optimized for RISC-V 32/64, compatible with mainstream RISC-V MCUs.

● Uses make and scons for building.

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

> make + qemu + toolchain

Currently, five types of QEMU bare metal startup codes have been supported.

| QEMU Device                       | Hardware Architecture | Toolchain               |
| --------------------------------- | --------------------- | ----------------------- |
| vexpress-a9                       | arm                   | arm-none-eabi-gcc       |
| mps3-an536 (requires latest QEMU) | arm                   | arm-none-eabi-gcc       |
| virt-aarch64                      | aarch64               | aarch64-unknown-elf-gcc |
| virt-riscv32                      | riscv32               | riscv32-unknown-elf-gcc |
| virt-riscv64                      | riscv64               | riscv64-unknown-elf-gcc |

#### Usage Steps

1. Configure the environment variables corresponding to the toolchain

**Linux：** Add the following environment variables to the `~/.bashrc` file through commands. Replace `YOUR_PATH_TO_TOOLCHAIN` with the corresponding path of your toolchain.
```
echo "" >> ~/.bashrc    #Append a a line break to prevent content merging

echo "export MLIBC_TOOLCHAIN_CC='YOUR_PATH_TO_TOOLCHAIN'" >> ~/.bashrc
echo "export MLIBC_TOOLCHAIN_AR='YOUR_PATH_TO_TOOLCHAIN'" >> ~/.bashrc

source ~/.bashrc
```

**Windows：** Open PowerShell and replace `YOUR_PATH_TO_TOOLCHAIN` with the corresponding path of your toolchain.
```
[System.Environment]::SetEnvironmentVariable("MLIBC_TOOLCHAIN_CC", "YOUR_PATH_TO_TOOLCHAIN", "User")
[System.Environment]::SetEnvironmentVariable("MLIBC_TOOLCHAIN_AR", "YOUR_PATH_TO_TOOLCHAIN", "User")
```
**\#**:After the configuration is completed, you need to restart the terminal for the changes to take effect.

2. Navigate to the `mlibc/helloworld/qemu/{qemu-device}` folder and open the command line.

```
# Here, we use qemu-vexpress-a9 as an example
make QEMU_BOARD=qemu-vexpress-a9 ARCH=arm
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
| qemu-vexpress-a9  | vexpress-a9    | make QEMU_BOARD=qemu-vexpress-a9 ARCH=arm  |
| qemu-mps3-an536   | mps3-an536     | make QEMU_BOARD=qemu-mps3-an536 ARCH=arm   |
| qemu-virt-aarch64 | virt-aarch64   | make QEMU_BOARD=qemu-virt-aarch64 ARCH=aarch64 |
| qemu-virt-riscv32 | virt-riscv32   | make QEMU_BOARD=qemu-virt-riscv32 ARCH=riscv32 |
| qemu-virt-riscv64 | virt-riscv64   | make QEMU_BOARD=qemu-virt-riscv64 ARCH=riscv64 |


## mlibc Library Compilation

#### Development Environment

For simple C library compilation, the development environment is relatively straightforward; make and the appropriate toolchain are sufficient.

> make + toolchain

#### Compilation Steps

1. Configure the environment variables corresponding to the toolchain
:Please refer to the detailed steps in the previous section)

**Compile the C library**

1. Navigate to the `mlibc` folder, open the command line, and use make to compile the static library.

```
# Here, we use the ARM architecture static library as an example
make mlibc ARCH=arm
```

2. A static library named `libmlibc.a` will be generated in the `mlibc/build/arm` directory. To integrate it with the toolchain, you can rename the file to `libc.a` for use.

**Compile crt0**

1. Navigate to the `mlibc` folder, open the command line, and use make to compile crt0.

```
# Here, we use the ARM architecture crt0 as an example
make crt0 ARCH=arm
```

2. The generated file will be located in `mlibc/build/$(ARCH)/crtobj`, named `crt0.o`.

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