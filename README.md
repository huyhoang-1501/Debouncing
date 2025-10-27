![C](https://img.shields.io/badge/C-00599C.svg?style=for-the-badge&logo=c&logoColor=white)
![STM32](https://img.shields.io/badge/STM32-F103-000000.svg?style=for-the-badge&logo=stmicroelectronics&logoColor=white)
![ARM](https://img.shields.io/badge/ARM-Cortex--M3-ED1C24.svg?style=for-the-badge&logo=arm&logoColor=white)

<a name="readme-top"></a>

<div align="center">
  <a href="https://github.com/yourusername/stm32f1-button-debounce-fsm">
    <img src="doc/logo.png" alt="Logo" width="300">
  </a>

  <h3 align="center">Button Debounce with FSM + SysTick</h3>

  <p align="center">
    Professional debounce for mechanical buttons on STM32F1<br>
    Using <strong>Finite State Machine (FSM)</strong> + <strong>SysTick 1ms</strong>
    <br />
    <a href="./doc/user_guide.md"><strong>Explore Docs</strong></a>
    ·
    <a href="https://github.com/yourusername/stm32f1-button-debounce-fsm/issues">Report Bug</a>
    ·
    <a href="https://github.com/yourusername/stm32f1-button-debounce-fsm/discussions">Discussions</a>
  </p>
</div>

---

## Table of Contents

<details>
  <summary>Click to expand</summary>
  <ol>
    <li><a href="#introduction">Introduction</a></li>
    <li><a href="#features">Features</a></li>
    <li><a href="#quick-start">Quick Start</a>
      <ul>
        <li><a href="#requirements">Requirements</a></li>
        <li><a href="#installation">Installation</a></li>
        <li><a href="#run-demo">Run Demo</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#fsm-structure">FSM Structure</a></li>
    <li><a href="#systick--1ms-interrupt">SysTick & 1ms Interrupt</a></li>
    <li><a href="#build">Build</a></li>
    <li><a href="#performance">Performance</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>

---

## Introduction

This project implements **professional button debouncing** for **mechanical switches** on **STM32F103 (Blue Pill)** using:

- **Finite State Machine (FSM)** – 4 states
- **SysTick timer** – precise **1ms interrupt**
- **No HAL** – pure register-level code
- **Non-blocking** – processing in interrupt

> **No polling, no delay, no external timer used!**

```mermaid
flowchart TD
    A[SysTick 1ms] --> B[btn_debounce_tick()]
    B --> C{FSM State}
    C -->|IDLE| D[Wait for press]
    C -->|BOUNCE_ON| E[Wait 20ms]
    C -->|PRESSED| F[Turn LED ON]
    C -->|BOUNCE_OFF| G[Wait 20ms]
    C -->|IDLE| H[Turn LED OFF]