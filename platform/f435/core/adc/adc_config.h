#ifndef ADC_CONFIG_H
#define ADC_CONFIG_H

#include "mcu_types.h"

#define ADC1_MAX_CHANNEL_COUNT (ADC_CHANNEL_17 + 1)
/**
 * @brief ADC peripheral configuration
 */
typedef struct adc_peripheral_config_t
{
    /**
     * @brief ADC peripheral register base address
     */
    adc_type *register_base;

    /**
     * @brief ADC peripheral clock id
     * @note in FCG3
     */
    crm_periph_clock_type clock_id;
		/**
     * @brief ADC peripheral clock div
     * @note in FCG3
     */
		adc_div_type clk_div;

    /**
     * @brief ADC channel count
     */
    uint16_t channel_count;
} adc_peripheral_config_t;

/**
 * @brief ADC init parameters
 */
typedef struct adc_init_params_t
{
	   /**
     * @brief ADC repeat
     */
		confirm_state repeat;
    /**
     * @brief ADC sample data alignment
     */
    adc_data_align_type data_alignment;

} adc_init_params_t;

/**
 * @brief ADC init parameters
 */
typedef struct adc_dma_config_t
{
	/**
  * @brief ADC dma ch
  * @note channel
  */
	dma_channel_type *dma_ch;
	
	crm_periph_clock_type dma_clk;
	
	IRQn_Type dma_irq;
	
	confirm_state loop_mode_enable;

} adc_dma_config_t;
/**
 * @brief ADC device configuration
 */
typedef struct adc_config_t
{
    /**
     * @brief ADC peripheral configuration
     */
    adc_peripheral_config_t peripheral;

    /**
     * @brief ADC init parameters
     */
    adc_init_params_t init_params;
	   /**
     * @brief ADC dma config
     */
		adc_dma_config_t dma_config;
} adc_config_t;


//
// ADC devices
//
extern adc_config_t ADC1_config;
#endif