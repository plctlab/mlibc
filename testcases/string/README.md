# PSE51 string.h 测试套件

本目录包含了针对 mlibc 库中 string.h 函数的全面测试套件，旨在验证其对 POSIX.13 (PSE51) 标准的符合性。

## 测试覆盖范围

### 1. 内存操作测试 (test_memory_ops.c)
- **memset**: 内存填充
  - 基本填充、零填充、部分填充
  - 对齐测试、不同数据类型
  - 性能模式测试
- **memcpy**: 内存复制
  - 基本复制、部分复制、零长度复制
  - 非对齐复制、结构体复制
  - 大容量复制测试
- **memmove**: 内存移动（支持重叠）
  - 非重叠移动、前向/后向重叠
  - 完全重叠、数组元素移动
  - 循环缓冲区旋转模式
- **memcmp**: 内存比较
  - 相等/不等比较、部分比较
  - 符号性测试、自比较
  - 结构体比较注意事项
- **memchr**: 内存搜索
  - 查找存在/不存在的字节
  - 边界查找、有限搜索
  - 高位 ASCII 值搜索

### 2. 字符串复制测试 (test_string_copy.c)
- **strcpy**: 字符串复制
  - 基本复制、空字符串、单字符
  - 长字符串、特殊字符
  - UTF-8 字符串支持
- **strncpy**: 限长字符串复制
  - 精确长度、带填充复制
  - 截断处理、零长度复制
  - 安全复制模式

### 3. 字符串连接测试 (test_string_concat.c)
- **strcat**: 字符串连接
  - 基本连接、多次连接
  - 空字符串处理
  - 路径构建、UTF-8 连接
- **strncat**: 限长字符串连接
  - 限制长度连接、截断处理
  - n=0 情况、保留终止符
  - 构建限长字符串模式

### 4. 字符串搜索测试 (test_string_search.c)
- **strlen**: 字符串长度
  - 空字符串、普通字符串
  - 特殊字符、UTF-8 长度
  - 超长字符串测试
- **strchr/strrchr**: 字符搜索
  - 首次/最后出现位置
  - 查找终止符、边界查找
  - 路径解析模式
- **strstr**: 子字符串搜索
  - 基本搜索、边界情况
  - 空针模式、重叠匹配
  - 查找替换模式
- **strpbrk**: 字符集合搜索
  - 多字符查找、标点符号
  - 数字查找、分隔符查找
- **strcspn/strspn**: 字符跨度
  - 补集长度、匹配长度
  - 验证输入、空白跳过
  - 标识符验证模式

### 5. 字符串比较测试 (test_string_compare.c)
- **strcmp**: 字符串比较
  - 相等/不等比较
  - 前缀比较、大小写敏感
  - UTF-8 字节比较
- **strncmp**: 限长字符串比较
  - 部分匹配、n=0 情况
  - 超出长度比较
  - 位置差异测试
- **strcoll**: 区域相关比较
  - C 语言环境测试
  - 区域设置影响
  - 排序一致性

### 6. 其他功能测试 (test_string_misc.c)
- **strtok/strtok_r**: 字符串分词
  - 基本分词、多分隔符
  - 连续分隔符、空白处理
  - 线程安全版本、嵌套分词
  - CSV/配置文件解析
- **strxfrm**: 字符串转换
  - 基本转换、长度计算
  - 缓冲区大小处理
  - 区域相关转换
- **strerror/strerror_r**: 错误消息
  - 标准错误码
  - 线程安全版本
  - 缓冲区大小测试

## 构建和运行

### 使用 Make

```bash
# 构建所有测试
make

# 运行所有测试
make test

# 运行特定测试
make run-test_memory_ops
make run-test_string_copy

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

## 测试特性

### 1. 全面覆盖
- 覆盖所有 PSE51 要求的 string.h 函数
- 测试正常用例和边界条件
- 验证错误处理

### 2. 边界测试
- 空指针和空字符串
- 缓冲区边界
- 超大数据量
- 特殊字符和高位 ASCII

### 3. 性能模式
- 常见使用场景
- 优化模式测试
- 大数据处理

### 4. Unicode 支持
- UTF-8 字符串测试
- 多字节字符边界
- 混合字符集

### 5. 区域设置
- C 语言环境行为
- strcoll/strxfrm 区域测试

## 注意事项

1. **实现依赖**: 
   - 某些函数可能未在 mlibc 中实现
   - strerror_r 可能返回 ENOSYS
   - 区域相关函数在嵌入式系统中可能简化

2. **平台差异**:
   - 字节序可能影响某些测试
   - 错误消息文本可能不同
   - 区域设置支持可能有限

3. **内存安全**:
   - 避免测试未定义行为
   - 重叠内存操作仅用 memmove
   - 缓冲区溢出保护

4. **线程安全**:
   - strtok 不是线程安全的
   - 使用 strtok_r 进行线程安全操作
   - strerror_r 提供线程安全的错误消息

## 测试标准

所有测试基于以下标准：
- IEEE Std 1003.13-2003 (POSIX.13 PSE51)
- ISO/IEC 9899:1999 (C99)
- ISO/IEC 9899:2011 (C11) 部分特性

## 常见使用模式

测试套件包含了许多实际编程中的常见模式：

1. **安全字符串操作**
   - 使用 strncpy + 手动终止符
   - 长度检查后使用 strcpy
   - strncat 防止溢出

2. **解析模式**
   - CSV 文件解析（strtok_r）
   - 路径分解（strchr/strrchr）
   - 配置文件解析（strstr/strtok）

3. **搜索和验证**
   - 文件扩展名检查
   - 输入验证（strspn/strcspn）
   - 查找替换（strstr）

4. **国际化**
   - UTF-8 字符串处理
   - 区域相关排序（strcoll/strxfrm）

## 扩展测试

如需添加新测试：
1. 创建新的测试文件 `test_xxx.c`
2. 在 Makefile 的 TESTS 变量中添加测试名
3. 更新 run_tests.sh 中的测试列表
4. 遵循现有测试的代码风格
5. 确保测试独立且可重复运行

## 性能考虑

- memcpy vs memmove：无重叠时 memcpy 更快
- strcmp vs strncmp：已知长度时 strncmp 可能更优
- 大数据量操作应考虑缓存友好性
- 某些实现可能有 SIMD 优化

## 调试建议

1. 查看测试日志：`test_results/test_name.log`
2. 使用 gdb 调试失败的测试
3. valgrind 检查内存问题
4. 添加断言帮助定位问题
5. 使用十六进制查看器检查二进制数据