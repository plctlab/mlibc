# Copilot Instructions for mlibc Project

## Project Overview / 项目概述

**English:**
mlibc is a lightweight C library specifically designed for deeply embedded systems. The project focuses on implementing POSIX.1-2001 (PSE51) and related ANSI C APIs to provide essential system functionality for resource-constrained embedded environments. This library aims to offer a minimal yet complete implementation of standard C library functions while maintaining compatibility with various embedded architectures including RISC-V, ARM, and AArch64.

Key features:
- PSE51 (POSIX.1-2001) compliance
- ANSI C standard library implementation
- Multi-architecture support (RISC-V32/64, ARM, AArch64)
- Minimal memory footprint
- Embedded-first design philosophy

**中文:**
mlibc 是一个专门为深度嵌入式系统设计的轻量级 C 库。该项目专注于实现 POSIX.1-2001 (PSE51) 和相关的 ANSI C API，为资源受限的嵌入式环境提供基础系统功能。本库旨在提供标准 C 库函数的最小化但完整的实现，同时保持与多种嵌入式架构（包括 RISC-V、ARM 和 AArch64）的兼容性。

主要特性：
- 符合 PSE51 (POSIX.1-2001) 标准
- ANSI C 标准库实现
- 多架构支持 (RISC-V32/64, ARM, AArch64)
- 最小内存占用
- 嵌入式优先的设计理念

## Code Review Instructions / 代码审查指南

### Review Language / 审查语言
When performing code reviews, respond in Chinese and English.
进行代码审查时，请使用中英文双语回复。

### Review Focus Areas / 审查重点

**English:**
1. **Memory Safety**: Pay special attention to buffer overflows, memory leaks, and pointer safety
2. **Embedded Constraints**: Consider memory usage, stack usage, and performance implications
3. **Standard Compliance**: Ensure compliance with PSE51 and ANSI C standards
4. **Architecture Portability**: Verify code works across supported architectures
5. **Error Handling**: Check for proper error handling and edge cases
6. **Documentation**: Ensure code is well-documented for embedded developers

**中文:**
1. **内存安全**: 特别关注缓冲区溢出、内存泄漏和指针安全性
2. **嵌入式约束**: 考虑内存使用、栈使用和性能影响
3. **标准合规性**: 确保符合 PSE51 和 ANSI C 标准
4. **架构可移植性**: 验证代码在支持的架构间正常工作
5. **错误处理**: 检查适当的错误处理和边界情况
6. **文档说明**: 确保代码有良好的文档供嵌入式开发者使用

### Review Format / 审查格式

Please structure your reviews as follows:
请按以下格式组织您的审查：

```
## Code Review / 代码审查

### Summary / 总结
**English:** [Brief summary of changes and overall assessment]
**中文:** [变更简要总结和整体评估]

### Issues Found / 发现的问题
**English:**
- [Issue 1 description]
- [Issue 2 description]

**中文:**
- [问题1描述]
- [问题2描述]

### Recommendations / 建议
**English:**
- [Recommendation 1]
- [Recommendation 2]

**中文:**
- [建议1]
- [建议2]

### Positive Aspects / 积极方面
**English:**
- [Good practice 1]
- [Good practice 2]

**中文:**
- [良好实践1]
- [良好实践2]
```

### Specific Considerations for mlibc / mlibc 特定考虑事项

**English:**
- **PSE51 Compatibility**: Verify function signatures and behavior match PSE51 specifications
- **Minimal Dependencies**: Avoid introducing unnecessary dependencies
- **Cross-platform Build**: Consider SCons build system compatibility
- **Test Coverage**: Ensure adequate test coverage in testcases/ directory
- **Performance**: Optimize for embedded environments with limited resources

**中文:**
- **PSE51 兼容性**: 验证函数签名和行为符合 PSE51 规范
- **最小依赖**: 避免引入不必要的依赖
- **跨平台构建**: 考虑 SCons 构建系统兼容性
- **测试覆盖**: 确保 testcases/ 目录中有充分的测试覆盖
- **性能优化**: 针对资源有限的嵌入式环境进行优化

### Common Patterns to Look For / 需要关注的常见模式

**English:**
- Use of `__attribute__` for compiler-specific optimizations
- Proper errno handling according to POSIX standards
- Thread-safe implementations where required
- Minimal stack usage in recursive functions
- Proper header guard usage

**中文:**
- 使用 `__attribute__` 进行编译器特定优化
- 按照 POSIX 标准正确处理 errno
- 在需要时实现线程安全
- 递归函数中的最小栈使用
- 正确使用头文件保护

### Quality Gates / 质量门槛

**English:**
All code contributions should meet these criteria:
- Compiles without warnings on all supported architectures
- Passes existing test cases
- Includes appropriate test coverage for new functionality
- Maintains or improves performance benchmarks
- Follows project coding style

**中文:**
所有代码贡献都应满足以下标准：
- 在所有支持的架构上无警告编译
- 通过现有测试用例
- 为新功能包含适当的测试覆盖
- 维持或改善性能基准
- 遵循项目编码风格
