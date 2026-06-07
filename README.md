# moneOS

[![Project Status](https://img.shields.io/badge/status-active-brightgreen.svg)]()

## Description

moneOS is a minimal, hobbyist operating system developed from scratch. It aims to provide a basic shell environment with core drivers for hardware interaction.

---

## 🇷🇺 Русский (Russian)

### О проекте
moneOS — это учебный проект по созданию собственной операционной системы с нуля (bare metal). Проект включает в себя базовые драйверы для работы с оборудованием (экран, клавиатура) и простую командную оболочку (shell).

### Основные компоненты
*   **Bootloader:** Загрузка системы (Multiboot).
*   **Kernel:** Базовое ядро ОС.
*   **Drivers:** Драйверы экрана и клавиатуры.
*   **Shell:** Простая интерпретация команд пользователя.

---

## 🇬🇧 English

### About the project
moneOS is a hobbyist operating system project developed from scratch. It provides a foundational kernel with essential drivers for hardware interaction (display, keyboard) and a simple shell for user interaction.

### Key Components
*   **Bootloader:** System startup (Multiboot).
*   **Kernel:** Basic OS kernel.
*   **Drivers:** Display and keyboard drivers.
*   **Shell:** Simple command-line interface.

---

## Getting Started

### Prerequisites
*   GCC (cross-compiler recommended)
*   NASM
*   QEMU (for running)
*   GRUB utilities (for ISO creation)

### Build
```bash
make
```

### Run
```bash
make run
```
