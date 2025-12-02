/**
 * @file blinky.cpp
 * @brief Simple LED blink example
 * @author Embedded Firmware Project
 * @date 2016
 * @license MIT
 * 
 * This example demonstrates basic GPIO usage with the
 * embedded firmware framework. It blinks an LED at 1Hz
 * and outputs debug messages via UART.
 * 
 * Hardware Setup:
 *   - LED connected to PA5 (most STM32 Nucleo boards)
 *   - UART TX on PA2, RX on PA3 (USART2)
 */

#include "system.hpp"
#include "hal/gpio.hpp"
#include "hal/uart.hpp"

using namespace embedded;
using namespace embedded::hal;

/*============================================================================
 * Hardware Configuration
 *===========================================================================*/
// Adjust these for your specific board
#define LED_PORT        GPIOA
#define LED_PIN         5

#define BTN_PORT        GPIOC
#define BTN_PIN         13

#define UART_INSTANCE   USART2

/*============================================================================
 * Application State
 *===========================================================================*/
static volatile bool buttonPressed = false;
static u32 blinkRate = 500;  // ms

/*============================================================================
 * Button Callback
 *===========================================================================*/
void onButtonPress(void* context) {
    (void)context;
    buttonPressed = true;
}

/*============================================================================
 * Main Application
 *===========================================================================*/
int main() {
    // Initialize system
    System::init();
    
    // Configure LED
    GPIO led(LED_PORT, LED_PIN);
    led.setMode(GPIO::Mode::Output);
    led.setSpeed(GPIO::Speed::Low);
    
    // Configure button with interrupt
    GPIO button(BTN_PORT, BTN_PIN);
    button.setMode(GPIO::Mode::Input);
    button.setPull(GPIO::Pull::Up);
    button.enableInterrupt(GPIO::Trigger::Falling, onButtonPress);
    
    // Configure debug UART
    UART debug(UART_INSTANCE);
    UART::Config config;
    config.baudRate = 115200;
    debug.init(config);
    
    debug.print("\r\n");
    debug.print("=================================\r\n");
    debug.print("  Blinky Example Application\r\n");
    debug.print("  Press button to change rate\r\n");
    debug.print("=================================\r\n");
    
    // Blink rates (cycle through on button press)
    const u32 rates[] = {500, 250, 100, 1000};
    const u8 numRates = sizeof(rates) / sizeof(rates[0]);
    u8 rateIndex = 0;
    
    // Main loop
    while (true) {
        // Toggle LED
        led.toggle();
        
        // Check for button press
        if (buttonPressed) {
            buttonPressed = false;
            
            // Cycle to next blink rate
            rateIndex = (rateIndex + 1) % numRates;
            blinkRate = rates[rateIndex];
            
            debug.print("Blink rate: ");
            // Print rate value
            char buf[8];
            u32 rate = blinkRate;
            int idx = 0;
            if (rate == 0) {
                buf[idx++] = '0';
            } else {
                char temp[8];
                int tempIdx = 0;
                while (rate > 0) {
                    temp[tempIdx++] = '0' + (rate % 10);
                    rate /= 10;
                }
                while (tempIdx > 0) {
                    buf[idx++] = temp[--tempIdx];
                }
            }
            buf[idx++] = 'm';
            buf[idx++] = 's';
            buf[idx++] = '\r';
            buf[idx++] = '\n';
            buf[idx] = '\0';
            debug.print(buf);
        }
        
        // Delay
        System::delayMs(blinkRate);
    }
    
    return 0;
}
