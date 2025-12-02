/**
 * @file gpio.hpp
 * @brief GPIO Hardware Abstraction Layer
 * @author Embedded Firmware Project
 * @date 2016
 * @license MIT
 */

#ifndef HAL_GPIO_HPP
#define HAL_GPIO_HPP

#include "types.hpp"

namespace embedded {
namespace hal {

/**
 * @class GPIO
 * @brief General Purpose Input/Output abstraction
 * 
 * Provides a portable interface for GPIO operations
 * across different microcontroller platforms.
 */
class GPIO {
public:
    /**
     * @brief GPIO pin modes
     */
    enum class Mode : u8 {
        Input       = 0x00,     ///< Input mode (floating)
        Output      = 0x01,     ///< Output push-pull
        OutputOD    = 0x02,     ///< Output open-drain
        Alternate   = 0x03,     ///< Alternate function
        Analog      = 0x04      ///< Analog mode
    };

    /**
     * @brief GPIO pull-up/pull-down configuration
     */
    enum class Pull : u8 {
        None    = 0x00,         ///< No pull-up/pull-down
        Up      = 0x01,         ///< Pull-up enabled
        Down    = 0x02          ///< Pull-down enabled
    };

    /**
     * @brief GPIO output speed
     */
    enum class Speed : u8 {
        Low     = 0x00,         ///< Low speed (2 MHz)
        Medium  = 0x01,         ///< Medium speed (10 MHz)
        High    = 0x02,         ///< High speed (50 MHz)
        VeryHigh = 0x03         ///< Very high speed (100 MHz)
    };

    /**
     * @brief GPIO interrupt trigger edge
     */
    enum class Trigger : u8 {
        Rising  = 0x01,         ///< Rising edge trigger
        Falling = 0x02,         ///< Falling edge trigger
        Both    = 0x03          ///< Both edges trigger
    };

    /**
     * @brief Callback function type for GPIO interrupts
     */
    using Callback = void (*)(void* context);

    /**
     * @brief Constructor
     * @param port GPIO port base address
     * @param pin Pin number (0-15)
     */
    GPIO(void* port, u8 pin);

    /**
     * @brief Destructor
     */
    ~GPIO() = default;

    /**
     * @brief Configure pin mode
     * @param mode Pin mode
     * @return Status::Ok on success
     */
    Status setMode(Mode mode);

    /**
     * @brief Configure pull-up/pull-down
     * @param pull Pull configuration
     * @return Status::Ok on success
     */
    Status setPull(Pull pull);

    /**
     * @brief Configure output speed
     * @param speed Output speed
     * @return Status::Ok on success
     */
    Status setSpeed(Speed speed);

    /**
     * @brief Set alternate function number
     * @param af Alternate function number (0-15)
     * @return Status::Ok on success
     */
    Status setAlternateFunction(u8 af);

    /**
     * @brief Set pin output high
     */
    void setHigh();

    /**
     * @brief Set pin output low
     */
    void setLow();

    /**
     * @brief Toggle pin output
     */
    void toggle();

    /**
     * @brief Write pin state
     * @param state Pin state
     */
    void write(PinState state);

    /**
     * @brief Read pin state
     * @return Current pin state
     */
    PinState read() const;

    /**
     * @brief Check if pin is high
     * @return true if pin is high
     */
    bool isHigh() const;

    /**
     * @brief Check if pin is low
     * @return true if pin is low
     */
    bool isLow() const;

    /**
     * @brief Enable interrupt on pin
     * @param trigger Trigger edge configuration
     * @param callback Callback function
     * @param context User context passed to callback
     * @return Status::Ok on success
     */
    Status enableInterrupt(Trigger trigger, Callback callback, void* context = nullptr);

    /**
     * @brief Disable interrupt on pin
     * @return Status::Ok on success
     */
    Status disableInterrupt();

    /**
     * @brief Lock pin configuration
     * @return Status::Ok on success
     */
    Status lock();

private:
    void*   m_port;
    u8      m_pin;
    Mode    m_mode;
    
    void configureExti(Trigger trigger);
};

} // namespace hal
} // namespace embedded

#endif // HAL_GPIO_HPP
