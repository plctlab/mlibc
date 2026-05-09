[English](README.md) | **中文**

# Embedded libc

mlibc 是一个面向嵌入式系统、RTOS 集成和裸机程序的小型 C 库。它关注低资源占用、代码易读，以及 ARM、AArch64、RISC-V 等目标上的交叉编译。

## Mlibc特性

+ 面向小资源系统的轻量 libc
+ MIT 协议开源
+ 支持 ARM、AArch64、RISC-V 32 位和 RISC-V 64 位构建
+ 提供常用 C 库头文件和实现，包括 `stdio`、`stdlib`、`string`、`ctype`、`time` 以及少量 POSIX 风格接口
+ 支持 Make 独立构建，也支持通过 SCons 集成到 RT-Thread 软件包
+ 包含 QEMU 裸机 hello-world 示例、PSE51 测试用例和 GCC 工具链构建脚本

## 文件结构

[mlibc文件架构](ARCH_zh.md)

```
├── arch                 -- 架构相关的 SCons 入口和优化实现预留目录
├── crt                  -- ARM、AArch64、RISC-V 32/64 和通用启动代码
├── include              -- 对外头文件
│   └── sys              -- 系统和 POSIX 风格头文件，通常以 <sys/name.h> 引用
├── src                  -- 库源码
│   ├── dummy            -- errno、lock、POSIX、sbrk 等弱符号/桩实现
│   ├── internal         -- mlibc 内部头文件
│   ├── misc             -- string、assert、env、uname、setjmp、inttypes 等小模块
│   ├── stdio            -- 标准 I/O 实现
│   ├── stdlib           -- 标准库、TLSF 分配器、qsort 和进程辅助函数
│   └── time             -- 时间函数
├── helloworld           -- 裸机 hello-world 示例
│   └── qemu             -- QEMU 板级链接脚本和 UART 头文件
├── mkconfigs            -- Make 构建片段
│   └── qemu             -- QEMU 板级构建配置
├── testcases            -- 面向 PSE51/POSIX.13 的测试用例
└── toolchain            -- GCC/Binutils 构建脚本、Docker 文件和补丁
```

## 背景

### 我们的期望

● mlibc 面向需要小型、易读 libc 的嵌入式系统，而不是替代完整的主机 libc。

● 它可以用于裸机程序，也可以用于 RT-Thread 等 RTOS 环境。

● 当前构建支持 ARM、AArch64、RISC-V 32 位和 RISC-V 64 位，RISC-V 仍是重点目标。

● 仓库同时保留独立 Make 构建、RT-Thread SCons 集成、QEMU 示例、测试和工具链脚本。

## 我们的计划

● 已提供五个 QEMU 裸机 hello-world 构建目标。

● 基础 crt、string、stdio、stdlib、time 和平台桩代码已经就绪。

● 后续工作是继续补齐 libc 接口、提升 PSE51 兼容性，并让更多嵌入式测试在真实 QEMU 目标上通过。

# 快速上手

## mlibc库编译

### 开发环境

独立构建静态库需要 `make` 和对应交叉工具链。`MLIBC_TOOLCHAIN` 应设置为编译器前缀，并包含最后的短横线。

示例：

```bash
export MLIBC_TOOLCHAIN=/path/to/arm-linux-eabi_for_x86_64-pc-linux-gnu/bin/arm-linux-eabi-
```

如果没有设置 `MLIBC_TOOLCHAIN`，Makefile 默认使用 `./toolchain/arm-linux-eabi_for_x86_64-pc-linux-gnu/bin/arm-linux-eabi-`。

### 编译步骤

**编译 C 库**

```bash
# 构建默认 ARM 静态库
make mlibc ARCH=arm

# 其他可选值：
# ARCH=aarch64
# ARCH=riscv32
# ARCH=riscv64
```

生成的静态库位于 `build/<ARCH>/libmlibc.a`。集成到工具链 sysroot 时，通常会复制或重命名为 `libc.a`。

**编译 crt 对象**

```bash
make crt ARCH=arm
```

crt 对象会生成到 `build/<ARCH>/crtobj/`。ARM 和 AArch64 当前包含 `crt0.o`、`crti.o`、`crtn.o`；RISC-V 目标当前提供 `crt0.o`。

**同时构建 C 库和 crt**

```bash
make ARCH=arm
```

## mlibc开发/测试环境配置

### PSE51 测试用例

`testcases/` 目录包含面向常见 PSE51/POSIX.13 头文件的测试，包括 `assert.h`、`ctype.h`、`fcntl.h`、`locale.h`、`signal.h`、`stat.h`、`stdio.h`、`stdlib.h`、`string.h`、`time.h`、`unistd.h` 和 `utsname.h`。

```bash
cd testcases

# 构建 PC 测试
make pc-build

# 运行 PC 测试
make pc-test

# 也可以使用辅助脚本
./build pc
./test pc
```

公共 Makefile 中保留了嵌入式测试配置，但当前各测试目录只提供 PC 测试 Makefile。更多说明见 [testcases/README.md](testcases/README.md) 和 [testcases/PSE51.md](testcases/PSE51.md)。

### QEMU运行RT-Thread

#### 开发环境

请参考 RT-Thread 当前文档完成 Env 工具、软件包、BSP 和 QEMU 环境配置：

- RT-Thread 官网：https://www.rt-thread.org/
- RT-Thread 仓库：https://github.com/RT-Thread/rt-thread

mlibc 通过 `SConscript` 作为 RT-Thread 软件包集成，启用条件是 `PKG_USING_MLIBC`。

### vexpress-a9 + RT-Thread

#### 宏配置

进入 `rt-thread/bsp/qemu-vexpress-a9`，打开 Env 并运行 `menuconfig`。

推荐配置：

- RT-Thread Components
  - DFS: device virtual file system
    - 如果当前 RT-Thread 版本仍提供该选项，可选择 DFS v1.0

启用 mlibc 软件包：

- RT-Thread online packages
  - system packages
    - `mlibc: Embedded libc, especially for RISC-V`

然后下载软件包并构建：

```bash
. ~/.env/env.sh
pkgs --update
scons -j12
```

RT-Thread 的包菜单和 BSP 配置会随版本变化。如果菜单项不存在，请以当前 RT-Thread 软件包文档为准。

### 星火一号 + RT-Thread

#### 开发环境

使用 RT-Thread 的 `stm32/stm32f407-rt-spark` BSP，并在 Env 中运行 `menuconfig`。

#### 宏配置

可选文件系统配置：

- Hardware Drivers Config
  - Onboard Peripheral Drivers
    - Enable File System

可选 FatFs 配置：

- RT-Thread Components
  - DFS: device virtual file system
    - Enable elm-chan FatFs
      - 板载存储需要时，将最大扇区大小设置为 4096

启用并下载 mlibc 软件包：

```bash
. ~/.env/env.sh
pkgs --update
scons -j12
```

### QEMU-裸机开发

#### 开发环境

> make + QEMU + 交叉工具链

当前仓库为五个 QEMU 裸机 hello-world 目标提供构建支持：

| QEMU 板级目标 | 硬件架构 | 期望工具链前缀 |
| ------------- | -------- | -------------- |
| qemu-vexpress-a9 | ARM Cortex-A9 | `arm-linux-eabi-` |
| qemu-mps3-an536 | ARM Cortex-R52 | `arm-linux-eabi-` |
| qemu-virt-aarch64 | AArch64 Cortex-A53 | `aarch64-linux-gnu-` |
| qemu-virt-riscv32 | RISC-V 32 位 | `riscv32-unknown-elf-` |
| qemu-virt-riscv64 | RISC-V 64 位 | `riscv64-unknown-elf-` |

#### 使用步骤

1. 配置工具链前缀。

Linux：

```bash
export MLIBC_TOOLCHAIN=/path/to/toolchain/bin/arm-linux-eabi-
```

Windows PowerShell：

```powershell
[System.Environment]::SetEnvironmentVariable("MLIBC_TOOLCHAIN", "C:\path\to\toolchain\bin\arm-linux-eabi-", "User")
```

Windows 用户环境变量修改后，需要重启终端生效。

2. 在仓库根目录构建示例。

```bash
make qemu-hello QEMU_BOARD=qemu-vexpress-a9 ARCH=arm
```

ELF 文件会生成到 `build/<ARCH>/qemu/<QEMU_BOARD>/<QEMU_BOARD>.elf`，例如 `build/arm/qemu/qemu-vexpress-a9/qemu-vexpress-a9.elf`。

3. 使用与板级目标匹配的 `qemu-system-*` 命令运行。当前仓库提供了板级链接脚本和 UART 头文件，但没有提交 QEMU 运行脚本。

各目标构建命令如下：

| QEMU 板级目标 | 虚拟设备 | 编译命令 |
| ------------- | -------- | -------- |
| qemu-vexpress-a9 | vexpress-a9 | `make qemu-hello QEMU_BOARD=qemu-vexpress-a9 ARCH=arm` |
| qemu-mps3-an536 | mps3-an536 | `make qemu-hello QEMU_BOARD=qemu-mps3-an536 ARCH=arm` |
| qemu-virt-aarch64 | virt-aarch64 | `make qemu-hello QEMU_BOARD=qemu-virt-aarch64 ARCH=aarch64` |
| qemu-virt-riscv32 | virt-riscv32 | `make qemu-hello QEMU_BOARD=qemu-virt-riscv32 ARCH=riscv32` |
| qemu-virt-riscv64 | virt-riscv64 | `make qemu-hello QEMU_BOARD=qemu-virt-riscv64 ARCH=riscv64` |

## 预编译的 GCC 工具链

预编译 GCC 工具链由 GitHub Actions 的发布流程生成，并内置 mlibc。

### 下载预编译工具链

访问 [Releases 页面](https://github.com/plctlab/mlibc/releases) 下载目标架构对应的包：

- **ARM**: `arm-linux-eabi_for_x86_64-pc-linux-gnu-*.tar.gz`
- **RISC-V 32位**: `riscv32-unknown-elf_for_x86_64-pc-linux-gnu-*.tar.gz`
- **RISC-V 64位**: `riscv64-unknown-elf_for_x86_64-pc-linux-gnu-*.tar.gz`
- **AArch64**: `aarch64-linux-gnu_for_x86_64-pc-linux-gnu-*.tar.gz`

### 安装

```bash
tar -xzf <toolchain-tarball>.tar.gz
export PATH=/path/to/toolchain/bin:$PATH

# 示例
arm-linux-eabi-gcc --version
```

### 自行构建工具链

工具链构建使用 GCC 12.2.0 和 Binutils 2.39，补丁位于 `toolchain/patches/`。

#### 使用 Docker（推荐）

Docker 本地构建步骤见 [toolchain/README.md](toolchain/README.md)。

#### 使用 GitHub Actions

仓库包含源码发布和工具链构建工作流：

- `release.yml`：创建源码归档发布
- `build-toolchain.yml`：构建全部或指定工具链
- `build-single-toolchain.yml`：构建单个工具链用于测试

更多信息见 [.github/workflows/README.md](.github/workflows/README.md)。

# 贡献代码

+ How to

  1. Fork 本仓库
  1. 新建 `Feat_xxx` 分支
  1. 提交代码
  1. 新建 Pull Request

# 许可协议

mlibc 遵循 MIT 协议。商业或非商业项目都可以使用和修改，只需保留 MIT 协议声明。完整内容见 [LICENSE](LICENSE)。
