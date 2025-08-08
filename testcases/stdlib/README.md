# PSE51 stdlib.h 测试套件

本目录包含了针对 mlibc 库中 stdlib.h 函数的全面测试套件，旨在验证其对 POSIX.13 (PSE51) 标准的符合性。

## 测试覆盖范围

### 1. 内存管理测试 (test_memory.c)
- **基本分配**: malloc, free
- **初始化分配**: calloc
- **重新分配**: realloc
- **边界条件**: 零大小分配、大容量分配
- **内存对齐**: 各种大小的对齐测试
- **内存模式**: 分配/释放模式压力测试
- **线程安全**: 单线程环境下的安全性测试

### 2. 字符串转换测试 (test_string_conversion.c)
- **简单转换**: atoi, atol, atoll, atof
- **高级转换**: strtol, strtoll, strtoul, strtoull
- **浮点转换**: strtod, strtof, strtold
- **基数支持**: 二进制、八进制、十进制、十六进制
- **错误检测**: 溢出、下溢、无效输入
- **特殊值**: 科学记数法、无穷大、NaN

### 3. 随机数测试 (test_random.c)
- **基本功能**: rand, srand
- **可重入版本**: rand_r
- **分布测试**: 均匀性检验（卡方检验）
- **序列测试**: 重复性、周期性检测
- **种子测试**: 不同种子的独立性
- **常用模式**: 范围限制、浮点随机数、数组洗牌

### 4. 整数运算测试 (test_integer_math.c)
- **绝对值**: abs, labs, llabs
- **整数除法**: div, ldiv, lldiv
- **边界条件**: INT_MIN、LONG_MIN、LLONG_MIN
- **数学性质**: 商和余数的关系验证
- **性能模式**: 数字提取、进制转换

### 5. 程序控制测试 (test_program_control.c)
- **正常退出**: exit
- **立即退出**: _Exit
- **异常终止**: abort
- **退出处理**: atexit
- **退出码**: 0-255 范围测试
- **清理行为**: 缓冲区刷新、处理程序调用

### 6. 环境和工具测试 (test_environment_utils.c)
- **环境变量**: getenv, setenv, unsetenv
- **系统命令**: system
- **排序**: qsort（整数、字符串、结构体）
- **二分查找**: bsearch
- **边界情况**: 空数组、单元素、大数组

## 构建和运行

### 使用 Make

```bash
# 构建所有测试
make

# 运行所有测试
make test

# 运行特定测试
make run-test_memory
make run-test_string_conversion

# 清理构建文件
make clean

# 查看可用测试
make list

# 查看帮助
make help
```

### 使用测试脚本

```bash
# 构建并运行所有测试
./run_tests.sh

# 仅构建
./run_tests.sh build

# 仅运行测试
./run_tests.sh test

# 生成测试报告
./run_tests.sh report

# 使用 valgrind 检查内存（如果可用）
./run_tests.sh valgrind

# 清理
./run_tests.sh clean
```

## 测试输出

- 测试结果保存在 `test_results/` 目录
- 每个测试生成独立的日志文件
- 综合报告保存在 `test_results/test_report.txt`

## 测试策略

### 1. 全面性测试
- 覆盖所有 PSE51 要求的 stdlib.h 函数
- 测试正常情况和边界条件
- 验证错误处理和返回值

### 2. 健壮性测试
- 无效输入处理
- 内存耗尽情况
- 数值溢出/下溢
- 空指针和零大小

### 3. 兼容性测试
- POSIX.13 标准符合性
- C99 标准行为
- 实现定义的行为记录

### 4. 性能相关测试
- 大数据量操作
- 常见使用模式
- 内存分配模式

## 注意事项

1. **平台依赖性**: 
   - 某些测试依赖于具体实现
   - 程序控制测试使用 fork() 避免影响测试运行器
   - 环境变量测试可能受系统配置影响

2. **未实现功能**: 
   - mlibc 可能未实现某些函数
   - 测试会检查 ENOSYS 错误并相应跳过

3. **测试环境要求**:
   - 可写的文件系统
   - 基本的 POSIX 环境
   - 对于 valgrind 测试需要安装 valgrind

4. **限制条件**:
   - 浮点测试使用 epsilon 比较
   - 随机数分布测试可能偶尔失败（统计特性）
   - system() 测试依赖于 shell 可用性

## 测试标准

所有测试基于以下标准：
- IEEE Std 1003.13-2003 (POSIX.13 PSE51)
- ISO/IEC 9899:1999 (C99)
- ISO/IEC 9899:2011 (C11) 部分特性

## 扩展测试

如需添加新测试：
1. 创建新的测试文件 `test_xxx.c`
2. 在 Makefile 的 TESTS 变量中添加测试名
3. 更新 run_tests.sh 中的测试列表
4. 遵循现有测试的代码风格
5. 确保测试独立且可重复运行

## 调试失败的测试

1. 查看日志文件: `test_results/test_name.log`
2. 使用 gdb 调试: `gdb test_results/test_name`
3. 使用 valgrind 检查内存问题
4. 检查系统日志和 dmesg（对于崩溃）

## 已知问题

- 某些嵌入式系统可能不支持 fork()
- 浮点转换精度可能因硬件而异
- 大内存分配测试可能在资源受限系统失败
- 环境变量操作可能受系统限制