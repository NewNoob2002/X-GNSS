#include "core_hooks.h"
#include "gpio/gpio.h"
#include "usart_config.h"
#include "usart.h"

//
// debug print helpers
//
#define USART_REG_TO_X(reg) \
    reg == USART1   ? 1     \
    : reg == USART2 ? 2     \
    : reg == USART3 ? 3     \
    : reg == UART4  ? 4     \
    : reg == UART5  ? 5     \
    : reg == USART6 ? 6     \
    : reg == UART7  ? 7     \
                    : 0
#define USART_DEBUG_PRINTF(fmt, ...) \
    CORE_DEBUG_PRINTF("[USART%d] " fmt, USART_REG_TO_X(this->_usart_config->peripheral.register_base), ##__VA_ARGS__)

//
// IRQ register / unregister helper
//
inline void usart_irq_register(usart_interrupt_config_t &irq, const char *name)
{
    // register and enable irq
    uint32_t temp_priority = 0;

    /* encode priority */
    temp_priority = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), irq.preempt_priority, irq.sub_priority);
    NVIC_SetPriority(irq.interrupt_number, temp_priority);
    NVIC_EnableIRQ(irq.interrupt_number);
    //CORE_DEBUG_PRINTF("Registered IRQ %s with preempt_priority %d, sub_priority %d\n", name, irq.preempt_priority, irq.sub_priority);
}

inline void usart_irq_resign(usart_interrupt_config_t &irq, const char *name)
{
    // disable interrupt and clear pending
    NVIC_DisableIRQ(irq.interrupt_number);
    NVIC_ClearPendingIRQ(irq.interrupt_number);
    //CORE_DEBUG_PRINTF("Unregistered IRQ %s\n", name);
}

Usart ::Usart(usart_config_t *usart_config, size_t rx_buffer_size, size_t tx_buffer_size)
{
    this->_usart_config = usart_config;
    this->tx_pin = usart_config->peripheral.tx_pin;
    this->rx_pin = usart_config->peripheral.rx_pin;
    this->rx_buffer = nullptr;
    this->tx_buffer = nullptr;
    this->rx_buffer_size = rx_buffer_size;
    this->tx_buffer_size = tx_buffer_size;

    this->initialized = false;
}
Usart ::~Usart()
{
    // deinit buffers
    if (this->rx_buffer != nullptr)
    {
        delete this->rx_buffer;
        this->rx_buffer = nullptr;
    }
    if (this->tx_buffer != nullptr)
    {
        delete this->tx_buffer;
        this->tx_buffer = nullptr;
    }
    // deinit usart
    if (this->initialized)
    {
        this->end();
    }
}

/**
 * @brief  串口中断回调
 * @param  Function: 回调函数
 * @retval 无
 */
void Usart::attachInterrupt(CallbackFunction_t func)
{
    _callbackFunction = func;
}

void Usart::begin(uint32_t baud)
{
    // if already initialized, ignore
    if (this->initialized)
    {
        USART_DEBUG_PRINTF("Already initialized, ignoring begin(%d)\n", baud);
        return;
    }
    // create rx / tx buffers
    this->rx_buffer = new RingBuffer<uint8_t>(this->rx_buffer_size);
    CORE_ASSERT(this->rx_buffer != nullptr, return;);
    this->tx_buffer = new RingBuffer<uint8_t>(this->tx_buffer_size);
    CORE_ASSERT(this->tx_buffer != nullptr, return;);

    begin(baud, SERIAL_8N1);
}
void Usart::begin(uint32_t baud, uint16_t config, bool irqn_enable)
{
    // if already initialized, ignore
    if (this->initialized)
    {
        USART_DEBUG_PRINTF("Already initialized, ignoring begin(%d, 0x%04X)\n", baud, config);
        return;
    }
    this->irqn_enable = irqn_enable;

    crm_periph_clock_enable(this->_usart_config->peripheral.peripheral_clock_id, TRUE);
    crm_periph_clock_enable(this->_usart_config->peripheral.gpio_clock_id, TRUE);
		gpio_pin_mux_config(digitalPinToPort(this->tx_pin), 
											 (gpio_pins_source_type)digitalPinToBitPos(this->tx_pin), 
												this->_usart_config->peripheral.mux_sel);
		gpio_pin_mux_config(digitalPinToPort(this->rx_pin), 
											 (gpio_pins_source_type)digitalPinToBitPos(this->rx_pin), 
												this->_usart_config->peripheral.mux_sel);
    pinMode(this->tx_pin, OUTPUT_AF_PP);
    pinMode(this->rx_pin, OUTPUT_AF_PP);

    usart_stop_bit_num_type stop_bits = USART_STOP_1_BIT;

    // stop bits
    switch (config & HARDSER_STOP_BIT_MASK)
    {
    default:
        CORE_ASSERT_FAIL("USART: invalid stop bit configuration");
    case HARDSER_STOP_BIT_1:
        stop_bits = USART_STOP_1_BIT;
        break;
    case HARDSER_STOP_BIT_2:
        stop_bits = USART_STOP_2_BIT;
        break;
    }

    // parity
    switch (config & HARDSER_PARITY_MASK)
    {
    default:
        CORE_ASSERT_FAIL("USART: invalid parity configuration");
    case HARDSER_PARITY_NONE:
        usart_parity_selection_config(this->_usart_config->peripheral.register_base, USART_PARITY_NONE);
        break;
    case HARDSER_PARITY_EVEN:
        usart_parity_selection_config(this->_usart_config->peripheral.register_base, USART_PARITY_EVEN);
        break;
    case HARDSER_PARITY_ODD:
        usart_parity_selection_config(this->_usart_config->peripheral.register_base, USART_PARITY_ODD);
        break;
    }

    usart_init(this->_usart_config->peripheral.register_base, baud, USART_DATA_8BITS, stop_bits);
    usart_transmitter_enable(this->_usart_config->peripheral.register_base, TRUE);
    usart_receiver_enable(this->_usart_config->peripheral.register_base, TRUE);
		
		usart_hardware_flow_control_set(this->_usart_config->peripheral.register_base, USART_HARDWARE_FLOW_NONE);
    // register IRQ
    if (this->irqn_enable)
    {
//        char irq_name[16];
//        snprintf(irq_name, sizeof(irq_name), "USART%d IRQ", USART_REG_TO_X(this->_usart_config->peripheral.register_base));
        usart_irq_register(this->_usart_config->interrupts, nullptr);
        // Enable RX buffer full interrupt
        usart_interrupt_enable(this->_usart_config->peripheral.register_base, USART_RDBF_INT, TRUE);
        // Enable TX buffer empty interrupt not call here, called in write()

#if defined(USART_RX_ERROR_COUNTERS_ENABLE)
        // Enable RX error interrupts
        usart_interrupt_enable(this->_usart_config->peripheral.register_base, USART_PERR_INT, TRUE);
        usart_interrupt_enable(this->_usart_config->peripheral.register_base, USART_ERR_INT, TRUE);
#endif
    }
		// Enable USART
    usart_enable(this->_usart_config->peripheral.register_base, TRUE);
    this->initialized = true;
}
void Usart::end()
{
    // if not initialized, ignore
    if (!this->initialized)
    {
        return;
    }
    // disable usart
    usart_enable(this->_usart_config->peripheral.register_base, FALSE);
    // disable interrupts
    if (this->irqn_enable)
    {
        usart_irq_resign(this->_usart_config->interrupts, "USART IRQ");
        usart_interrupt_enable(this->_usart_config->peripheral.register_base, USART_RDBF_INT, FALSE);
        usart_interrupt_enable(this->_usart_config->peripheral.register_base, USART_TDBE_INT, FALSE);
#if defined(USART_RX_ERROR_COUNTERS_ENABLE)
        usart_interrupt_enable(this->_usart_config->peripheral.register_base, USART_PERR_INT, FALSE);
        usart_interrupt_enable(this->_usart_config->peripheral.register_base, USART_ERR_INT, FALSE);
#endif
    }
}
int Usart::available(void)
{
    return this->rx_buffer->count();
}

int Usart::availableForWrite(void)
{
    return this->tx_buffer->capacity() - this->tx_buffer->count();
    //return 0;
}

int Usart::peek(void)
{
    return this->rx_buffer->peek();
}

int Usart::read(void)
{
    uint8_t ch;
    if (this->rx_buffer->pop(ch))
    {
        return ch;
    }
    else
    {
        return -1;
    }
}

void Usart::flush(void)
{
    // ignore if not initialized
    if (!this->initialized)
    {
        return;
    }

    // wait for tx buffer to empty
    while (!this->tx_buffer->isEmpty())
    {
        //yield();
    }
}

size_t Usart::write(uint8_t ch)
{
    // if uninitialized, ignore write
       if (!this->initialized)
       {
           return 0;
       }

       // wait until tx buffer is no longer full
       while (this->tx_buffer->isFull())
       {
           //yield();
       }

       // add to tx buffer
       while (!this->tx_buffer->push(ch))
       {
           //yield();
       }

//    // enable tx + empty interrupt
//    //usart_transmitter_enable(this->_usart_config->peripheral.register_base, TRUE);
    usart_interrupt_enable(this->_usart_config->peripheral.register_base, USART_TDBE_INT, TRUE);
//		while(usart_flag_get(this->_usart_config->peripheral.register_base, USART_TDBE_FLAG) == RESET);
//		usart_data_transmit(this->_usart_config->peripheral.register_base, ch);
    // wrote one byte
    return 1;
}

void Usart::irqHandler()
{
    // check if the RX Data Buffer Full Register is full
    if (usart_interrupt_flag_get(this->_usart_config->peripheral.register_base, USART_RDBF_FLAG) != RESET)
    {
        /* read one byte from the receive data register */
        uint8_t ch = usart_data_receive(this->_usart_config->peripheral.register_base);
        core_hook_usart_rx_irq(ch, USART_REG_TO_X(this->_usart_config->peripheral.register_base));
        bool rxOverrun;
        this->rx_buffer->push(ch, /*force*/ true, rxOverrun);
        // if the buffer was overrun, set the overrun error flag
        if (rxOverrun)
        {
            this->_usart_config->state.rx_error = usart_receive_error_t::RxDataDropped;

#ifdef USART_RX_ERROR_COUNTERS_ENABLE
            usartx->state.rx_error_counters.rx_data_dropped++;
#endif
        }
				usart_flag_clear(this->_usart_config->peripheral.register_base, USART_RDBF_FLAG);
    }
    // check if the TX Empty Register is empty
    if(usart_interrupt_flag_get(this->_usart_config->peripheral.register_base, USART_TDBE_FLAG) != RESET)
    {
        // check if there is data in the tx buffer
        uint8_t ch;
        if (this->tx_buffer->pop(ch))
        {
            core_hook_usart_tx_irq(ch, USART_REG_TO_X(this->_usart_config->peripheral.register_base));
            usart_data_transmit(this->_usart_config->peripheral.register_base, ch);
        }
        else
        {
            // no data in tx buffer, disable TX empty interrupt
            usart_interrupt_enable(this->_usart_config->peripheral.register_base, USART_TDBE_INT, FALSE);
            usart_interrupt_enable(this->_usart_config->peripheral.register_base, USART_TDC_INT, TRUE);
        }
				
				usart_flag_clear(this->_usart_config->peripheral.register_base, USART_TDBE_FLAG);
    }
    // check if the TX Data Complete Register is set
    // this is used to indicate that the last byte was sent
    // and the TX buffer is now empty
    if(usart_interrupt_flag_get(this->_usart_config->peripheral.register_base, USART_TDC_FLAG) != RESET)
    {
        // disable TX Data Complete interrupt
        usart_interrupt_enable(this->_usart_config->peripheral.register_base, USART_TDC_INT, FALSE);
        //usart_transmitter_enable(this->_usart_config->peripheral.register_base, FALSE);
			  usart_flag_clear(this->_usart_config->peripheral.register_base, USART_TDC_FLAG);
    }

    // call the callback function if set
    if (_callbackFunction)
    {
        _callbackFunction(this);
    }
}

#ifdef USART1_ENABLE
Usart Serial1(&USART1_config, SERIAL_RX_BUFFER_SIZE, SERIAL_TX_BUFFER_SIZE);

extern "C" void USART1_IRQHandler(void)
{
    Serial1.irqHandler();
}

extern "C" void io_putc(int ch)
{
	Serial1.write(ch);
}
#endif