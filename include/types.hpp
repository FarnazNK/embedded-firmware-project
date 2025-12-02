/**
 * @file types.hpp
 * @brief Common type definitions for embedded systems
 * @author Embedded Firmware Project
 * @date 2016
 * @license MIT
 */

#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstdint>
#include <cstddef>

namespace embedded {

/*============================================================================
 * Standard Integer Types
 *===========================================================================*/
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

/*============================================================================
 * Register Types
 *===========================================================================*/
using reg8_t  = volatile uint8_t;
using reg16_t = volatile uint16_t;
using reg32_t = volatile uint32_t;

/*============================================================================
 * Status and Error Codes
 *===========================================================================*/
enum class Status : u8 {
    Ok          = 0x00,
    Error       = 0x01,
    Busy        = 0x02,
    Timeout     = 0x03,
    InvalidArg  = 0x04,
    NotReady    = 0x05,
    NoMemory    = 0x06,
    NotFound    = 0x07,
    Permission  = 0x08,
    HwError     = 0x09
};

/*============================================================================
 * Pin State
 *===========================================================================*/
enum class PinState : u8 {
    Low  = 0,
    High = 1
};

/*============================================================================
 * Interrupt Priority Levels
 *===========================================================================*/
enum class IrqPriority : u8 {
    Highest = 0,
    High    = 1,
    Medium  = 2,
    Low     = 3,
    Lowest  = 4
};

/*============================================================================
 * Utility Macros
 *===========================================================================*/
#define BIT(n)                  (1UL << (n))
#define SET_BIT(reg, bit)       ((reg) |= (bit))
#define CLEAR_BIT(reg, bit)     ((reg) &= ~(bit))
#define READ_BIT(reg, bit)      ((reg) & (bit))
#define TOGGLE_BIT(reg, bit)    ((reg) ^= (bit))

#define ARRAY_SIZE(arr)         (sizeof(arr) / sizeof((arr)[0]))

#define UNUSED(x)               ((void)(x))

/*============================================================================
 * Memory Barrier Macros
 *===========================================================================*/
#define DMB()                   __asm volatile ("dmb" ::: "memory")
#define DSB()                   __asm volatile ("dsb" ::: "memory")
#define ISB()                   __asm volatile ("isb" ::: "memory")

/*============================================================================
 * Critical Section Helpers
 *===========================================================================*/
inline u32 disableInterrupts() {
    u32 primask;
    __asm volatile ("mrs %0, primask" : "=r" (primask));
    __asm volatile ("cpsid i" ::: "memory");
    return primask;
}

inline void restoreInterrupts(u32 primask) {
    __asm volatile ("msr primask, %0" :: "r" (primask) : "memory");
}

/*============================================================================
 * RAII Critical Section Guard
 *===========================================================================*/
class CriticalSection {
public:
    CriticalSection() : m_primask(disableInterrupts()) {}
    ~CriticalSection() { restoreInterrupts(m_primask); }
    
    // Non-copyable
    CriticalSection(const CriticalSection&) = delete;
    CriticalSection& operator=(const CriticalSection&) = delete;

private:
    u32 m_primask;
};

} // namespace embedded

#endif // TYPES_HPP
