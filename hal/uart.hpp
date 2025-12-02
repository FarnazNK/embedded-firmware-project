/**
 * @file uart.hpp
 * @brief UART Hardware Abstraction Layer
 * @author Embedded Firmware Project
 * @date 2016
 * @license MIT
 */

#ifndef HAL_UART_HPP
#define HAL_UART_HPP

#include "types.hpp"
#include "config.hpp"

namespace embedded {
namespace hal {

/**
 * @class UART
 * @brief Universal Asynchronous Receiver/Transmitter abstraction
 * 
 * Provides a portable interface for UART communication
 * with support for interrupts and DMA transfers.
 */
class UART {
public:
    /**
     * @brief UART parity configuration
     */
    enum class Parity : u8 {
        None    = 0x00,
        Even    = 0x01,
        Odd     = 0x02
    };

    /**
     * @brief UART stop bits configuration
     */
    enum class StopBits : u8 {
        One     = 0x00,
        Two     = 0x01,
        OneHalf = 0x02
    };

    /**
     * @brief UART data bits configuration
     */
    enum class DataBits : u8 {
        Eight   = 0x00,
        Nine    = 0x01
    };

    /**
     * @brief UART flow control configuration
     */
    enum class FlowControl : u8 {
        None    = 0x00,
        RTS     = 0x01,
        CTS     = 0x02,
        RTS_CTS = 0x03
    };

    /**
     * @brief UART configuration structure
     */
    struct Config {
        u32         baudRate    = 115200;
        DataBits    dataBits    = DataBits::Eight;
        Parity      parity      = Parity::None;
        StopBits    stopBits    = StopBits::One;
        FlowControl flowControl = FlowControl::None;
    };

    /**
     * @brief Callback function type for UART events
     */
    using RxCallback = void (*)(u8 data, void* context);
    using TxCallback = void (*)(void* context);

    /**
     * @brief Constructor
     * @param instance UART peripheral instance (e.g., USART1, USART2)
     */
    explicit UART(void* instance);

    /**
     * @brief Destructor
     */
    ~UART();

    /**
     * @brief Initialize UART with configuration
     * @param config UART configuration
     * @return Status::Ok on success
     */
    Status init(const Config& config);

    /**
     * @brief Deinitialize UART
     * @return Status::Ok on success
     */
    Status deinit();

    /**
     * @brief Transmit single byte (blocking)
     * @param data Byte to transmit
     * @return Status::Ok on success
     */
    Status transmit(u8 data);

    /**
     * @brief Transmit buffer (blocking)
     * @param data Pointer to data buffer
     * @param length Number of bytes to transmit
     * @return Status::Ok on success
     */
    Status transmit(const u8* data, size_t length);

    /**
     * @brief Transmit string (blocking)
     * @param str Null-terminated string
     * @return Status::Ok on success
     */
    Status print(const char* str);

    /**
     * @brief Receive single byte (blocking)
     * @param data Pointer to store received byte
     * @param timeout Timeout in milliseconds
     * @return Status::Ok on success, Status::Timeout on timeout
     */
    Status receive(u8* data, u32 timeout = 0);

    /**
     * @brief Receive buffer (blocking)
     * @param data Pointer to receive buffer
     * @param length Number of bytes to receive
     * @param timeout Timeout in milliseconds
     * @return Status::Ok on success
     */
    Status receive(u8* data, size_t length, u32 timeout = 0);

    /**
     * @brief Start interrupt-based reception
     * @param callback Receive callback function
     * @param context User context
     * @return Status::Ok on success
     */
    Status startReceiveIT(RxCallback callback, void* context = nullptr);

    /**
     * @brief Stop interrupt-based reception
     * @return Status::Ok on success
     */
    Status stopReceiveIT();

    /**
     * @brief Transmit buffer using interrupts
     * @param data Pointer to data buffer
     * @param length Number of bytes to transmit
     * @param callback Completion callback
     * @param context User context
     * @return Status::Ok on success
     */
    Status transmitIT(const u8* data, size_t length, TxCallback callback = nullptr, void* context = nullptr);

    /**
     * @brief Check if transmit is complete
     * @return true if ready for next transmission
     */
    bool isTxReady() const;

    /**
     * @brief Check if data is available for reading
     * @return true if data available
     */
    bool isRxAvailable() const;

    /**
     * @brief Flush transmit buffer
     */
    void flushTx();

    /**
     * @brief Flush receive buffer
     */
    void flushRx();

    /**
     * @brief Set baud rate dynamically
     * @param baudRate New baud rate
     * @return Status::Ok on success
     */
    Status setBaudRate(u32 baudRate);

private:
    void*       m_instance;
    Config      m_config;
    RxCallback  m_rxCallback;
    TxCallback  m_txCallback;
    void*       m_rxContext;
    void*       m_txContext;
    
    u8          m_txBuffer[UART_BUFFER_SIZE];
    volatile u16 m_txHead;
    volatile u16 m_txTail;
    
    void enableClock();
    void configurePins();
    void configureNvic();
};

} // namespace hal
} // namespace embedded

#endif // HAL_UART_HPP
