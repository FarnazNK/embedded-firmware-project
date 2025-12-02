/**
 * @file startup.cpp
 * @brief Startup code and vector table for ARM Cortex-M4
 * @author Embedded Firmware Project
 * @date 2016
 * @license MIT
 * 
 * This file contains the startup code including:
 * - Vector table
 * - Reset handler
 * - Default interrupt handlers
 * - Memory initialization
 */

#include <cstdint>

/*============================================================================
 * External Symbols (defined by linker script)
 *===========================================================================*/
extern uint32_t _estack;        // End of stack (top of RAM)
extern uint32_t _sidata;        // Start of .data initialization values
extern uint32_t _sdata;         // Start of .data section
extern uint32_t _edata;         // End of .data section
extern uint32_t _sbss;          // Start of .bss section
extern uint32_t _ebss;          // End of .bss section

/*============================================================================
 * Function Prototypes
 *===========================================================================*/
extern "C" {
    void Reset_Handler(void);
    void Default_Handler(void);
    int main(void);
    
    // Cortex-M4 core interrupt handlers
    void NMI_Handler(void)              __attribute__((weak, alias("Default_Handler")));
    void HardFault_Handler(void)        __attribute__((weak));
    void MemManage_Handler(void)        __attribute__((weak, alias("Default_Handler")));
    void BusFault_Handler(void)         __attribute__((weak, alias("Default_Handler")));
    void UsageFault_Handler(void)       __attribute__((weak, alias("Default_Handler")));
    void SVC_Handler(void)              __attribute__((weak, alias("Default_Handler")));
    void DebugMon_Handler(void)         __attribute__((weak, alias("Default_Handler")));
    void PendSV_Handler(void)           __attribute__((weak, alias("Default_Handler")));
    void SysTick_Handler(void)          __attribute__((weak, alias("Default_Handler")));
    
    // STM32F4 peripheral interrupt handlers
    void WWDG_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
    void PVD_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
    void TAMP_STAMP_IRQHandler(void)    __attribute__((weak, alias("Default_Handler")));
    void RTC_WKUP_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
    void FLASH_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
    void RCC_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
    void EXTI0_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
    void EXTI1_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
    void EXTI2_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
    void EXTI3_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
    void EXTI4_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
    void DMA1_Stream0_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
    void DMA1_Stream1_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
    void DMA1_Stream2_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
    void DMA1_Stream3_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
    void DMA1_Stream4_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
    void DMA1_Stream5_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
    void DMA1_Stream6_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
    void ADC_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
    void USART1_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));
    void USART2_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));
    void USART3_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));
    void EXTI9_5_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
    void TIM1_BRK_TIM9_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
    void TIM1_UP_TIM10_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
    void TIM2_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
    void TIM3_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
    void TIM4_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
    void I2C1_EV_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
    void I2C1_ER_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
    void I2C2_EV_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
    void I2C2_ER_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
    void SPI1_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
    void SPI2_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
}

/*============================================================================
 * Vector Table
 *===========================================================================*/
__attribute__((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    // Initial stack pointer
    reinterpret_cast<void (*)(void)>(&_estack),
    
    // Cortex-M4 core handlers
    Reset_Handler,          // Reset Handler
    NMI_Handler,            // NMI Handler
    HardFault_Handler,      // Hard Fault Handler
    MemManage_Handler,      // MPU Fault Handler
    BusFault_Handler,       // Bus Fault Handler
    UsageFault_Handler,     // Usage Fault Handler
    nullptr,                // Reserved
    nullptr,                // Reserved
    nullptr,                // Reserved
    nullptr,                // Reserved
    SVC_Handler,            // SVCall Handler
    DebugMon_Handler,       // Debug Monitor Handler
    nullptr,                // Reserved
    PendSV_Handler,         // PendSV Handler
    SysTick_Handler,        // SysTick Handler
    
    // STM32F4 peripheral handlers
    WWDG_IRQHandler,
    PVD_IRQHandler,
    TAMP_STAMP_IRQHandler,
    RTC_WKUP_IRQHandler,
    FLASH_IRQHandler,
    RCC_IRQHandler,
    EXTI0_IRQHandler,
    EXTI1_IRQHandler,
    EXTI2_IRQHandler,
    EXTI3_IRQHandler,
    EXTI4_IRQHandler,
    DMA1_Stream0_IRQHandler,
    DMA1_Stream1_IRQHandler,
    DMA1_Stream2_IRQHandler,
    DMA1_Stream3_IRQHandler,
    DMA1_Stream4_IRQHandler,
    DMA1_Stream5_IRQHandler,
    DMA1_Stream6_IRQHandler,
    ADC_IRQHandler,
    nullptr,                // Reserved
    nullptr,                // Reserved
    nullptr,                // Reserved
    nullptr,                // Reserved
    EXTI9_5_IRQHandler,
    TIM1_BRK_TIM9_IRQHandler,
    TIM1_UP_TIM10_IRQHandler,
    nullptr,                // Reserved
    nullptr,                // Reserved
    TIM2_IRQHandler,
    TIM3_IRQHandler,
    TIM4_IRQHandler,
    I2C1_EV_IRQHandler,
    I2C1_ER_IRQHandler,
    I2C2_EV_IRQHandler,
    I2C2_ER_IRQHandler,
    SPI1_IRQHandler,
    SPI2_IRQHandler,
    USART1_IRQHandler,
    USART2_IRQHandler,
    USART3_IRQHandler,
};

/*============================================================================
 * Reset Handler
 *===========================================================================*/
void Reset_Handler(void) {
    // Copy .data section from flash to RAM
    uint32_t* src = &_sidata;
    uint32_t* dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    // Zero fill .bss section
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    // Call static constructors
    extern void (*__preinit_array_start[])(void);
    extern void (*__preinit_array_end[])(void);
    extern void (*__init_array_start[])(void);
    extern void (*__init_array_end[])(void);

    for (void (**p)(void) = __preinit_array_start; p < __preinit_array_end; ++p) {
        (*p)();
    }

    for (void (**p)(void) = __init_array_start; p < __init_array_end; ++p) {
        (*p)();
    }

    // Call main
    main();

    // If main returns, loop forever
    while (true) {
        __asm volatile ("nop");
    }
}

/*============================================================================
 * Default Handler
 *===========================================================================*/
void Default_Handler(void) {
    // Infinite loop for unhandled interrupts
    while (true) {
        __asm volatile ("nop");
    }
}
