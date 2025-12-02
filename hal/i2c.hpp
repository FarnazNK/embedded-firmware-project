/**
 * @file i2c.hpp
 * @brief I2C Hardware Abstraction Layer
 * @author Embedded Firmware Project
 * @date 2016
 * @license MIT
 */

#ifndef HAL_I2C_HPP
#define HAL_I2C_HPP

#include "types.hpp"

namespace embedded {
namespace hal {

/**
 * @class I2C
 * @brief Inter-Integrated Circuit (I2C) abstraction
 * 
 * Provides a portable interface for I2C master
 * communication with support for various speeds.
 */
class I2C {
public:
    /**
     * @brief I2C speed modes
     */
    enum class Speed : u8 {
        Standard = 0,   ///< 100 kHz
        Fast     = 1,   ///< 400 kHz
        FastPlus = 2    ///< 1 MHz
    };

    /**
     * @brief I2C addressing mode
     */
    enum class AddressMode : u8 {
        SevenBit = 0,
        TenBit   = 1
    };

    /**
     * @brief I2C configuration structure
     */
    struct Config {
        Speed       speed       = Speed::Standard;
        AddressMode addressMode = AddressMode::SevenBit;
        bool        analogFilter = true;
        u8          digitalFilter = 0;  ///< 0-15
    };

    /**
     * @brief Constructor
     * @param instance I2C peripheral instance
     */
    explicit I2C(void* instance);

    /**
     * @brief Destructor
     */
    ~I2C();

    /**
     * @brief Initialize I2C with configuration
     * @param config I2C configuration
     * @return Status::Ok on success
     */
    Status init(const Config& config);

    /**
     * @brief Deinitialize I2C
     * @return Status::Ok on success
     */
    Status deinit();

    /**
     * @brief Write data to device
     * @param deviceAddr 7-bit device address
     * @param data Pointer to data buffer
     * @param length Number of bytes to write
     * @param timeout Timeout in milliseconds
     * @return Status::Ok on success
     */
    Status write(u8 deviceAddr, const u8* data, size_t length, u32 timeout = I2C_TIMEOUT_MS);

    /**
     * @brief Read data from device
     * @param deviceAddr 7-bit device address
     * @param data Pointer to receive buffer
     * @param length Number of bytes to read
     * @param timeout Timeout in milliseconds
     * @return Status::Ok on success
     */
    Status read(u8 deviceAddr, u8* data, size_t length, u32 timeout = I2C_TIMEOUT_MS);

    /**
     * @brief Write to register
     * @param deviceAddr 7-bit device address
     * @param regAddr Register address
     * @param data Pointer to data buffer
     * @param length Number of bytes to write
     * @param timeout Timeout in milliseconds
     * @return Status::Ok on success
     */
    Status writeRegister(u8 deviceAddr, u8 regAddr, const u8* data, size_t length, u32 timeout = I2C_TIMEOUT_MS);

    /**
     * @brief Write single byte to register
     * @param deviceAddr 7-bit device address
     * @param regAddr Register address
     * @param data Byte to write
     * @param timeout Timeout in milliseconds
     * @return Status::Ok on success
     */
    Status writeRegister(u8 deviceAddr, u8 regAddr, u8 data, u32 timeout = I2C_TIMEOUT_MS);

    /**
     * @brief Read from register
     * @param deviceAddr 7-bit device address
     * @param regAddr Register address
     * @param data Pointer to receive buffer
     * @param length Number of bytes to read
     * @param timeout Timeout in milliseconds
     * @return Status::Ok on success
     */
    Status readRegister(u8 deviceAddr, u8 regAddr, u8* data, size_t length, u32 timeout = I2C_TIMEOUT_MS);

    /**
     * @brief Read single byte from register
     * @param deviceAddr 7-bit device address
     * @param regAddr Register address
     * @param data Pointer to store byte
     * @param timeout Timeout in milliseconds
     * @return Status::Ok on success
     */
    Status readRegister(u8 deviceAddr, u8 regAddr, u8* data, u32 timeout = I2C_TIMEOUT_MS);

    /**
     * @brief Scan bus for devices
     * @param addresses Array to store found addresses
     * @param maxDevices Maximum number of addresses to find
     * @return Number of devices found
     */
    size_t scanBus(u8* addresses, size_t maxDevices);

    /**
     * @brief Check if device is present
     * @param deviceAddr 7-bit device address
     * @return true if device responds
     */
    bool isDevicePresent(u8 deviceAddr);

    /**
     * @brief Reset the I2C bus
     * @return Status::Ok on success
     */
    Status resetBus();

    /**
     * @brief Check if bus is busy
     * @return true if bus is busy
     */
    bool isBusy() const;

private:
    void*   m_instance;
    Config  m_config;
    
    void enableClock();
    void configurePins();
    void configureTimings();
    
    Status waitForFlag(u32 flag, bool state, u32 timeout);
    Status startCondition(u8 deviceAddr, bool read);
    Status stopCondition();
};

} // namespace hal
} // namespace embedded

#endif // HAL_I2C_HPP
