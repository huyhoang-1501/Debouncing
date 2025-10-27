![C](https://img.shields.io/badge/C-00599C.svg?style=for-the-badge&logo=c&logoColor=white)
![STM32](https://img.shields.io/badge/STM32-F103-000000.svg?style=for-the-badge&logo=stmicroelectronics&logoColor=white)

# STM32F1 Button Debounce (FSM + SysTick)

**Professional button debouncing – No HAL – Non-blocking CPU**

---

## Introduction

This project implements **professional button debouncing** for **mechanical switches** on **STM32F103** using:

- **4-state FSM** + **SysTick 1 ms interrupt**
- No polling
- No `delay()`
- No peripheral timers
- Callbacks fire **once per stable edge**

---

## Features

| Feature | Description |
|--------|-------------|
| Dual-edge debounce | Filters noise on **press** and **release** |
| Single callback | `on_press()`, `on_release()` called **once** |
| Non-blocking CPU | `while(1)` empty – safe for `__WFI()` |
| Easy to extend | Long press, double-click, multi-button |

---

## Quick Start

### Requirements
- STM32F103 (Blue Pill)
- Button (PA0) + LED (PA1)
- ST-Link

### Clone
```bash
git clone https://github.com/huyhoang-1501/Debouncing.git
cd Debouncing