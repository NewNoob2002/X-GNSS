#include "adc.h"

ADC adc1(&ADC1_config);

ADC::ADC(adc_config_t *adc_config)
{
	this->adc_config = adc_config;
	this->enableDma = false;
	this->clockInitialized = false;
	memset((void *)this->adc_ordinary_valuetab, 0, ADC1_MAX_CHANNEL_COUNT);
	memset((void *)this->isInitilized, 0, ADC1_MAX_CHANNEL_COUNT);
	this->adc_ch_enable_count = 0;
	this->adc_ch_sequence = 1;
}

bool ADC::addOrdinaryChannel(uint16_t channel)
{
	if (channel < ADC1_MAX_CHANNEL_COUNT && !this->isInitilized[this->adc_ch_enable_count])
	{
		this->isInitilized[this->adc_ch_enable_count] = true;
		this->adc_ch_enable_count++;
		return true;
	}
	return false;
}

void ADC::begin()
{
//	if(adc_ch_enable_count == 0)
//	{
//		ADC_DEBUG_PRINTF("channel enable is 0, please add channel before call begin()\n");
//		return;
//	}
//	adc_base_config_type adc_base_struct;
//	
//	if (!this->clockInitialized)
//	{
//		crm_periph_clock_enable(this->adc_config->peripheral.clock_id, TRUE);
//		crm_adc_clock_div_set(this->adc_config->peripheral.clk_div);
//		this->clockInitialized = true;
//	}
//	adc_combine_mode_select(ADC_INDEPENDENT_MODE);
//	adc_base_default_para_init(&adc_base_struct);
//	adc_base_struct.repeat_mode = this->adc_config->init_params.repeat;
//	if(this->enableDma == true)
//	{
//		adc_base_struct.repeat_mode = TRUE;
//	}
//	adc_base_struct.data_align = this->adc_config->init_params.data_alignment;
//	if (this->adc_ch_enable_count == 1)
//	{
//		ADC_DEBUG_PRINTF("single ordinary channel\n");
//		adc_base_struct.sequence_mode = FALSE;
//	}
//	else
//	{
//		ADC_DEBUG_PRINTF("multiply sequence channel\n");
//		adc_base_struct.sequence_mode = TRUE;
//	}
//	adc_base_struct.ordinary_channel_length = this->adc_ch_enable_count;
//	adc_base_config(this->adc_config->peripheral.register_base, &adc_base_struct);

//	for (uint8_t ch = 0; ch < ADC1_MAX_CHANNEL_COUNT; ch++)
//	{
//		if (this->isInitilized[ch] == true)
//		{
//			ADC_DEBUG_PRINTF("enable channel%d\n", ch);
//			adc_ordinary_channel_set(this->adc_config->peripheral.register_base, (adc_channel_select_type)ch, this->adc_ch_sequence++, ADC_SAMPLETIME_239_5);
//		}
//	}
//	adc_ordinary_conversion_trigger_set(this->adc_config->peripheral.register_base, ADC12_ORDINARY_TRIG_SOFTWARE, TRUE);

//	if (this->enableDma)
//	{	
//		ADC_DEBUG_PRINTF("dma enale\n");
//		dma_init_type dma_init_struct;
//		crm_periph_clock_enable(this->adc_config->dma_config.dma_clk, TRUE);
//		nvic_irq_enable(this->adc_config->dma_config.dma_irq, 0, 0);
//		dma_reset(this->adc_config->dma_config.dma_ch);
//		dma_default_para_init(&dma_init_struct);
//		dma_init_struct.buffer_size = this->adc_ch_enable_count;
//		dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
//		dma_init_struct.memory_base_addr = (uint32_t)this->adc_ordinary_valuetab;
//		dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
//		dma_init_struct.memory_inc_enable = TRUE;
//		dma_init_struct.peripheral_base_addr = (uint32_t)&(this->adc_config->peripheral.register_base->odt);
//		dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
//		dma_init_struct.peripheral_inc_enable = FALSE;
//		dma_init_struct.priority = DMA_PRIORITY_HIGH;
//		dma_init_struct.loop_mode_enable = TRUE;
//		dma_init(this->adc_config->dma_config.dma_ch, &dma_init_struct);

//		dma_interrupt_enable(this->adc_config->dma_config.dma_ch, DMA_FDT_INT, TRUE);
//		dma_channel_enable(this->adc_config->dma_config.dma_ch, TRUE);
//		
//		adc_dma_mode_enable(this->adc_config->peripheral.register_base, TRUE);
//	}

//	adc_enable(ADC1, TRUE);
//	adc_calibration_init(ADC1);
//	while (adc_calibration_init_status_get(ADC1));
//	adc_calibration_start(ADC1);
//	while (adc_calibration_status_get(ADC1));
//	//if enable DMA enable trigger in begin(), or not enable trigger in user code;
//	if (this->enableDma)
//	{	
//		adc_ordinary_software_trigger_enable(this->adc_config->peripheral.register_base, TRUE);
//	}
}

void ADC::dmaIrq()
{
	if(dma_interrupt_flag_get(DMA1_FDT1_FLAG) != RESET)
  {
    dma_flag_clear(DMA1_FDT1_FLAG);
  }
}

ADC::~ADC()
{
	// Destructor logic if needed
	// Currently, nothing to clean up as we are using simple types
	// and no dynamic memory allocation is done in this class.
	memset((void *)this->adc_ordinary_valuetab, 0, ADC1_MAX_CHANNEL_COUNT);
	memset((void *)this->isInitilized, 0, ADC1_MAX_CHANNEL_COUNT);
}


//extern "C" void DMA1_Channel1_IRQHandler(void)
//{
//	adc1.dmaIrq();
//}