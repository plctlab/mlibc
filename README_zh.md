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

## 目录

[mlibc文件架构](ARCH.md)

```
├───arch            
├───include         
│   ├───arch        
├───src             
│   ├───crt         
│   ├───internal    
│   ├───stdio       
│   └───stdlib
└───toolchains      
```

## 背景



# Why we do

## 我们的期望

 ● mlibc可以支持到多种嵌入式工具链，能够使用gcc(arm/risc-v)、甚至LLVM等编译器

● 为小资源系统设计，完美地支持到一些嵌入式实时操作系统（RT-Thread等）和裸机

● 针对risc-v 32/64进行优化，能够适配主流的一些RISC-V MCU

● 采用xmake和scons构建

● reserve



## 我们的计划



● 使用QEMU/RISC-V 32GC模拟主机方式，输出第一个hello word

● 针对裸机版本的mlibc，加入最基础的crt.s，string，printf函数

● 基于此环境，完善mlibc



# 快速上手

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
    - 选中当前页面的倒数第六个选项： mlibc: Embedded libc, especially for RISC-V

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

### cortex-a9裸机

#### 开发环境

> xmake + env

#### 使用步骤

1. 进入mlibc文件夹，打开env，然后敲入以下命令

```
xmake f --board=qemu-vexpress-a9
xmake
```

### cortex-r52裸机

#### 开发环境

> xmake + env + qemu-9.0.0

替换env中原本的qemu文件夹：

替换指定路径下的文件夹：`env-windows-1.5.0\tools\qemu\qemu`

> 注意：文件夹的名字也需要改为对应的名字（qemu/qemu64），具体取决于env的版本

#### 使用步骤

1. 进入mlibc文件夹，打开env，然后敲入以下命令

```
xmake f --board=qemu-vexpress-r52
xmake
```

### qemu-virt-aarch64裸机

#### 开发环境

> xmake + env

#### 使用步骤

1. 进入mlibc文件夹，打开env，然后敲入以下命令

```
xmake f --board=qemu-virt-aarch64
xmake 
```

#### 问题

1. 测试时会出现地址对齐异常
2. 目前推测可以通过开启芯片的mmu来解决（不属于mlibc的范畴）

### qemu-virt-riscv64裸机

#### 开发环境

> xmake + env

#### 使用步骤

1. 进入mlibc文件夹，打开env，然后敲入以下命令

```
xmake f --board=qemu-virt-riscv64
xmake
```

#### 问题

1. 使用内存管理的相关函数时无法通过编译

### TODO

1. 提供可重入函数，为RT-Thread的多线程环境提供支持
2. 提供对riscv32的支持

# 贡献代码

+ How to

  1. Fork 本仓库

  1. 新建 Feat_xxx 分支

  1. 提交代码

  1. 新建 Pull Request

# 许可协议

mlibc完全开源，遵循MIT协议，允许商用和随意修改，可以放心使用，仅仅需要在软件中声明使用的是MIT协议，没有潜在的商业风险。



