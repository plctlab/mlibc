# Embedded libc



## 介绍

---

Embedded libc，一个为嵌入式系统乃至裸机适配的libc库



## 目标

---

- mlibc需要摆脱对工具链的依赖，能够使用gcc(arm/risc-v)、甚至LLVM等编译器
- 为小资源系统设计，完美地支持一些小型操作系统（RT-thread等）和裸机（另开一个版本）
- 针对risc-v 32/64进行优化，能够适配国内大多数厂家的产品
- 采用xmake构建
- reserve



## 计划

1. 使用模拟主机，输出第一个hello word
2. 针对裸机版本的mlibc，加入最基础的crt.s，string，printf函数
3. 基于此环境，完善mlibc



## 使用说明

reserve



## 作者

reserve、



## 赞助

+ reserve



## 感谢

reserve



## 许可

**reserve**



## 参与贡献

1. Fork 本仓库
2. 新建 Feat_xxx 分支
3. 提交代码
4. 新建 Pull Request

