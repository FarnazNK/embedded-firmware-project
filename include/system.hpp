/**
 * @file system.hpp
 * @brief System initialization and core utilities
 * @author Embedded Firmware Project
 * @date 2016
 * @license MIT
 */

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "types.hpp"
#include "config.hpp"

namespace embedded {

/**
 * @class System
 * @brief Core system management class
 * 
 * Provides system initialization, clock configuration,
 * and timing utilities for the embedded platform.
 */
class System {
public:
    /**
     * @brief Initialize the system
     * 
     * Configures system clocks, enables peripherals,
     * and sets up the SysTick timer.
     * 
     * @return Status::Ok on success
     */
    static Status init();

    /**
     * @brief Reset the system
     */
    static void reset();

    /**
     * @brief Get system tick count
     * @return Current tick count in milliseconds
     */
    static u32 getTicks();

    /**
     * @brief Delay execution for specified milliseconds
     * @param ms Delay duration in milliseconds
     */
    static void delayMs(u32 ms);

    /**
     * @brief Delay execution for specified microseconds
     * @param us Delay duration in microseconds
     */
    static void delayUs(u32 us);

    /**
     * @brief Get system clock frequency
     * @return System clock frequency in Hz
     */
    static u32 getSystemClock();

    /**
     * @brief Enter low power sleep mode
     */
    static void sleep();

    /**
     * @brief Enter deep sleep (stop) mode
     */
    static void deepSleep();

    /**
     * @brief Get unique device ID
     * @param id Pointer to array to store 96-bit ID (3 x u32)
     */
    static void getUniqueId(u32* id);

    /**
     * @brief Enable specific peripheral clock
     * @param peripheral Peripheral identifier
     * @return Status::Ok on success
     */
    static Status enablePeripheralClock(u32 peripheral);

    /**
     * @brief Disable specific peripheral clock
     * @param peripheral Peripheral identifier
     * @return Status::Ok on success
     */
    static Status disablePeripheralClock(u32 peripheral);

private:
    static volatile u32 s_tickCount;
    
    static void initClocks();
    static void initSysTick();
    static void initNvic();
};

} // namespace embedded

#endif // SYSTEM_HPP
