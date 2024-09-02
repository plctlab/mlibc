[English](README.md) | **中文** 

# Embedded libc 

Embedded libc，一个为嵌入式系统和裸机适配的libc库

## Mlibc特性

+ 低资源占用
+ OpenSource
+ Especially for RISC-V..
+ 代码结构清晰易懂
+ 可扩展
+ 移植性强

## 文件结构

[mlibc文件架构](ARCH.md)

```
├───arch				-- 硬件相关的优化实现	
├───crt					-- 硬件相关的启动代码
├───include         	-- 头文件
│   └───sys        		-- 系统相关的头文件，引用方式一般为 <sys/head.h>
├───src             	-- 源文件
│   ├───internal    	-- 一些内部使用的头文件
│   ├───misc       		-- 杂项，存放单文件就能实现的模块
|	├───stdio       	-- 标准IO模块
│   └───stdlib    		-- 标准工具库模块
├───xscript				-- xmake相关的脚本
└───toolchains		    -- 工具链相关的xmake脚本
```

## 背景

### 我们的期望

 ● mlibc可以支持到多种嵌入式工具链，能够使用gcc(arm/risc-v)、甚至LLVM等编译器

● 为小资源系统设计，完美地支持到一些嵌入式实时操作系统（RT-Thread等）和裸机

● 针对risc-v 32/64进行优化，能够适配主流的一些RISC-V MCU

● 采用xmake和scons构建

● reserve

## 我们的计划

● 使用QEMU/RISC-V 32GC模拟主机方式，输出第一个hello word（已完成）

● 针对裸机版本的mlibc，加入最基础的crt.s，string，printf函数（已完成）

● 基于此环境，完善mlibc



# 快速上手

## mlbc开发/测试环境配置

### QEMU运行RT-Thread

#### 开发环境

windows下的环境配置教程：

https://github.com/RT-Thread/rt-thread/blob/master/documentation/quick-start/quick_start_qemu/quick_start_qemu_windows.md

通过这个教程我们就可以在windows环境下运行RT-Thread了。

### vexpress-a9 + RT-Thread

#### 宏配置

我们进入`rt-thread\bsp\qemu-vexpress-a9`文件夹，打开**env**，在命令行中输入**menuconfig**，进入配置界面

**将DFS v2.0切换为DFS v1.0：**

- RT-Thread Components
  - DFS: device virtual file system
    - The version of DFS (DFS v1.0)

**下载mlibc软件包：**

- RT-Thread online packages
  - system packages
    - 选中当前页面的倒数第六个选项： `mlibc: Embedded libc, especially for RISC-V`

完成之后就可以退出配置页面然后在命令行输入**scons -j12**进行编译了。

### 星火一号 + RT-Thread

#### 开发环境

不知道哪里下载源码的同学可以参考上面windows环境配置教程

进入`rt-thread\bsp\stm32\stm32f407-rt-spark`目录，然后打开**env**，在命令行输入**menuconfig**，进入配置界面

#### 宏配置

**开启文件系统（可选）：**

- Hardware Drivers Config
  - Onboard Peripheral Drivers
    - Enable File System

**开启fatfs（可选）：**

- RT-Thread Components
  - DFS: device virtual file system
    - Enable elm-chan fatfs
      - elm-chan's FatFs, Generic FAT Filesystem Module
      - 修改 Maximum sector size to be handled 为 4096

**下载mlibc软件包：**

- RT-Thread online packages
  - system packages
    - 选中当前页面的倒数第六个选项： mlibc: Embedded libc, especially for RISC-V

### QEMU-裸机开发

#### 开发环境

> xmake + qemu + 工具链

目前已经实现了五种qemu的裸机启动代码

| qemu设备                         | 硬件架构 | 工具链                  |
| -------------------------------- | -------- | ----------------------- |
| vexpress-a9                      | arm      | arm-none-eabi-gcc       |
| mps3-an536（要求最新的qemu版本） | arm      | arm-none-eabi-gcc       |
| virt-aarch64                     | aarch64  | aarch64-unknown-elf-gcc |
| virt-riscv32                     | riscv32  | riscv32-unknown-elf-gcc |
| virt-riscv64                     | riscv64  | riscv64-unknown-elf-gcc |

#### 使用步骤

1. 进入`mlibc/toolchains`文件夹，选择你想要使用的工具链对应的脚本，然后配置对应的工具链路径即可
2. 进入`mlibc/helloworld/qemu/{qemu-device}`文件夹打开命令行

```
# 这里使用qemu-vexpress-a9举例
xmake f --qemu-board=qemu-vexpress-a9
xmake build qemu-hello
```

 执行完命令之后，mlibc/helloworld/qemu/qemu-vexpress-a9文件夹下会生成一份可执行文件为qemu-vexpress-a9.elf

3. 运行对应文件夹下的脚本`qemu.bat`

```
# 在命令行敲入下列命令
qemu.bat
```

各个虚拟环境对应的信息如下：

| 文件名            | 虚拟设备     | 切换命令                               |
| ----------------- | ------------ | -------------------------------------- |
| qemu-vexpress-a9  | vexpress-a9  | xmake f --qemu-board=qemu-vexpress-a9  |
| qemu-mps3-an536   | mps3-an536   | xmake f --qemu-board=qemu-mps3-an536   |
| qmeu-virt-aarch64 | virt-aarch64 | xmake f --qemu-board=qemu-virt-aarch64 |
| qemu-virt-riscv32 | virt-riscv32 | xmake f --qemu-board=qemu-virt-riscv32 |
| qemu-virt-riscv64 | virt-riscv64 | xmake f --qemu-board=qemu-virt-riscv64 |

> **注意：**切换完设备之后记得要执行`xmake build qemu-hello`命令

## mlibc库编译

#### 开发环境

对于单纯的c库编译来说，开发环境就简单一些了，具备xmake和对应工具链就可以

> xmake + 工具链

#### 编译步骤

1. 进入`mlibc/toolchains`文件夹，选择你想要使用的工具链对应的脚本，配置对应工具链的路径

**编译c库**

1. 进入mlibc文件夹，打开命令行使用xmake编译静态库

```
# 这里使用arm架构的静态库举例
xmake f --mlibc-arch=arm
xmake build mlibc
```

2. 然后会在mlibc/build/arm目录下生成一份静态库，文件名为`libmlibc.a`

**编译crt0**

1. 进入mlibc文件夹，打开命令行使用xmake编译crt0

```
# 这里使用arm架构的crt0举例
xmake f --crt-arch=arm
xmake build crt0
```

2. 生成的文件在

# 贡献代码

+ How to

  1. Fork 本仓库

  1. 新建 Feat_xxx 分支

  1. 提交代码

  1. 新建 Pull Request

# 许可协议

mlibc完全开源，遵循MIT协议，允许商用和随意修改，可以放心使用，仅仅需要在软件中声明使用的是MIT协议，没有潜在的商业风险。



