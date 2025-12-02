/**
 * @file main.cpp
 * @brief Application entry point
 * @author Embedded Firmware Project
 * @date 2016
 * @license MIT
 */

#include "system.hpp"
#include "hal/gpio.hpp"
#include "hal/uart.hpp"

using namespace embedded;
using namespace embedded::hal;

// Hardware definitions (adjust for your board)
#define LED_PORT            GPIOA
#define LED_PIN             5

#define DEBUG_UART          USART2

/**
 * @brief Application entry point
 */
int main() {
    // Initialize system clocks and peripherals
    if (System::init() != Status::Ok) {
        // Initialization failed - enter error state
        while (true) {
            // Error handler
        }
    }

    // Configure LED GPIO
    GPIO led(LED_PORT, LED_PIN);
    led.setMode(GPIO::Mode::Output);
    led.setSpeed(GPIO::Speed::Low);
    led.setPull(GPIO::Pull::None);

    // Configure debug UART
    UART debug(DEBUG_UART);
    UART::Config uartConfig;
    uartConfig.baudRate = 115200;
    uartConfig.dataBits = UART::DataBits::Eight;
    uartConfig.parity = UART::Parity::None;
    uartConfig.stopBits = UART::StopBits::One;
    
    if (debug.init(uartConfig) != Status::Ok) {
        // UART initialization failed
        while (true) {
            led.toggle();
            System::delayMs(100);
        }
    }

    // Print startup message
    debug.print("\r\n");
    debug.print("================================\r\n");
    debug.print("  Embedded Firmware Framework\r\n");
    debug.print("  Version 1.0.0 (2016)\r\n");
    debug.print("================================\r\n");
    debug.print("\r\n");
    debug.print("System initialized successfully.\r\n");

    // Main application loop
    u32 counter = 0;
    while (true) {
        // Toggle LED
        led.toggle();
        
        // Print heartbeat message every second
        counter++;
        if (counter % 2 == 0) {
            debug.print("Heartbeat: ");
            // Simple integer to string conversion
            char buf[16];
            u32 ticks = System::getTicks() / 1000;
            int idx = 0;
            if (ticks == 0) {
                buf[idx++] = '0';
            } else {
                char temp[16];
                int tempIdx = 0;
                while (ticks > 0) {
                    temp[tempIdx++] = '0' + (ticks % 10);
                    ticks /= 10;
                }
                while (tempIdx > 0) {
                    buf[idx++] = temp[--tempIdx];
                }
            }
            buf[idx++] = 's';
            buf[idx++] = '\r';
            buf[idx++] = '\n';
            buf[idx] = '\0';
            debug.print(buf);
        }
        
        // Delay 500ms
        System::delayMs(500);
    }

    return 0;
}

/**
 * @brief Hard Fault Handler
 */
extern "C" void HardFault_Handler(void) {
    while (true) {
        // Trap in hard fault
    }
}

/**
 * @brief Memory Management Fault Handler
 */
extern "C" void MemManage_Handler(void) {
    while (true) {
        // Trap in memory management fault
    }
}

/**
 * @brief Bus Fault Handler
 */
extern "C" void BusFault_Handler(void) {
    while (true) {
        // Trap in bus fault
    }
}

/**
 * @brief Usage Fault Handler
 */
extern "C" void UsageFault_Handler(void) {
    while (true) {
        // Trap in usage fault
    }
}
