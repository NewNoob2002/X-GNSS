#include "gpio.h"

void pinMode(gpio_pin_t gpio_pin, const int pin_mode, gpio_drive_type dirve_type)
{
  ASSERT_GPIO_PIN_VALID(gpio_pin, "pinMode");
	gpio_init_type gpio_init_struct;
	gpio_default_para_init(&gpio_init_struct);
	switch(pin_mode)
	{
		case INPUT:
			gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
			break;
		case INPUT_PULLUP:
			gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
			gpio_init_struct.gpio_pull = GPIO_PULL_UP;
			break;
		case INPUT_PULLDOWN:
			gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
			gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
			break;
		case INPUT_ANALOG:
		{
			gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
			const pin_adc_info_t adc_info = PIN_MAP[gpio_pin].adc_info;
			const uint8_t adc_device = adc_info.device;
			const uint8_t adc_channel = adc_info.channel;
			if(adc_device !=0 &&  ADC_CHANNEL_0 <= adc_channel && ADC_CHANNEL_17 >= adc_channel)
			{
				adc1.addOrdinaryChannel(adc_channel);
			}
		}
			break;
		case INPUT_ANALOG_DMA:
		{
			gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
			const pin_adc_info_t adc_info = PIN_MAP[gpio_pin].adc_info;
			const uint8_t adc_device = adc_info.device;
			const uint8_t adc_channel = adc_info.channel;
			if(adc_device !=0 &&  ADC_CHANNEL_0 <= adc_channel && ADC_CHANNEL_17 >= adc_channel)
			{
				adc1.addOrdinaryChannel(adc_channel);
			}
			adc1.enableDMA(true);
			break;
		}
		case OUTPUT:
			gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
			gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
			break;
		case OUTPUT_OPEN_DRAIN:
			gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
			gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
			break;
		case OUTPUT_AF_OD:
			gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
			gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
			break;
		case OUTPUT_AF_PP:
			gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
			break;
		case OUTPUT_AF_PP_DOWN:
			gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
			gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
			break;
		case PWM:
			break;
	}
	gpio_init_struct.gpio_drive_strength = dirve_type;
	GPIO_Init(gpio_pin, &gpio_init_struct);
}

void digitalWrite(gpio_pin_t gpio_pin, const int pin_state)
{
	ASSERT_GPIO_PIN_VALID(gpio_pin, "digitalWrite");
	if(pin_state == HIGH)
	{
		GPIO_SetPin(gpio_pin);
	}
	else
	{
		GPIO_ResetPin(gpio_pin);
	}
}

uint8_t digitalRead(gpio_pin_t gpio_pin)
{
	ASSERT_GPIO_PIN_VALID(gpio_pin, "digitalRead");
	return GPIO_ReadPin(gpio_pin);
}

void digitalToggle(gpio_pin_t gpio_pin)
{
	ASSERT_GPIO_PIN_VALID(gpio_pin, "digitalToggle");
	GPIO_TogglePin(gpio_pin);
}