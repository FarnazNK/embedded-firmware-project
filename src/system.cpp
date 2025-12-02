/**
 * @file system.cpp
 * @brief System initialization and core utilities implementation
 * @author Embedded Firmware Project
 * @date 2016
 * @license MIT
 */

#include "system.hpp"

namespace embedded {

// Static member initialization
volatile u32 System::s_tickCount = 0;

Status System::init() {
    // Initialize system clocks
    initClocks();
    
    // Initialize SysTick timer
    initSysTick();
    
    // Initialize NVIC
    initNvic();
    
    return Status::Ok;
}

void System::reset() {
    // Request system reset via AIRCR register
    // SCB->AIRCR = (0x5FA << 16) | (1 << 2);
    volatile u32* AIRCR = reinterpret_cast<volatile u32*>(0xE000ED0C);
    *AIRCR = (0x5FA << 16) | (1 << 2);
    
    // Wait for reset
    while (true) {
        __asm volatile ("nop");
    }
}

u32 System::getTicks() {
    return s_tickCount;
}

void System::delayMs(u32 ms) {
    u32 start = s_tickCount;
    while ((s_tickCount - start) < ms) {
        __asm volatile ("nop");
    }
}

void System::delayUs(u32 us) {
    // Approximate delay using NOP loops
    // Calibrated for 168 MHz system clock
    u32 cycles = (SYSTEM_CLOCK_HZ / 1000000) * us / 4;
    while (cycles--) {
        __asm volatile ("nop");
    }
}

u32 System::getSystemClock() {
    return SYSTEM_CLOCK_HZ;
}

void System::sleep() {
    // Enter sleep mode (WFI)
    __asm volatile ("wfi");
}

void System::deepSleep() {
    // Set SLEEPDEEP bit
    volatile u32* SCR = reinterpret_cast<volatile u32*>(0xE000ED10);
    *SCR |= (1 << 2);
    
    // Enter deep sleep (WFI)
    __asm volatile ("dsb");
    __asm volatile ("wfi");
    
    // Clear SLEEPDEEP bit after wakeup
    *SCR &= ~(1 << 2);
}

void System::getUniqueId(u32* id) {
    // STM32F4 unique device ID registers
    volatile u32* UID_BASE = reinterpret_cast<volatile u32*>(0x1FFF7A10);
    id[0] = UID_BASE[0];
    id[1] = UID_BASE[1];
    id[2] = UID_BASE[2];
}

Status System::enablePeripheralClock(u32 peripheral) {
    // Implementation depends on target MCU
    // This is a placeholder for STM32F4
    UNUSED(peripheral);
    return Status::Ok;
}

Status System::disablePeripheralClock(u32 peripheral) {
    // Implementation depends on target MCU
    UNUSED(peripheral);
    return Status::Ok;
}

void System::initClocks() {
    // Clock initialization for STM32F4 @ 168 MHz
    // Using external 8 MHz crystal with PLL
    
    // This is a simplified example - full implementation would:
    // 1. Enable HSE (High Speed External) oscillator
    // 2. Wait for HSE ready
    // 3. Configure PLL (M, N, P, Q dividers)
    // 4. Enable PLL
    // 5. Wait for PLL ready
    // 6. Configure flash latency
    // 7. Select PLL as system clock
    // 8. Configure AHB, APB1, APB2 prescalers
}

void System::initSysTick() {
    // Configure SysTick for 1ms interrupts
    // SysTick registers
    volatile u32* SYST_CSR = reinterpret_cast<volatile u32*>(0xE000E010);
    volatile u32* SYST_RVR = reinterpret_cast<volatile u32*>(0xE000E014);
    volatile u32* SYST_CVR = reinterpret_cast<volatile u32*>(0xE000E018);
    
    // Calculate reload value for 1ms tick
    u32 reloadValue = (SYSTEM_CLOCK_HZ / TICK_RATE_HZ) - 1;
    
    // Set reload value
    *SYST_RVR = reloadValue;
    
    // Clear current value
    *SYST_CVR = 0;
    
    // Enable SysTick with processor clock and interrupt
    *SYST_CSR = (1 << 0) |  // Enable
                (1 << 1) |  // Enable interrupt
                (1 << 2);   // Use processor clock
}

void System::initNvic() {
    // Set priority grouping (4 bits preemption, 0 bits subpriority)
    volatile u32* AIRCR = reinterpret_cast<volatile u32*>(0xE000ED0C);
    *AIRCR = (0x5FA << 16) | (3 << 8);
}

} // namespace embedded

/**
 * @brief SysTick interrupt handler
 */
extern "C" void SysTick_Handler(void) {
    embedded::System::s_tickCount++;
}
