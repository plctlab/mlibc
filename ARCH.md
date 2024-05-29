## 模块架构图

```mermaid
flowchart LR
mlibc-->src
mlibc-->include
mlibc-->arch
mlibc-->system
src-->crt
src-->internal
src-->stdio
src-->otherImpl...
include-->sys
include-->arch_h
arch_h-->generic
system-->bare
system-->rtthread
system-->generic_
system-->otherSys..
arch-->arm
arch-->otherArch...
```

## 模块介绍

### 1. src

src目录中有三种类型的文件

- crt：存放c库的运行时初始化源文件
- internal：存放内部一些工具函数的头文件
- 存放其它标准函数的实现的源文件

### 2. include

include目录中有三种类型的文件

- sys：存放系统相关及系统调用接口相关的头文件
- arch：存放硬件接口相关的头文件（对应树状图中的arch_h）
  - 其中的generic文件夹用于存放不同架构之间的通用文件

- include（当前目录）：存放对外开放的接口，如stdio.h

> 注：generic存放各个硬件架构通用的一些文件

### 3. arch

存放不同架构对于一些硬件相关接口的实现

各个架构的硬件接口实现各占用一个文件夹

### 4. system

system目录中有三种类型的文件

- generic：存放不同的系统的通用

> 注：比较特殊的是bare文件夹，它是用于实现裸机状态下的相关调用的。

## 系统调用接口

### 内存管理接口

TODO

### 文件I/O接口

TODO

## 硬件接口

TODO
