[中文](README_zh.md) | **English** 

# Embedded libc 

Embedded libc，a library which design for RTOS and Bare machines.

## Mlibc Architecture

+ Designed for resource-constrained devices
+ OpenSource
+ Especially for RISC-V..

## Code Catalogue



## Background



# Why we do

## Future

 ● mlibc can support multiple embedded tool-chains,it can be compiled by gcc(arm/risc-v)、even though LLVM compiler.

● Designed for resource-constrained devices，it can support some RTOS (RT-Thread) and Bare machines.

● It optimized for risc-v 32/64， have been specially adapted for RISC-V MCU

● It is is built by xmake or scons

● TODO



## Our plan



● QEMU/RISC-V 32GC can be used as a computer to putout "Hello word!" at first

● For Bare machines ,we push out especially mlibc sothat wecan accomplish crt.s，string and printf

● Basing on this environment,we will make mlibc perfectly.



# Getting Started

TODO





# Contribution

+ How to

  1. Fork the repository

  1. Create Feat_xxx branch
  1. Commit your code
  1. Create Pull Request







# License

RT-Thread follows the MIT License free software license. It's completely open-source, can be used in commercial applications for free, does not require the disclosure of code, and has no potential commercial risk.It is only necessary to declare that the MIT protocol is used in the software.



