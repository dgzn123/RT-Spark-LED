# RT-Thread Key_LED Project (English)

## Overview

This is an RT-Thread based project developed for the **STM32F407ZGTx** microcontroller. The project demonstrates basic RT-Thread OS features including thread management, custom MSH (FinSH) commands, and GPIO control.

## Hardware Platform

- **MCU:** STM32F407ZGTx
- **Board:** Custom / Spark Board (based on configuration)
- **LED Connections:**
  - **Red LED:** `PF12` (Active Low)
  - **Green LED:** `PF11` (Active Low)

## Features

The project implements several custom MSH commands to control LEDs and manage threads via the serial console.

### 1. Red LED Blinking
- **Command:** `led_red`
- **Description:** Starts a static thread named `led_red` that blinks the Red LED (PF12) every 500ms.

### 2. Green LED Breathing
- **Command:** `led_green`
- **Description:** Starts a static thread named `led_green` that simulates a breathing effect on the Green LED (PF11) using software-based PWM.

### 3. Thread Management (Killer Queen)
- **Command:** `killerqueen <thread_name>`
- **Description:** A utility command to safely terminate a running thread.
  - If the target thread is `led_red` or `led_green`, it automatically turns off the respective LED before terminating the thread.
  - Supports both static (detach) and dynamic (delete) threads.
  - **Usage Example:** `killerqueen led_red`

## Directory Structure

```
Key_LED/
├── applications/     # User application code (main.c, etc.)
├── board/           # Board Support Package (BSP) and drivers
├── libraries/       # STM32 HAL drivers
├── rt-thread/       # RT-Thread kernel source
└── ...
```

## How to Build and Run

1. **Environment:**
   - Requires RT-Thread Env tool, RT-Thread Studio, or Keil MDK/IAR.
   
2. **Compile:**
   - Using scons: `scons --target=mdk5` (to generate Keil project) or just `scons` to build.
   
3. **Flash:**
   - Download the firmware to the STM32F407 board.

4. **Usage:**
   - Open a serial terminal (baud rate usually 115200).
   - Type `help` to see available commands.
   - Run `led_red` or `led_green` to test the LEDs.
   - Use `killerqueen` to stop them.

## License

This project relies on RT-Thread, which is licensed under Apache-2.0.
