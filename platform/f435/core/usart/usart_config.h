#ifndef USART_CONFIG_H
#define USART_CONFIG_H

#include "mcu_types.h"
#include "RingBuffer.h"
/**
 * @brief USART peripheral configuration
 */
struct usart_peripheral_config_t
{
    /**
     * @brief USART peripheral register base address
     */
    usart_type *register_base;

    /**
     * @brief USART peripheral clock id
     * @note in FCG1
     */
    crm_periph_clock_type peripheral_clock_id;

       /**
     * @brief USART pin clock id
     * @note in FCG1
     */
    crm_periph_clock_type gpio_clock_id;

    /**
     * @brief pin function for usart tx pin
     */
    gpio_pin_t tx_pin;

    /**
     * @brief pin function for usart rx pin
     */
    gpio_pin_t rx_pin;
    /**
     * @brief pin function for usart clk pin
     */
    gpio_pin_t clk_pin;
		
		gpio_mux_sel_type mux_sel;
};

/**
 * @brief USART single interrupt and handler configuration
 */
struct usart_interrupt_config_t
{
    /**
     * @brief IRQn assigned to this interrupt handler
     * @note auto-assigned in Usart implementation
     */
    IRQn_Type interrupt_number;

    /**
     * @brief Interrupt preempt_priority to set for this interrupt handler
     */
    uint32_t preempt_priority;
    /**
     * @brief Interrupt sub_priority to set for this interrupt handler
     */
    uint32_t sub_priority;
};

/**
 * @brief USART receive error codes
 */
enum usart_receive_error_t
{
    /**
     * @brief no error
     */
    None_Error = 0,

    /**
     * @brief usart peripheral framing error
     * @note occurs, for example, when the stop bit of the received data is not detected
     */
    FramingError,

    /**
     * @brief usart peripheral parity error
     * @note occurs when the parity of the received data does not match the expected parity
     */
    ParityError,

    /**
     * @brief usart peripheral RX register overrun error
     * @note occurs when the RX data register is not read before the next data is received
     */
    OverrunError,

    /**
     * @brief RX ring buffer was full and a new byte was received
     * @note occurs when the RX ring buffer is full and a new byte is received
     * @note this error indicates that the oldest data in the RX buffer was lost
     */
    RxDataDropped,
};

#ifdef USART_RX_ERROR_COUNTERS_ENABLE
/**
 * @brief USART receive error counters
 */
struct usart_rx_error_counters_t
{
    /**
     * @brief counter for usart_receive_error_t::FramingError
     */
    uint32_t framing_error;

    /**
     * @brief counter for usart_receive_error_t::ParityError
     */
    uint32_t parity_error;

    /**
     * @brief counter for usart_receive_error_t::OverrunError
     */
    uint32_t overrun_error;

    /**
     * @brief counter for usart_receive_error_t::RxDataDropped
     */
    uint32_t rx_data_dropped;
};
#endif // USART_RX_ERROR_COUNTERS_ENABLE

/**
 * @brief USART runtime states
 */
struct usart_runtime_state_t
{
    // /**
    //  * @brief USART receive buffer
    //  * @note allocated and freed in Usart class (constructor and destructor methods)
    //  */
    // RingBuffer<uint8_t> *rx_buffer;

    // /**
    //  * @brief USART transmit buffer
    //  * @note allocated and freed in Usart class (constructor and destructor methods)
    //  */
    // RingBuffer<uint8_t> *tx_buffer;

    /**
     * @brief last error in RX error interrupt handler
     */
    usart_receive_error_t rx_error;

#ifdef USART_RX_ERROR_COUNTERS_ENABLE
    /**
     * @brief USART receive error counters
     */
    usart_rx_error_counters_t rx_error_counters;
#endif
};

#ifdef USART_RX_DMA_SUPPORT
#endif // USART_RX_DMA_SUPPORT
/**
 * @brief USART device configuration
 */
struct usart_config_t
{
    /**
     * @brief USART peripheral configuration
     */
    usart_peripheral_config_t peripheral;

    /**
     * @brief USART interrupts and handlers configuration
     */
    usart_interrupt_config_t interrupts;

    /**
     * @brief USART runtime states
     */
    usart_runtime_state_t state;

#ifdef USART_RX_DMA_SUPPORT
    /**
     * @brief USART DMA configuration
     */
    usart_dma_config_t dma;
#endif
};

#define USART1_ENABLE
#if defined(USART1_ENABLE)
extern usart_config_t USART1_config;
// #define USART1_IOMUX_ENABLE
#endif

#endif // USART_CONFIG_H