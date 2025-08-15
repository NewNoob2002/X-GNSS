#ifndef __GPIO_H
#define __GPIO_H
#include "core_debug.h"
#include "mcu_types.h"
#include "wiring_constants.h"
#include "adc/adc.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum
	{
		PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15,
		PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15,
		PC0,PC1,PC2,PC3,PC4,PC5,PC6,PC7,PC8,PC9,PC10,PC11,PC12,PC13,PC14,PC15,
		PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7,PD8,PD9,PD10,PD11,PD12,PD13,PD14,PD15,
#ifdef GPIOE
		PE0,PE1,PE2,PE3,PE4,PE5,PE6,PE7,PE8,PE9,PE10,PE11,PE12,PE13,PE14,PE15,
#endif
#ifdef GPIOF
		PF0,PF1,PF2,PF3,PF4,PF5,PF6,PF7,PF8,PF9,PF10,PF11,PF12,PF13,PF14,PF15,
#endif
#ifdef GPIOG
		PG0,PG1,PG2,PG3,PG4,PG5,PG6,PG7,PG8,PG9,PG10,PG11,PG12,PG13,PG14,PG15,
#endif
		PIN_MAX
	} Pin_TypeDef;

	/**
	 * @brief adc info for a pin
	 */
	typedef struct pin_adc_info_t
	{
		/**
		 * @brief pointer to the ADC device of this pin, if any
		 * @note
		 * - 0 if not set / not a ADC pin
		 * - 1 = ADC1
		 * - 2 = ADC2
		 * - 3 = ADC3
		 */
		const uint8_t device : 2;

		/**
		 * @brief adc channel number of this pin, if any
		 * @note only valid if device != 0
		 */
		const uint8_t channel : 6;

#ifdef __cplusplus
		/**
		 * @brief get the pointer to the ADC device of this pin
		 * @return pointer to the ADC device of this pin, or NULL if no adc device is assigned
		 */
//		adc_device_t *get_device() const
//		{
//			switch (device)
//			{
//			case 1:
//				return &ADC1_device;
//			case 2:
//				// TODO: ADC2 is not yet supported
//				// return &ADC2_device;
//			default:
//				return NULL;
//			}
//		}
#endif
	} pin_adc_info_t;

	/**
	 * @brief TimerA info for a pin
	 */
	typedef struct pin_timer_info_t
	{
		/**
		 * @brief TimerA unit assigned to this pin for PWM or Servo output
		 *
		 * @note
		 * - 0: no timer assigned
		 * - 6-7: TIMER6, TIMER7 Basic Timer
		 * - 2-5: TIMER2, TIMER3, TIMER4, TIMER5 General timer
		 * - 9-14: TIMER9, TIMER10, TIMER11, TIMER12, TIMER13, TIMER14 General timer
		 * - 1, 8: TIMER1, TIMER8 Advanced Timer
		 */
		const uint8_t unit : 4;

		/**
		 * @brief TimerA channel assigned to this pin for PWM or Servo output
		 *
		 * @note
		 * - 0: null
		 * - 1: Ch1
		 * - 2: Ch2
		 * - 3: Ch3
		 * - 4: Ch4
		 * @note only valid if unit != 0
		 */
		const uint8_t channel : 4;

	} pin_timer_info_t;

	/**
	 * @brief variant pin map struct
	 */
	typedef struct pin_info_t
	{
		/**
		 * @brief bit position of the pin in the port
		 */
		const uint8_t pin_bit_pos : 8;

		/**
		 * @brief IO port this pin belongs to
		 */
		gpio_type *GPIOx;

#ifdef __cplusplus
		/**
		 * @brief bit mask of the pin in the port
		 */
		uint16_t bit_mask() const
		{
			return (1 << pin_bit_pos);
		}
		
		uint8_t bit_pos() const
		{
			return pin_bit_pos;
		}
#endif

		/**
		 * @brief adc configuration for this pin
		 */
		const pin_adc_info_t adc_info;

		/**
		 * @brief TimerA configuration for this pin
		 */
		const pin_timer_info_t timer_info;

	} pin_info_t;

	/**
	 * @brief GPIO pin map
	 */
extern const pin_info_t PIN_MAP[PIN_MAX];
extern const uint32_t analogInputPin[];

#define portOutputRegister(P)       (&(P->odt))
#define portInputRegister(P)        (&(P->idt))
	
#define digitalPinToPort(Pin)       (PIN_MAP[Pin].GPIOx)
#define digitalPinToBitMask(Pin)    (PIN_MAP[Pin].bit_mask())
#define digitalPinToBitPos(Pin)     (PIN_MAP[Pin].bit_pos())
/**
 * @brief test if a gpio pin number is valid
 */
#define IS_GPIO_PIN(Pin) (Pin >= 0 && Pin < PIN_MAX)
#define IS_ADC_PIN(Pin) (IS_PIN(Pin) && PIN_MAP[Pin].adc_info.device != 0 && PIN_MAP[Pin].adc_info.channel != 0)
#define IS_PWM_PIN(Pin) (IS_PIN(Pin) && PIN_MAP[Pin].timer_info.unit != 0 && PIN_MAP[Pin].timer_info.channel != 0)

#define ASSERT_GPIO_PIN_VALID(gpio_pin, fn_name, ...) \
    CORE_ASSERT(IS_GPIO_PIN(gpio_pin), "invalid GPIO pin supplied to " fn_name, ##__VA_ARGS__)
#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus // c++ special function

inline void GPIO_Init(gpio_pin_t gpio_pin, gpio_init_type *gpio_init_struct)
{
	ASSERT_GPIO_PIN_VALID(gpio_pin, "GPIO_Init");
	gpio_init_struct->gpio_pins = digitalPinToBitMask(gpio_pin);
	gpio_type *gpiox = digitalPinToPort(gpio_pin);
	if(gpiox == GPIOA)
	{
		crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
	}
	else if(gpiox == GPIOB)
	{
		crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
	}
	else if(gpiox == GPIOC)
	{
		crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
	}
	else if(gpiox == GPIOD)
	{
		crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
	}
	else if(gpiox == GPIOE)
	{
		crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);
	}
	gpio_init(gpiox, gpio_init_struct);
	//gpiox->clr = gpio_init_struct->gpio_pins;
}

inline uint8_t GPIO_ReadPin(gpio_pin_t gpio_pin)
{
	ASSERT_GPIO_PIN_VALID(gpio_pin, "GPIO_ReadPin");
	return digitalPinToPort(gpio_pin)->idt & digitalPinToBitMask(gpio_pin);
}

inline void GPIO_SetPin(gpio_pin_t gpio_pin)
{
	ASSERT_GPIO_PIN_VALID(gpio_pin, "GPIO_SetPin");
	digitalPinToPort(gpio_pin)->scr = digitalPinToBitMask(gpio_pin);
}

inline void GPIO_ResetPin(gpio_pin_t gpio_pin)
{
	ASSERT_GPIO_PIN_VALID(gpio_pin, "GPIO_ResetPin");
	digitalPinToPort(gpio_pin)->clr = digitalPinToBitMask(gpio_pin);
}

inline void GPIO_TogglePin(gpio_pin_t gpio_pin)
{
	ASSERT_GPIO_PIN_VALID(gpio_pin, "GPIO_TogglePin");
	digitalPinToPort(gpio_pin)->odt ^= digitalPinToBitMask(gpio_pin);
}

/**
 * \brief Configures the specified pin to behave either as an input or an output. See the description of digital pins for details.
 *
 * \param gpio_pin The number of the pin whose mode you wish to set
 * \param pin_mode Can be INPUT, OUTPUT, INPUT_PULLUP, INPUT_PULLDOWN, OUTPUT_PWM
 */
extern void pinMode(gpio_pin_t gpio_pin, const int pin_mode, gpio_drive_type dirve_type = GPIO_DRIVE_STRENGTH_STRONGER);
#endif //__cplusplus
/**
 * \brief Write a HIGH or a LOW value to a digital pin.
 *
 * If the pin has been configured as an OUTPUT with pinMode(), its voltage will be set to the
 * corresponding value: 5V (or 3.3V on 3.3V boards) for HIGH, 0V (ground) for LOW.
 *
 * If the pin is configured as an INPUT, writing a HIGH value with digitalWrite() will enable an internal
 * 20K pullup resistor (see the tutorial on digital pins). Writing LOW will disable the pullup. The pullup
 * resistor is enough to light an LED dimly, so if LEDs appear to work, but very dimly, this is a likely
 * cause. The remedy is to set the pin to an output with the pinMode() function.
 *
 * \note Digital pin PIN_LED is harder to use as a digital input than the other digital pins because it has an LED
 * and resistor attached to it that's soldered to the board on most boards. If you enable its internal 20k pull-up
 * resistor, it will hang at around 1.7 V instead of the expected 5V because the onboard LED and series resistor
 * pull the voltage level down, meaning it always returns LOW. If you must use pin PIN_LED as a digital input, use an
 * external pull down resistor.
 *
 * \param gpio_pin the pin number
 * \param pin_state HIGH or LOW
 */
extern void digitalWrite(gpio_pin_t gpio_pin, const int pin_state);
/**
 * \brief Reads the value from a specified digital pin, either HIGH or LOW.
 *
 * \param gpio_pin The number of the digital pin you want to read (int)
 *
 * \return HIGH or LOW
 */
extern uint8_t digitalRead(gpio_pin_t gpio_pin);
/**
 * \brief Toggle digital pin.
 *
 * \param gpio_pin The number of the digital pin you want to toggle
 */
extern void digitalToggle(gpio_pin_t gpio_pin);

#endif