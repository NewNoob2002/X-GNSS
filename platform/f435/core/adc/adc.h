#ifndef ADC_H
#define ADC_H
#include "core_debug.h"
#include "adc_config.h"

#define ADC_REG_TO_X(reg) \
	reg == ADC1	  ? 1     \
	: reg == ADC2 ? 2     \
	: reg == ADC3 ? 3     \
				  : 0
#define ADC_DEBUG_PRINTF(fmt, ...) \
	CORE_DEBUG_PRINTF("[ADC%d] " fmt, ADC_REG_TO_X(this->adc_config->peripheral.register_base), ##__VA_ARGS__)
		
class ADC
{
public:
	ADC(adc_config_t *adc_config);
	~ADC();

	bool addOrdinaryChannel(uint16_t channel);

	void begin();

	void start_conversion(){
		adc_flag_clear(this->adc_config->peripheral.register_base, ADC_OCCE_FLAG);
		adc_ordinary_software_trigger_enable(this->adc_config->peripheral.register_base, TRUE);
	}
	
	void wait_conversion_completed(uint32_t timeout){
		while(adc_flag_get(this->adc_config->peripheral.register_base, ADC_OCCE_FLAG) != SET)
		{
			//
			if(timeout-- <= 10)
			{
				ADC_DEBUG_PRINTF("Wait Conversion Timeout\n");
				break;
			}
		}
	}
	
	uint16_t conversion_read_result()
	{
		uint16_t retval = 0;
		retval = adc_ordinary_conversion_data_get(this->adc_config->peripheral.register_base);
		return retval;
	}
	
	uint16_t read_sync()
	{
		if(this->enableDma)
		{
			return 0;
		}
		start_conversion();
		wait_conversion_completed(1000);
		return conversion_read_result();
	}
	void enableDMA(bool enable)
	{
		this->enableDma = enable;
	}
	void dmaIrq();
	/**
	 * @brief Get the value of an ADC channel
	 * @param channel ADC channel number
	 * @return ADC value
	 * @note The channel number should be less than ADC1_MAX_CHANNEL_COUNT
	 * @return 0 if the channel is not initialized
	 */
	uint16_t getOrdinaryValue(uint16_t channel) const
	{
		if (channel < ADC1_MAX_CHANNEL_COUNT && this->isInitilized[channel])
		{
			return this->adc_ordinary_valuetab[channel];
		}
		return 0;
	}

private:
	adc_config_t *adc_config;
	bool enableDma;
	bool clockInitialized;
	/**
	 * @brief ADC ordinary value table
	 * @note The size of this array is defined by ADC1_MAX_CHANNEL_COUNT
	 * This array holds the values read from the ADC channels.
	 * Each index corresponds to a channel number.
	 * @note The values are updated when the ADC is read.
	 * @note This array is initialized to zero in the constructor.
	 * 	*/
	__IO uint16_t adc_ordinary_valuetab[ADC1_MAX_CHANNEL_COUNT];
	__IO bool isInitilized[ADC1_MAX_CHANNEL_COUNT];

	uint16_t adc_ch_enable_count; // Count of enabled ADC channels

	uint8_t adc_ch_sequence;
};

extern ADC adc1;
#endif