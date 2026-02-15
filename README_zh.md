# RT-Thread Key_LED 项目 (中文)

[English Version](README.md)

## 简介

这是一个基于 RT-Thread 操作系统的 **STM32F407ZGTx** 微控制器项目。该项目演示了 RT-Thread 的基础功能，包括线程管理、自定义 MSH (FinSH) 命令以及 GPIO 控制。

## 硬件平台

- **MCU:** STM32F407ZGTx
- **开发板:** 自定义 / Spark 开发板 (基于配置宏)
- **LED 连接引脚:**
  - **红灯 (Red LED):** `PF12` (低电平点亮)
  - **绿灯 (Green LED):** `PF11` (低电平点亮)

## 功能特性

本项目实现了几个自定义的 MSH 命令，允许用户通过串口控制台来控制 LED 和管理线程。

### 1. 红灯闪烁
- **命令:** `led_red`
- **描述:** 启动一个名为 `led_red` 的静态线程。该线程会使红色 LED (PF12) 以 500ms 的间隔闪烁。

### 2. 绿灯呼吸效果
- **命令:** `led_green`
- **描述:** 启动一个名为 `led_green` 的静态线程。该线程利用软件延时模拟 PWM 效果，使绿色 LED (PF11) 实现呼吸灯效果。

### 3. 线程管理 (Killer Queen)
- **命令:** `killerqueen <thread_name>`
- **描述:** 用于安全终止运行中线程的实用命令。
  - 如果目标线程是 `led_red` 或 `led_green`，它会在终止线程前自动关闭对应的 LED，防止 LED 保持常亮状态。
  - 支持静态线程（脱离/Detach）和动态线程（删除/Delete）的处理。
  - **使用示例:** `killerqueen led_red`

## 目录结构

```
Key_LED/
├── applications/     # 用户应用代码 (main.c 等)
├── board/           # 板级支持包 (BSP) 和驱动配置
├── libraries/       # STM32 HAL 库文件
├── rt-thread/       # RT-Thread 内核源码
└── ...
```

## 编译与运行

1. **开发环境:**
   - 需要 RT-Thread Env 工具、RT-Thread Studio 或 Keil MDK/IAR。
   
2. **编译:**
   - 使用 scons: `scons --target=mdk5` (生成 Keil 工程) 或直接使用 `scons` 编译。
   
3. **烧录:**
   - 将编译好的固件下载到 STM32F407 开发板。

4. **使用方法:**
   - 打开串口终端 (波特率通常为 115200)。
   - 输入 `help` 查看可用命令。
   - 运行 `led_red` 或 `led_green` 测试 LED 功能。
   - 使用 `killerqueen` 命令停止相应的任务。

## 许可证

本项目基于 RT-Thread 开发，遵循 Apache-2.0 许可证。
