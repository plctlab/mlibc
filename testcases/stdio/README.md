# PSE51 stdio.h 测试套件

本目录包含了针对 mlibc 库中 stdio.h 函数的全面测试套件，旨在验证其对 POSIX.13 (PSE51) 标准的符合性。

## 测试覆盖范围

### 1. 文件操作测试 (test_file_operations.c)
- **文件打开/关闭**: fopen, fclose
- **文件读写**: fread, fwrite
- **文件定位**: fseek, ftell, rewind
- **文件状态**: feof, ferror, clearerr
- **二进制操作**: 二进制文件读写
- **错误处理**: 各种错误条件测试
- **大文件操作**: 大数据量读写测试

### 2. 格式化 I/O 测试 (test_formatted_io.c)
- **printf 系列**: printf, fprintf, sprintf, snprintf
- **vprintf 系列**: vprintf, vfprintf, vsprintf, vsnprintf
- **scanf 系列**: scanf, fscanf, sscanf
- **格式说明符**: %d, %u, %x, %o, %s, %c, %p 等
- **宽度和精度**: 字段宽度、精度控制
- **缓冲区安全**: snprintf 边界测试

### 3. 字符 I/O 测试 (test_char_io.c)
- **基本字符 I/O**: fgetc, fputc, getc, putc
- **标准 I/O**: getchar, putchar
- **字符回退**: ungetc
- **无锁版本**: getc_unlocked, putc_unlocked 等
- **不同文件模式**: 读、写、追加模式测试
- **错误条件**: 错误处理测试

### 4. 字符串 I/O 测试 (test_string_io.c)
- **字符串输入**: fgets
- **字符串输出**: fputs, puts
- **弃用函数**: gets（仅说明，不测试）
- **行结束符处理**: 不同换行符测试
- **多字节字符**: UTF-8 支持测试
- **缓冲区边界**: 边界条件测试

### 5. 其他功能测试 (test_misc_stdio.c)
- **缓冲控制**: fflush, setvbuf
- **错误报告**: perror
- **文件操作**: rename, remove
- **临时文件**: tmpfile, tmpnam
- **文件描述符**: fileno, fdopen, freopen
- **文件锁定**: flockfile, ftrylockfile, funlockfile

## 构建和运行

### 使用 Make

```bash
# 构建所有测试
make

# 运行所有测试
make test

# 运行特定测试
make run-test_file_operations

# 清理构建文件
make clean

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

# 使用 valgrind 检查内存泄漏（如果可用）
./run_tests.sh valgrind

# 清理
./run_tests.sh clean
```

## 测试输出

- 测试结果保存在 `test_results/` 目录
- 每个测试生成独立的日志文件
- 测试报告汇总在 `test_results/test_report.txt`

## 注意事项

1. **平台依赖性**: 某些测试可能依赖于底层系统实现
2. **未实现功能**: mlibc 可能未实现某些 PSE51 函数，测试会相应跳过
3. **文件系统**: 测试需要可写的文件系统
4. **权限**: 某些测试可能需要特定权限

## 测试标准

所有测试基于以下标准：
- IEEE Std 1003.13-2003 (POSIX.13 PSE51)
- ISO/IEC 9899:1999 (C99)

## 扩展测试

如需添加新测试：
1. 创建新的测试文件 `test_xxx.c`
2. 在 Makefile 的 TESTS 变量中添加测试名
3. 遵循现有测试的代码风格和结构
4. 确保测试独立且可重复运行

## 已知限制

- scanf 系列函数可能未完全实现
- 某些高级格式说明符可能不支持
- 浮点数格式化可能受限
- 多线程相关功能在单线程环境下可能简化