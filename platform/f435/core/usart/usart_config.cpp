#include "usart_config.h"
#include "gpio/gpio.h"

usart_config_t USART1_config = {
		.peripheral = {
			.register_base = USART1,
			.peripheral_clock_id = CRM_USART1_PERIPH_CLOCK,
			#if defined(USART1_IOMUX_ENABLE)
			//.gpio_clock_id = CRM_GPIOB_PERIPH_CLOCK,
			.tx_pin = PB6,
			.rx_pin = PB7,
			.clk_pin = PIN_MAX, // USART1 does not use CLK pin
			#else
			.gpio_clock_id = CRM_GPIOA_PERIPH_CLOCK,
			.tx_pin = PA9,
			.rx_pin = PA10,
			.clk_pin = PIN_MAX, // USART1 does not use CLK pin
			.mux_sel = GPIO_MUX_7
			#endif
		},
		.interrupts = {
			.interrupt_number = USART1_IRQn,
			.preempt_priority = 1,
			.sub_priority = 0,
		},
		.state = {
			// .rx_buffer = nullptr,
			// .tx_buffer = nullptr,
			.rx_error = None_Error,
		}
};