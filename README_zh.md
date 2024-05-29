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

目前仅支持 **RT-Thread + 星火一号** 

使用步骤：

1. 配置Env + scons环境

   - 环境配置步骤可以参考[文档中心]([Env 用户手册 (rt-thread.org)](https://www.rt-thread.org/document/site/#/development-tools/env/env))

     





# 贡献代码

+ How to

  1. Fork 本仓库

  1. 新建 Feat_xxx 分支

  1. 提交代码

  1. 新建 Pull Request

# 许可协议

mlibc完全开源，遵循MIT协议，允许商用和随意修改，可以放心使用，仅仅需要在软件中声明使用的是MIT协议，没有潜在的商业风险。



