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
│   │   └───generic 
│   └───sys         
├───src             
│   ├───crt         
│   ├───internal    
│   ├───stdio       
│   └───stdlib      
├───system          
│   ├───bare        
│   │   ├───sys_fio 
│   │   └───sys_mem 
│   ├───generic     
│   │   └───tlsf    
│   │       └───inc 
│   └───rtthread    
│       ├───sys_fio 
│       └───sys_mem 
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

**开启文件系统：**

- Hardware Drivers Config
  - Onboard Peripheral Drivers
    - Enable File System

**开启fatfs：**

- RT-Thread Components
  - DFS: device virtual file system
    - Enable elm-chan fatfs
      - elm-chan's FatFs, Generic FAT Filesystem Module
      - 修改 Maximum sector size to be handled 为 4096

**下载mlibc软件包：**

- RT-Thread online packages
  - system packages
    - 选中当前页面的倒数第六个选项： mlibc: Embedded libc, especially for RISC-V

# 贡献代码

+ How to

  1. Fork 本仓库

  1. 新建 Feat_xxx 分支

  1. 提交代码

  1. 新建 Pull Request

# 许可协议

mlibc完全开源，遵循MIT协议，允许商用和随意修改，可以放心使用，仅仅需要在软件中声明使用的是MIT协议，没有潜在的商业风险。



