# PSE51 assert.h 嵌入式测试套件

本目录包含适用于嵌入式系统的 assert.h 测试代码，特别针对 RISC-V 32 位 QEMU virt 机器进行了优化。

## 文件说明

### 核心文件
- `test_assert_embedded.c` - 嵌入式环境的 assert 测试主程序
- `Makefile.embedded` - 嵌入式构建配置
- `run_embedded_test.sh` - 自动构建和运行脚本

### QEMU 配置文件
- `qemu-virt-riscv32/uart.h` - UART 驱动头文件
- `qemu-virt-riscv32/link.ld` - 链接脚本

### 原有 PC 版本文件
- `test_assert.c` - PC 环境的测试程序（使用 fork）
- `Makefile` - PC 环境的构建文件

## 嵌入式版本特点

1. **无操作系统依赖**
   - 不使用 fork() 等系统调用
   - 直接运行在裸机环境

2. **UART 输出**
   - 通过 UART 串口输出测试结果
   - 实现了 write() 函数支持 printf

3. **自定义 assert 处理**
   - 重写 `__assert()` 函数
   - 测试模式下不会真正 abort
   - 设置标志位来检测 assert 触发

4. **适配嵌入式环境**
   - 使用无限循环结束程序
   - 内存和栈大小受限考虑

## 构建和运行

### 前置要求

1. RISC-V 32 位工具链
   ```bash
   # 需要 riscv32-unknown-elf-gcc
   # 或设置 MLIBC_TOOLCHAIN 环境变量
   export MLIBC_TOOLCHAIN=riscv32-unknown-elf-
   ```

2. QEMU RISC-V 模拟器
   ```bash
   # 安装 qemu-system-riscv32
   sudo apt-get install qemu-system-misc
   ```

### 快速开始

使用提供的脚本自动构建和运行：

```bash
./run_embedded_test.sh
```

### 手动构建

1. 构建 mlibc 库（如果尚未构建）
   ```bash
   cd ../../..
   make ARCH=riscv32
   ```

2. 构建测试程序
   ```bash
   make -f Makefile.embedded
   ```

3. 运行测试
   ```bash
   make -f Makefile.embedded run
   ```

4. 运行 NDEBUG 版本
   ```bash
   make -f Makefile.embedded run-ndebug
   ```

## 测试内容

测试套件包含以下测试：

1. **基本 assert 功能**
   - 真值表达式不触发 assert
   - 复杂表达式测试
   - 函数调用测试

2. **Assert 失败测试**
   - 测试 false 表达式
   - 验证 assert 消息输出

3. **NDEBUG 测试**
   - 验证 NDEBUG 定义时 assert 被禁用
   - 确保表达式不被求值

4. **副作用测试**
   - 测试表达式求值
   - NDEBUG 时的行为

5. **上下文测试**
   - 循环中的 assert
   - 条件语句中的 assert
   - switch 语句中的 assert

6. **复杂表达式**
   - 数组边界检查
   - 结构体验证
   - 字符串操作
   - 位操作

7. **函数中的 assert**
   - 前置条件检查
   - 除零保护

8. **常见模式**
   - NULL 指针检查
   - 数组索引验证
   - 状态机验证
   - 不变量检查

## QEMU 操作

- 退出 QEMU：按 `Ctrl-A` 然后按 `X`
- 串口输出直接显示在终端

## 与 PC 版本的区别

| 特性 | PC 版本 | 嵌入式版本 |
|-----|---------|-----------|
| 进程支持 | 使用 fork() | 无进程概念 |
| 信号处理 | SIGABRT 处理 | 自定义 assert 处理 |
| 程序结束 | exit() | 无限循环 |
| 输出方式 | 标准 I/O | UART 串口 |
| 内存管理 | 操作系统管理 | 固定内存布局 |

## 扩展到其他平台

要支持其他嵌入式平台：

1. 复制 `qemu-virt-riscv32` 目录
2. 修改 `uart.h` 中的 UART 基地址和寄存器定义
3. 调整 `link.ld` 中的内存布局
4. 更新 `Makefile.embedded` 中的架构标志

支持的平台示例：
- ARM Cortex-M
- ARM Cortex-A
- RISC-V 64 位
- MIPS

## 调试技巧

1. 使用 QEMU 的 `-s -S` 选项启用 GDB 调试
2. 查看生成的 `.map` 文件了解内存布局
3. 在关键位置添加 printf 调试输出
4. 使用 `objdump` 查看生成的汇编代码

## 已知限制

1. 不支持多线程测试
2. 内存限制可能影响大型测试
3. 某些 POSIX 功能在裸机环境不可用
4. 浮点支持取决于工具链配置