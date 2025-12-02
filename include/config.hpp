/**
 * @file config.hpp
 * @brief System configuration parameters
 * @author Embedded Firmware Project
 * @date 2016
 * @license MIT
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP

/*============================================================================
 * Target Configuration
 *===========================================================================*/
#define TARGET_STM32F4          1
#define TARGET_STM32F1          0
#define TARGET_LPC1768          0
#define TARGET_NRF52            0

/*============================================================================
 * System Clock Configuration
 *===========================================================================*/
#define SYSTEM_CLOCK_HZ         168000000UL     // 168 MHz system clock
#define AHB_CLOCK_HZ            SYSTEM_CLOCK_HZ
#define APB1_CLOCK_HZ           (SYSTEM_CLOCK_HZ / 4)
#define APB2_CLOCK_HZ           (SYSTEM_CLOCK_HZ / 2)

/*============================================================================
 * SysTick Configuration
 *===========================================================================*/
#define TICK_RATE_HZ            1000            // 1ms tick rate
#define TICK_PERIOD_MS          (1000 / TICK_RATE_HZ)

/*============================================================================
 * Feature Flags
 *===========================================================================*/
#define USE_RTOS                0               // 0: Bare-metal, 1: FreeRTOS
#define DEBUG_ENABLED           1               // Enable debug output
#define USE_WATCHDOG            1               // Enable watchdog timer
#define LOW_POWER_MODE          0               // Enable low power features

/*============================================================================
 * Peripheral Configuration
 *===========================================================================*/
#define UART_BUFFER_SIZE        256
#define SPI_BUFFER_SIZE         128
#define I2C_TIMEOUT_MS          100

/*============================================================================
 * Debug Configuration
 *===========================================================================*/
#if DEBUG_ENABLED
    #define DEBUG_UART_BAUDRATE 115200
    #define DEBUG_UART_PORT     USART2
#endif

/*============================================================================
 * Version Information
 *===========================================================================*/
#define FIRMWARE_VERSION_MAJOR  1
#define FIRMWARE_VERSION_MINOR  0
#define FIRMWARE_VERSION_PATCH  0
#define FIRMWARE_BUILD_DATE     "2016-01-01"

#endif // CONFIG_HPP
