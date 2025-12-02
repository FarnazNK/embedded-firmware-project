/**
 * @file led_driver.hpp
 * @brief LED Driver with effects support
 * @author Embedded Firmware Project
 * @date 2016
 * @license MIT
 */

#ifndef DRIVERS_LED_DRIVER_HPP
#define DRIVERS_LED_DRIVER_HPP

#include "types.hpp"
#include "hal/gpio.hpp"

namespace embedded {
namespace drivers {

/**
 * @class LedDriver
 * @brief LED control with blinking and dimming effects
 * 
 * Provides high-level LED control including:
 * - On/Off control
 * - Blinking patterns
 * - PWM dimming (requires timer)
 */
class LedDriver {
public:
    /**
     * @brief LED active state (low or high)
     */
    enum class ActiveState : u8 {
        Low,    ///< LED on when GPIO is low (active low)
        High    ///< LED on when GPIO is high (active high)
    };

    /**
     * @brief Blink pattern modes
     */
    enum class Pattern : u8 {
        Solid,      ///< Always on
        Blink,      ///< Regular on/off blink
        FastBlink,  ///< Fast blinking (error indication)
        SlowBlink,  ///< Slow blinking (standby)
        Heartbeat,  ///< Two quick blinks + pause
        SOS         ///< SOS morse code pattern
    };

    /**
     * @brief Constructor
     * @param gpio Pointer to configured GPIO object
     * @param activeState Active state configuration
     */
    LedDriver(hal::GPIO* gpio, ActiveState activeState = ActiveState::High);

    /**
     * @brief Destructor
     */
    ~LedDriver() = default;

    /**
     * @brief Turn LED on
     */
    void on();

    /**
     * @brief Turn LED off
     */
    void off();

    /**
     * @brief Toggle LED state
     */
    void toggle();

    /**
     * @brief Check if LED is on
     * @return true if LED is currently on
     */
    bool isOn() const;

    /**
     * @brief Set blink pattern
     * @param pattern Blink pattern to use
     */
    void setPattern(Pattern pattern);

    /**
     * @brief Get current pattern
     * @return Current blink pattern
     */
    Pattern getPattern() const;

    /**
     * @brief Set custom blink timing
     * @param onTime On time in milliseconds
     * @param offTime Off time in milliseconds
     */
    void setBlinkTiming(u16 onTime, u16 offTime);

    /**
     * @brief Update LED state (call periodically)
     * 
     * This function should be called periodically (e.g., every 10ms)
     * to update blink patterns and effects.
     * 
     * @param elapsedMs Milliseconds since last update
     */
    void update(u32 elapsedMs);

    /**
     * @brief Blink LED a specific number of times
     * @param count Number of blinks
     * @param onTime On time per blink in ms
     * @param offTime Off time per blink in ms
     */
    void blinkCount(u8 count, u16 onTime = 200, u16 offTime = 200);

private:
    hal::GPIO*  m_gpio;
    ActiveState m_activeState;
    Pattern     m_pattern;
    bool        m_isOn;
    
    u32         m_timer;
    u16         m_onTime;
    u16         m_offTime;
    
    u8          m_blinkCount;
    u8          m_blinkRemaining;
    u8          m_patternStep;
    
    void setPhysicalState(bool on);
    void updatePattern(u32 elapsedMs);
};

} // namespace drivers
} // namespace embedded

#endif // DRIVERS_LED_DRIVER_HPP
