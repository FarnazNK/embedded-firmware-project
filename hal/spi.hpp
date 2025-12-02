/**
 * @file spi.hpp
 * @brief SPI Hardware Abstraction Layer
 * @author Embedded Firmware Project
 * @date 2016
 * @license MIT
 */

#ifndef HAL_SPI_HPP
#define HAL_SPI_HPP

#include "types.hpp"
#include "gpio.hpp"

namespace embedded {
namespace hal {

/**
 * @class SPI
 * @brief Serial Peripheral Interface abstraction
 * 
 * Provides a portable interface for SPI master/slave
 * communication with support for various modes and speeds.
 */
class SPI {
public:
    /**
     * @brief SPI clock polarity
     */
    enum class ClockPolarity : u8 {
        Low  = 0,   ///< Clock idle low
        High = 1    ///< Clock idle high
    };

    /**
     * @brief SPI clock phase
     */
    enum class ClockPhase : u8 {
        FirstEdge  = 0, ///< Data sampled on first clock edge
        SecondEdge = 1  ///< Data sampled on second clock edge
    };

    /**
     * @brief SPI data frame format
     */
    enum class DataSize : u8 {
        Bits8  = 0,
        Bits16 = 1
    };

    /**
     * @brief SPI bit order
     */
    enum class BitOrder : u8 {
        MSBFirst = 0,
        LSBFirst = 1
    };

    /**
     * @brief SPI mode (role)
     */
    enum class Mode : u8 {
        Master = 0,
        Slave  = 1
    };

    /**
     * @brief SPI configuration structure
     */
    struct Config {
        Mode            mode        = Mode::Master;
        ClockPolarity   cpol        = ClockPolarity::Low;
        ClockPhase      cpha        = ClockPhase::FirstEdge;
        DataSize        dataSize    = DataSize::Bits8;
        BitOrder        bitOrder    = BitOrder::MSBFirst;
        u32             clockHz     = 1000000;  ///< SPI clock frequency
        bool            softwareCS  = true;     ///< Software-managed chip select
    };

    /**
     * @brief Constructor
     * @param instance SPI peripheral instance
     */
    explicit SPI(void* instance);

    /**
     * @brief Destructor
     */
    ~SPI();

    /**
     * @brief Initialize SPI with configuration
     * @param config SPI configuration
     * @return Status::Ok on success
     */
    Status init(const Config& config);

    /**
     * @brief Deinitialize SPI
     * @return Status::Ok on success
     */
    Status deinit();

    /**
     * @brief Transfer single byte
     * @param txData Byte to transmit
     * @return Received byte
     */
    u8 transfer(u8 txData);

    /**
     * @brief Transfer 16-bit word
     * @param txData Word to transmit
     * @return Received word
     */
    u16 transfer16(u16 txData);

    /**
     * @brief Transfer buffer (full duplex)
     * @param txData Transmit buffer (can be nullptr for receive-only)
     * @param rxData Receive buffer (can be nullptr for transmit-only)
     * @param length Number of bytes to transfer
     * @return Status::Ok on success
     */
    Status transfer(const u8* txData, u8* rxData, size_t length);

    /**
     * @brief Transmit buffer (ignore received data)
     * @param data Transmit buffer
     * @param length Number of bytes to transmit
     * @return Status::Ok on success
     */
    Status transmit(const u8* data, size_t length);

    /**
     * @brief Receive buffer (transmit dummy bytes)
     * @param data Receive buffer
     * @param length Number of bytes to receive
     * @return Status::Ok on success
     */
    Status receive(u8* data, size_t length);

    /**
     * @brief Set chip select pin
     * @param csPin Pointer to GPIO for chip select
     */
    void setChipSelect(GPIO* csPin);

    /**
     * @brief Assert chip select (active low)
     */
    void selectDevice();

    /**
     * @brief Deassert chip select
     */
    void deselectDevice();

    /**
     * @brief Set SPI clock frequency
     * @param clockHz Desired clock frequency in Hz
     * @return Actual clock frequency achieved
     */
    u32 setClockFrequency(u32 clockHz);

    /**
     * @brief Check if SPI is busy
     * @return true if transfer in progress
     */
    bool isBusy() const;

private:
    void*   m_instance;
    Config  m_config;
    GPIO*   m_csPin;
    
    void enableClock();
    void configurePins();
    u8 calculatePrescaler(u32 clockHz);
};

} // namespace hal
} // namespace embedded

#endif // HAL_SPI_HPP
