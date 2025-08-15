#include "adc_config.h"

adc_config_t ADC1_config = 
{
	.peripheral = {
		.register_base = ADC1,
		.clock_id = CRM_ADC1_PERIPH_CLOCK,
		.clk_div = ADC_HCLK_DIV_4,
		.channel_count = ADC_CHANNEL_17,
	},
	.init_params = {
		.repeat = FALSE,
		.data_alignment = ADC_RIGHT_ALIGNMENT,
	},
	.dma_config = {
		.dma_ch = DMA1_CHANNEL1,
		.dma_clk = CRM_DMA1_PERIPH_CLOCK,
		.dma_irq = DMA1_Channel1_IRQn,
		.loop_mode_enable = TRUE,
	}
};