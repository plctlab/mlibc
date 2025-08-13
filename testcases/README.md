# mlibc 测试用例

本目录包含 mlibc 的测试用例，用于验证 POSIX.13 (PSE51) 标准兼容性。

## 目录结构

```
testcases/
├── common/              # 公共文件
│   ├── Makefile.common  # 通用 Makefile 定义
│   ├── Makefile.template # Makefile 模板文件
│   ├── embedded_startup.c # 嵌入式环境启动代码
│   └── main.c          # 通用主入口文件
├── assert/             # assert.h 测试
├── ctype/              # ctype.h 测试
├── fcntl/              # fcntl.h 测试
├── locale/             # locale.h 测试
├── signal/             # signal.h 测试
├── stat/               # stat.h 测试
├── stdio/              # stdio.h 测试（多文件）
├── stdlib/             # stdlib.h 测试（多文件）
├── string/             # string.h 测试（多文件）
├── time/               # time.h 测试
├── unistd/             # unistd.h 测试
├── utsname/            # utsname.h 测试
├── build               # 构建脚本
├── test                # 测试运行脚本
├── test_colors.h       # 测试输出颜色定义
└── README.md           # 本文件
```

## 文件组织

每个测试目录包含：
- `test_*.c` - 测试实现文件，包含具体的测试函数和 `run_tests()` 函数
- `Makefile` - 构建配置（引用 common/main.c 作为主入口）

## 构建系统

### 构建命令

```bash
# 构建所有测试
./build

# 只构建 PC 测试
./build pc

# 只构建嵌入式测试
./build embedded

# 清理构建产物
./build clean

# 详细输出模式
./build -v pc
```

### Makefile 结构

- `common/Makefile.common` - 定义通用变量和规则，支持多架构配置
- `common/Makefile.template` - 单文件测试的 Makefile 模板
- 各测试目录的 `Makefile` - 包含特定测试的配置

特殊情况：
- `time/` - 需要链接 `-lrt -lpthread`
- `stdio/`, `stdlib/`, `string/` - 包含多个测试文件

### 嵌入式构建配置

通过环境变量配置嵌入式构建：
```bash
# 为不同架构构建
EMBEDDED_ARCH=riscv32 make embedded  # 默认
EMBEDDED_ARCH=riscv64 make embedded
EMBEDDED_ARCH=arm make embedded
EMBEDDED_ARCH=aarch64 make embedded

# 运行嵌入式测试
EMBEDDED_ARCH=arm make test
```

### 文件组织特点

1. **统一的主入口**：所有测试共享 `common/main.c`，不需要在每个测试目录中复制
2. **相对路径引用**：Makefile 使用相对路径引用 QEMU 配置文件（`../../helloworld/qemu/`）
3. **模块化构建**：`Makefile.common` 处理不同架构的特定配置

## 测试执行

```bash
# 运行所有测试
./test

# 测试输出说明：
# ✓ - 测试通过
# ✗ - 测试失败
# ⚠ - 测试未找到或跳过
```

## 测试框架

### main.c 结构

所有测试使用统一的 `main.c`：

```c
int main(void) {
#if EMBEDDED_TEST
    initialise_monitor_handles();
    printf("\n=== Running Embedded Tests ===\n\n");
#endif

    run_tests();
    
    printf("\n=== Tests Completed ===\n");
    
    return 0;
}
```

### 测试文件结构

每个 `test_*.c` 文件包含：
1. 具体的测试函数（如 `test_xxx()`）
2. `run_tests()` 函数，调用所有测试函数

示例：
```c
void test_feature1(void) { /* ... */ }
void test_feature2(void) { /* ... */ }

void run_tests(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 xxx.h Test Suite ===" COLOR_RESET "\n\n");
    
    test_feature1();
    test_feature2();
    
    TEST_SUITE_PASSED("xxx.h");
}
```

## 平台支持

测试支持两种运行环境：
- **PC 环境** - 在主机上直接运行
- **嵌入式环境** - 通过 QEMU 模拟器运行，支持多种架构：
  - RISC-V 32位 (riscv32) - 默认
  - RISC-V 64位 (riscv64)
  - ARM (arm)
  - AArch64 (aarch64)

平台检测通过预处理器宏实现：
```c
#if defined(__riscv) || defined(__arm__) || defined(__aarch64__)
    #define EMBEDDED_TEST 1
#else
    #define EMBEDDED_TEST 0
#endif
```

## 测试覆盖

测试用例覆盖 POSIX.13 (PSE51) 标准要求的以下头文件：
- assert.h - 断言宏
- ctype.h - 字符分类和转换
- fcntl.h - 文件控制操作
- locale.h - 本地化支持
- signal.h - 信号处理
- stat.h - 文件状态
- stdio.h - 标准输入输出
- stdlib.h - 标准库函数
- string.h - 字符串操作
- time.h - 时间处理
- unistd.h - POSIX 操作系统接口
- utsname.h - 系统名称

## 添加新测试

1. 创建新目录（如果需要）
2. 创建 `test_xxx.c` 文件，实现测试函数和 `run_tests()`
3. 复制 `common/Makefile.template` 到测试目录并重命名为 `Makefile`
4. 更新 `testcases/Makefile` 添加新测试目录

## 注意事项

- 所有测试应该是自包含的，不依赖外部状态
- 测试失败时使用 `assert()` 宏
- 使用 `TEST_START()` 和 `TEST_PASSED()` 宏标记测试进度
- 多文件测试目录（stdio、stdlib、string）中每个文件都有自己的 `run_tests()` 函数

## 当前测试状态

### PC 环境测试状态
所有 12 个测试用例在 PC 环境下全部通过。

### 嵌入式环境测试状态
嵌入式测试需要相应的交叉编译工具链和 QEMU 环境。详细的各架构测试状态请参见 `PSE51.md`。