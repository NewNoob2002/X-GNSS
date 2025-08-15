#ifndef USART_H
#define USART_H
#include "HardwareSerial.h"
#include "usart_config.h"

#ifndef SERIAL_BUFFER_SIZE
#define SERIAL_BUFFER_SIZE 64
#endif
#ifndef SERIAL_TX_BUFFER_SIZE
#define SERIAL_TX_BUFFER_SIZE SERIAL_BUFFER_SIZE
#endif
#ifndef SERIAL_RX_BUFFER_SIZE
#define SERIAL_RX_BUFFER_SIZE SERIAL_BUFFER_SIZE
#endif

class Usart : public HardwareSerial
{
    typedef void (*CallbackFunction_t)(HardwareSerial *serial);

public:
    Usart(usart_config_t *usart_config, size_t rx_buffer_size = SERIAL_RX_BUFFER_SIZE, size_t tx_buffer_size = SERIAL_TX_BUFFER_SIZE);
    ~Usart();
    void attachInterrupt(CallbackFunction_t func);
    void begin(uint32_t baud);
    void begin(uint32_t baud, uint16_t config, bool irqn_enable = true);
    void end();
    int available();
    int availableForWrite();
    int peek();
    int read();
    void flush();
    size_t write(uint8_t ch);
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool() { return true; }
    /**
     * @brief access the base usart config struct
     */
    const usart_config_t *c_dev(void) { return this->_usart_config; }
    /**
     * @brief get the last receive error
     * @note calling this function clears the error
     */
    const usart_receive_error_t getReceiveError(void);
#ifdef USART_RX_ERROR_COUNTERS_ENABLE
    /**
     * @brief get the number of framing errors
     * @note usart_receive_error_t::FramingError
     */
    inline uint32_t getFramingErrorCount(void) { return this->config->state.rx_error_counters.framing_error; }

    /**
     * @brief get the number of parity errors
     * @note usart_receive_error_t::ParityError
     */
    inline uint32_t getParityErrorCount(void) { return this->config->state.rx_error_counters.parity_error; }

    /**
     * @brief get the number of overrun errors
     * @note usart_receive_error_t::OverrunError
     */
    inline uint32_t getOverrunErrorCount(void) { return this->config->state.rx_error_counters.overrun_error; }

    /**
     * @brief get how often bytes were dropped from the RX buffer
     * @note usart_receive_error_t::RxDataDropped
     */
    inline uint32_t getDroppedDataErrorCount(void) { return this->config->state.rx_error_counters.rx_data_dropped; }
#endif // USART_RX_ERROR_COUNTERS_ENABLE

#ifdef USART_RX_DMA_SUPPORT
public:
    /**
     * @brief enable RX DMA for this Usart
     * @param dma pointer to the DMA peripheral
     * @param channel DMA channel to use for RX
     * @note must be called before begin()
     */
    void enableRxDma(M4_DMA_TypeDef *dma, en_dma_channel_t channel);

    /**
     * @brief disable RX DMA for this Usart
     * @note must be called before begin()
     * @note if begin() was already called, this function MUST be called before end()
     */
    void disableRxDma();

private:
    void rx_dma_init();
    void rx_dma_deinit();
#endif // USART_RX_DMA_SUPPORT

    void irqHandler();

private:
    usart_config_t *_usart_config;
    CallbackFunction_t _callbackFunction;
    // tx / rx pin numbers
    gpio_pin_t tx_pin;
    gpio_pin_t rx_pin;
    // rx / tx buffers
    RingBuffer<uint8_t> *rx_buffer;
    RingBuffer<uint8_t> *tx_buffer;

    uint16_t rx_buffer_size;
    uint16_t tx_buffer_size;

    bool irqn_enable;
    // is initialized? (begin() called)
    bool initialized;
};

#ifdef USART1_ENABLE
extern Usart Serial1;

#define Serial Serial1
#endif

#endif // USART_H