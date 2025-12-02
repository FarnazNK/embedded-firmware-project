# API Documentation

## Embedded Firmware Framework

*Version 1.0.0 - 2016*

---

## Table of Contents

1. [Overview](#overview)
2. [System Module](#system-module)
3. [HAL - GPIO](#hal---gpio)
4. [HAL - UART](#hal---uart)
5. [HAL - SPI](#hal---spi)
6. [HAL - I2C](#hal---i2c)
7. [Drivers](#drivers)

---

## Overview

The Embedded Firmware Framework provides a portable hardware abstraction layer (HAL) for ARM Cortex-M microcontrollers. It is designed to be:

- **Portable**: Works across multiple MCU families
- **Efficient**: Minimal overhead and memory usage
- **Modern**: Leverages C++14 features for type safety
- **Testable**: Designed with unit testing in mind

### Namespace Structure

```cpp
embedded::          // Root namespace
  hal::             // Hardware abstraction layer
  drivers::         // Device drivers
```

---

## System Module

The System class provides core system functionality.

### Header
```cpp
#include "system.hpp"
```

### Initialization
```cpp
embedded::Status status = embedded::System::init();
```

### Functions

| Function | Description |
|----------|-------------|
| `init()` | Initialize system clocks and peripherals |
| `reset()` | Perform software reset |
| `getTicks()` | Get system tick count (ms) |
| `delayMs(u32 ms)` | Blocking delay in milliseconds |
| `delayUs(u32 us)` | Blocking delay in microseconds |
| `getSystemClock()` | Get system clock frequency |
| `sleep()` | Enter low power sleep mode |
| `deepSleep()` | Enter deep sleep (stop) mode |

### Example

```cpp
#include "system.hpp"

using namespace embedded;

int main() {
    System::init();
    
    while (true) {
        // Do work...
        System::delayMs(1000);
    }
}
```

---

## HAL - GPIO

General Purpose Input/Output abstraction.

### Header
```cpp
#include "hal/gpio.hpp"
```

### Enumerations

#### GPIO::Mode
| Value | Description |
|-------|-------------|
| `Input` | Input mode (floating) |
| `Output` | Output push-pull |
| `OutputOD` | Output open-drain |
| `Alternate` | Alternate function mode |
| `Analog` | Analog mode |

#### GPIO::Pull
| Value | Description |
|-------|-------------|
| `None` | No pull-up/pull-down |
| `Up` | Pull-up resistor enabled |
| `Down` | Pull-down resistor enabled |

#### GPIO::Speed
| Value | Description |
|-------|-------------|
| `Low` | Low speed (2 MHz) |
| `Medium` | Medium speed (10 MHz) |
| `High` | High speed (50 MHz) |
| `VeryHigh` | Very high speed (100 MHz) |

### Functions

| Function | Description |
|----------|-------------|
| `setMode(Mode)` | Configure pin mode |
| `setPull(Pull)` | Configure pull resistor |
| `setSpeed(Speed)` | Configure output speed |
| `setHigh()` | Set output high |
| `setLow()` | Set output low |
| `toggle()` | Toggle output |
| `read()` | Read pin state |
| `isHigh()` | Check if pin is high |
| `enableInterrupt(Trigger, Callback)` | Enable edge interrupt |

### Example

```cpp
#include "hal/gpio.hpp"

using namespace embedded::hal;

// Configure LED output
GPIO led(GPIOA, 5);
led.setMode(GPIO::Mode::Output);
led.setSpeed(GPIO::Speed::Low);

// Blink LED
led.setHigh();
System::delayMs(500);
led.setLow();

// Configure button input with interrupt
GPIO button(GPIOC, 13);
button.setMode(GPIO::Mode::Input);
button.setPull(GPIO::Pull::Up);
button.enableInterrupt(GPIO::Trigger::Falling, [](void*) {
    // Button pressed
});
```

---

## HAL - UART

Universal Asynchronous Receiver/Transmitter abstraction.

### Header
```cpp
#include "hal/uart.hpp"
```

### Configuration Structure

```cpp
UART::Config config;
config.baudRate = 115200;
config.dataBits = UART::DataBits::Eight;
config.parity = UART::Parity::None;
config.stopBits = UART::StopBits::One;
config.flowControl = UART::FlowControl::None;
```

### Functions

| Function | Description |
|----------|-------------|
| `init(Config)` | Initialize with configuration |
| `deinit()` | Deinitialize UART |
| `transmit(u8)` | Transmit single byte |
| `transmit(data, length)` | Transmit buffer |
| `print(str)` | Transmit null-terminated string |
| `receive(data, timeout)` | Receive byte with timeout |
| `startReceiveIT(callback)` | Start interrupt reception |

### Example

```cpp
#include "hal/uart.hpp"

using namespace embedded::hal;

UART serial(USART2);

UART::Config config;
config.baudRate = 115200;
serial.init(config);

serial.print("Hello, World!\r\n");

// Interrupt-based receive
serial.startReceiveIT([](u8 data, void*) {
    // Process received byte
});
```

---

## HAL - SPI

Serial Peripheral Interface abstraction.

### Header
```cpp
#include "hal/spi.hpp"
```

### Functions

| Function | Description |
|----------|-------------|
| `init(Config)` | Initialize SPI |
| `transfer(u8)` | Transfer single byte |
| `transfer(tx, rx, len)` | Full-duplex transfer |
| `selectDevice()` | Assert chip select |
| `deselectDevice()` | Deassert chip select |

### Example

```cpp
#include "hal/spi.hpp"

using namespace embedded::hal;

SPI spi(SPI1);

SPI::Config config;
config.mode = SPI::Mode::Master;
config.clockHz = 1000000;  // 1 MHz
spi.init(config);

// Transfer data
spi.selectDevice();
u8 response = spi.transfer(0x9F);  // Read ID command
spi.deselectDevice();
```

---

## HAL - I2C

Inter-Integrated Circuit abstraction.

### Header
```cpp
#include "hal/i2c.hpp"
```

### Functions

| Function | Description |
|----------|-------------|
| `init(Config)` | Initialize I2C |
| `write(addr, data, len)` | Write to device |
| `read(addr, data, len)` | Read from device |
| `writeRegister(addr, reg, data)` | Write register |
| `readRegister(addr, reg, data)` | Read register |
| `scanBus(addrs, max)` | Scan for devices |

### Example

```cpp
#include "hal/i2c.hpp"

using namespace embedded::hal;

I2C i2c(I2C1);

I2C::Config config;
config.speed = I2C::Speed::Fast;  // 400 kHz
i2c.init(config);

// Read temperature from sensor at address 0x48
u8 temp;
i2c.readRegister(0x48, 0x00, &temp);
```

---

## Drivers

### LED Driver

High-level LED control with effects.

```cpp
#include "drivers/led_driver.hpp"

using namespace embedded::drivers;

GPIO ledGpio(GPIOA, 5);
ledGpio.setMode(GPIO::Mode::Output);

LedDriver led(&ledGpio);

// Simple on/off
led.on();
led.off();

// Set pattern
led.setPattern(LedDriver::Pattern::Heartbeat);

// Update in main loop
while (true) {
    led.update(10);  // 10ms elapsed
    System::delayMs(10);
}
```

---

## Error Handling

All HAL functions return a `Status` enum:

```cpp
enum class Status : u8 {
    Ok,         // Success
    Error,      // General error
    Busy,       // Resource busy
    Timeout,    // Operation timed out
    InvalidArg, // Invalid argument
    NotReady,   // Not initialized
    NoMemory,   // Out of memory
    NotFound,   // Resource not found
    Permission, // Permission denied
    HwError     // Hardware error
};
```

### Example Error Handling

```cpp
Status status = uart.init(config);
if (status != Status::Ok) {
    // Handle error
    led.setPattern(LedDriver::Pattern::FastBlink);
}
```

---

*Copyright (c) 2016 - MIT License*
