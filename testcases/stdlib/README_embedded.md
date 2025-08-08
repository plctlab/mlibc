# PSE51 stdlib.h 嵌入式测试套件

本目录包含适用于嵌入式系统的 stdlib.h 测试代码，特别针对 RISC-V 32 位 QEMU virt 机器进行了优化。

## 文件说明

### 嵌入式版本文件
- `test_stdlib_embedded.c` - 合并的嵌入式环境 stdlib 测试程序
- `Makefile.embedded` - 嵌入式构建配置
- `run_embedded_test.sh` - 自动构建和运行脚本
- `qemu-virt-riscv32/` - QEMU 配置目录
  - `uart.h` - UART 驱动头文件
  - `link.ld` - 链接脚本

### 原有 PC 版本文件
- `test_memory.c` - 内存管理测试
- `test_integer_math.c` - 整数数学测试
- `test_string_conversion.c` - 字符串转换测试
- `test_random.c` - 随机数测试
- `test_program_control.c` - 程序控制测试
- `test_environment_utils.c` - 环境工具测试
- `Makefile` - PC 环境构建文件
- `run_tests.sh` - PC 版本测试脚本

## 嵌入式版本特点

1. **单一可执行文件**
   - 将所有 stdlib 测试合并到一个程序中
   - 减少嵌入式环境的复杂性

2. **内存受限适配**
   - 简化大内存分配测试
   - 调整测试用例以适应有限内存

3. **无系统调用依赖**
   - 移除 fork()、system() 等系统调用
   - 适配裸机运行环境

4. **UART 控制台输出**
   - 通过串口输出测试结果
   - 实时显示测试进度

## 测试覆盖范围

### 内存管理
- `malloc()` - 基本内存分配
- `free()` - 内存释放
- `calloc()` - 清零内存分配
- `realloc()` - 内存重分配
- 边界测试和压力测试

### 整数数学
- `abs()`, `labs()` - 绝对值函数
- `div()`, `ldiv()` - 整数除法

### 字符串转换
- `atoi()`, `atol()` - 简单转换
- `strtol()`, `strtoul()` - 高级转换
- 各种进制支持（2, 8, 10, 16）
- 溢出和错误处理

### 随机数
- `rand()` - 伪随机数生成
- `srand()` - 随机数种子设置
- 确定性和分布测试

### 排序和搜索
- `qsort()` - 快速排序
- `bsearch()` - 二分查找

### 环境和程序控制
- `getenv()` - 环境变量（通常返回 NULL）
- `exit()`, `abort()` - 程序终止（仅测试链接）
- `system()` - 命令执行（通常不支持）

## 构建和运行

### 快速开始

使用自动化脚本：

```bash
./run_embedded_test.sh
```

该脚本会：
1. 检查并构建 mlibc 库
2. 构建测试程序
3. 在 QEMU 中运行测试
4. 可选择以更多内存运行（调试模式）

### 手动构建

1. 确保 mlibc 已构建：
   ```bash
   cd ../../..
   make ARCH=riscv32
   ```

2. 构建测试：
   ```bash
   make -f Makefile.embedded
   ```

3. 运行测试：
   ```bash
   make -f Makefile.embedded run
   ```

4. 调试模式（更多内存）：
   ```bash
   make -f Makefile.embedded run-debug
   ```

## 内存配置

默认配置：
- RAM: 256MB（标准运行）
- RAM: 512MB（调试模式）

链接脚本定义：
```
__flash = 0x80200000;  # 代码段
__ram = 0x82000000;    # 数据段
__stack_size = 1k;     # 栈大小
```

## 测试输出示例

```
=== PSE51 stdlib.h Test Suite (Embedded) ===

-- Memory Management Tests --
Running test_malloc_basic... [allocated 10 small blocks] PASSED
Running test_calloc... PASSED
Running test_realloc... PASSED
Running test_edge_cases... [allocated 100 small blocks] [64KB allocation succeeded] PASSED

-- Integer Math Tests --
Running test_abs_functions... PASSED
Running test_div_functions... PASSED

...

=== PSE51 stdlib.h Test Summary ===
Tests passed: 11
Tests failed: 0
Tests skipped: 1
Total tests: 12

*** ALL TESTS PASSED! ***
```

## 常见问题

### 内存分配失败
如果看到大量内存分配失败：
1. 使用调试模式运行（更多内存）
2. 检查堆大小配置
3. 减少测试中的分配大小

### QEMU 退出
- 按 `Ctrl-A` 然后 `X` 退出 QEMU

### 工具链问题
确保安装了 RISC-V 工具链：
```bash
# Ubuntu/Debian
sudo apt-get install gcc-riscv64-unknown-elf

# 或使用环境变量
export MLIBC_TOOLCHAIN=riscv32-unknown-elf-
```

## 扩展到其他平台

1. 复制 QEMU 配置目录
2. 修改 UART 基地址和寄存器
3. 调整链接脚本内存布局
4. 更新 Makefile 架构标志

示例平台：
- ARM Cortex-M (qemu-mps3-an536)
- ARM Cortex-A (qemu-vexpress-a9)
- RISC-V 64-bit (qemu-virt-riscv64)

## 与 PC 版本的区别

| 特性 | PC 版本 | 嵌入式版本 |
|------|---------|-----------|
| 测试组织 | 多个独立程序 | 单一程序 |
| 内存限制 | 无限制 | 受限于配置 |
| 系统调用 | 完整支持 | 最小化 |
| 输出方式 | 标准 I/O | UART 串口 |
| 进程支持 | fork() 可用 | 单进程 |

## 调试技巧

1. **GDB 调试**
   ```bash
   # 启动 QEMU with GDB
   qemu-system-riscv32 -machine virt -nographic -m 256M \
     -kernel build/test_stdlib.elf -s -S
   
   # 另一个终端
   riscv32-unknown-elf-gdb build/test_stdlib.elf
   (gdb) target remote :1234
   ```

2. **查看内存映射**
   ```bash
   riscv32-unknown-elf-objdump -h build/test_stdlib.elf
   ```

3. **检查符号表**
   ```bash
   riscv32-unknown-elf-nm build/test_stdlib.elf | grep malloc
   ```

## 性能考虑

嵌入式环境的限制：
- 有限的堆空间
- 较小的栈大小
- 无虚拟内存
- 简单的内存分配器

测试已针对这些限制进行了调整。