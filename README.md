# Embedded Firmware Framework

[![C++](https://img.shields.io/badge/C++-11/14/17-blue.svg)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-ARM%20Cortex--M-green.svg)](https://developer.arm.com/ip-products/processors/cortex-m)

A lightweight, portable embedded firmware framework written in modern C++ for ARM Cortex-M microcontrollers.

## Overview

This open-source project provides a hardware abstraction layer (HAL) and driver framework for embedded systems development. It is designed to be portable across different microcontroller families while maintaining efficiency and a small memory footprint.

## Features

- **Hardware Abstraction Layer (HAL)** - Portable interface for GPIO, UART, SPI, I2C, and Timers
- **Driver Framework** - Reusable peripheral drivers with consistent API
- **RTOS Support** - Compatible with FreeRTOS and bare-metal configurations
- **Low Power Modes** - Built-in power management utilities
- **Modern C++** - Utilizes C++11/14 features for type safety and efficiency
- **Unit Testing** - Comprehensive test suite for validation

## Supported Platforms

| Platform | Status |
|----------|--------|
| STM32F4xx | ✅ Supported |
| STM32F1xx | ✅ Supported |
| NXP LPC1768 | ✅ Supported |
| Nordic nRF52 | 🔄 In Progress |

## Project Structure

```
embedded-firmware-project/
├── src/                    # Source files
│   ├── main.cpp
│   ├── system.cpp
│   └── startup.cpp
├── include/                # Header files
│   ├── system.hpp
│   ├── types.hpp
│   └── config.hpp
├── hal/                    # Hardware Abstraction Layer
│   ├── gpio.hpp
│   ├── uart.hpp
│   ├── spi.hpp
│   └── i2c.hpp
├── drivers/                # Peripheral drivers
│   ├── led_driver.cpp
│   └── sensor_driver.cpp
├── tests/                  # Unit tests
├── docs/                   # Documentation
├── examples/               # Example applications
├── CMakeLists.txt
├── Makefile
└── README.md
```

## Requirements

- ARM GCC Toolchain (arm-none-eabi-gcc 5.4+)
- CMake 3.5+
- Make or Ninja build system
- OpenOCD or J-Link for flashing (optional)

## Building

### Using CMake

```bash
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-toolchain.cmake ..
make
```

### Using Make

```bash
make all
make flash    # Flash to target device
make clean    # Clean build artifacts
```

## Quick Start

```cpp
#include "system.hpp"
#include "hal/gpio.hpp"

int main() {
    // Initialize system
    System::init();
    
    // Configure LED pin
    GPIO led(GPIOA, GPIO_PIN_5);
    led.setMode(GPIO::Mode::Output);
    
    while (true) {
        led.toggle();
        System::delayMs(500);
    }
    
    return 0;
}
```

## Configuration

Edit `include/config.hpp` to customize:

```cpp
#define SYSTEM_CLOCK_HZ     168000000   // System clock frequency
#define TICK_RATE_HZ        1000        // SysTick rate
#define USE_RTOS            0           // Enable RTOS support
#define DEBUG_ENABLED       1           // Enable debug output
```

## Contributing

Contributions are welcome! Please read our [Contributing Guidelines](CONTRIBUTING.md) before submitting pull requests.

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/new-driver`)
3. Commit your changes (`git commit -am 'Add new sensor driver'`)
4. Push to the branch (`git push origin feature/new-driver`)
5. Open a Pull Request



## Authors

- Farnaz Nasehi 

## Acknowledgments

- ARM CMSIS for Cortex-M definitions

---
