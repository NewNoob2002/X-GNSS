/*
 Copyright (c) 2015 Arduino LLC.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "gpio/gpio.h"

//
// ADC helpers
//

/**
 * @brief ADC config struct shorthand
 * @param channel ADC channel number. 0 == ADC1_IN0, ...
 * @note device is always ADC1_device
 */
#define ADC(Adc_x, ch)                    \
	{                              \
		.device = Adc_x, .channel = ch \
	}

/**
 * @brief ADC config struct shorthand for no ADC function
 */
#define ADC_NONE                  \
	{                             \
		.device = 0, .channel = 0 \
	}

//
// TimerA helpers
//

/**
 * @brief TimerA config struct shorthand
 * @param uni TimerA unit. 1 == TIMER1, ...
 * @param ch TimerA channel. 1 == Ch1, ...
 */
#define TIM(uni, ch)                                      \
	{                                                            \
		.unit = uni, .channel = ch \
	}

/**
 * @brief TimerA config struct shorthand for no TimerA function
 */
#define TIM_NONE                              \
	{                                          \
		.unit = 0, .channel = 0 \
	}

//
// Pin Map
//

extern const pin_info_t PIN_MAP[PIN_MAX] = {
	// ---  PAx  ---
	{0,  GPIOA, ADC(1,0), TIM(2,1)},		// PA0
	{1,  GPIOA, ADC(1,1), TIM(2,2)},		// PA1
	{2,  GPIOA, ADC(1,2), TIM(2,3)},		// PA2
	{3,  GPIOA, ADC(1,3), TIM(2,4)},		// PA3
	{4,  GPIOA, ADC(1,4), TIM_NONE},		// PA4
	{5,  GPIOA, ADC(1,5), TIM_NONE},		// PA5
	{6,  GPIOA, ADC(1,6), TIM(3,1)},		// PA6
	{7,  GPIOA, ADC(1,7), TIM(3,2)},		// PA7
	{8,  GPIOA, ADC_NONE, TIM(1,1)},		// PA8
	{9,  GPIOA, ADC_NONE, TIM(1,2)},		// PA9
	{10, GPIOA, ADC_NONE, TIM(1,3)},		// PA10
	{11, GPIOA, ADC_NONE, TIM(1,4)},		// PA11
	{12, GPIOA, ADC_NONE, TIM_NONE},		// PA12
	{13, GPIOA, ADC_NONE, TIM_NONE},		// PA13
	{14, GPIOA, ADC_NONE, TIM_NONE},		// PA14
	{15, GPIOA, ADC_NONE, TIM(2,1)},		// PA15
												//
												// ---  PBx  ---
												//
	{0,  GPIOB, ADC(1,8), TIM(3,3)},		// PB0
	{1,  GPIOB, ADC(1,9), TIM(3,4)},		// PB1
	{2,  GPIOB, ADC_NONE, TIM_NONE},		// PB2
	{3,  GPIOB, ADC_NONE, TIM(2,2)},		// PB3
	{4,  GPIOB, ADC_NONE, TIM(3,1)},		// PB4
	{5,  GPIOB, ADC_NONE, TIM(3,2)},		// PB5
	{6,  GPIOB, ADC_NONE, TIM(4,1)},		// PB6
	{7,  GPIOB, ADC_NONE, TIM(4,2)},		// PB7
	{8,  GPIOB, ADC_NONE, TIM(4,3)},		// PB8
	{9,  GPIOB, ADC_NONE, TIM(4,4)},		// PB9
	{10, GPIOB, ADC_NONE, TIM(2,3)},		// PB10
	{11, GPIOB, ADC_NONE, TIM(2,4)},		// PB11
	{12, GPIOB, ADC_NONE, TIM_NONE},		// PB12
	{13, GPIOB, ADC_NONE, TIM_NONE},		// PB13
#ifdef TMR12
	{14, GPIOB, ADC_NONE, TIM(12,1)},		// PB14
#else
	{14, GPIOB, ADC_NONE, TIM_NONE},		// PB14
#endif
	{15, GPIOB, ADC_NONE, TIM_NONE},		// PB15
												//
												// ---  PCx  ---
												//
	{0,  GPIOC, ADC(1,10), TIM_NONE}, // PC0
	{1,  GPIOC, ADC(1,11), TIM_NONE}, // PC1
	{2,  GPIOC, ADC(1,12), TIM_NONE},	// PC2
	{3,  GPIOC, ADC(1,13), TIM_NONE},	// PC3
	{4,  GPIOC, ADC(1,14), TIM_NONE},	// PC4
	{5,  GPIOC, ADC(1,15), TIM_NONE},	// PC5
	{6,  GPIOC, ADC_NONE,  TIM(3,1)},	// PC6
	{7,  GPIOC, ADC_NONE,  TIM(3,2)},	// PC7
	{8,  GPIOC, ADC_NONE,  TIM(3,3)},	// PC8
	{9,  GPIOC, ADC_NONE,  TIM(3,4)},	// PC9
	{10, GPIOC, ADC_NONE,  TIM_NONE},	// PC10
	{11, GPIOC, ADC_NONE,  TIM_NONE},	// PC11
	{12, GPIOC, ADC_NONE,  TIM_NONE},	// PC12
	{13, GPIOC, ADC_NONE,  TIM_NONE},	// PC13
	{14, GPIOC, ADC_NONE,  TIM_NONE},	// PC14 XTAL32_OUT
	{15, GPIOC, ADC_NONE,  TIM_NONE},	// PC15 XTAL32_IN
												//
												// ---  PDx  ---
												//
	{0,  GPIOD, ADC_NONE, TIM_NONE},		// PD0
	{1,  GPIOD, ADC_NONE, TIM_NONE},		// PD1
	{2,  GPIOD, ADC_NONE, TIM_NONE},		// PD2
	{3,  GPIOD, ADC_NONE, TIM_NONE},		// PD3
	{4,  GPIOD, ADC_NONE, TIM_NONE},		// PD4
	{5,  GPIOD, ADC_NONE, TIM_NONE},		// PD5
	{6,  GPIOD, ADC_NONE, TIM_NONE},		// PD6
	{7,  GPIOD, ADC_NONE, TIM_NONE},		// PD7
	{8,  GPIOD, ADC_NONE, TIM_NONE},		// PD8
	{9,  GPIOD, ADC_NONE, TIM_NONE},		// PD9
	{10, GPIOD, ADC_NONE, TIM_NONE},		// PD10
	{11, GPIOD, ADC_NONE, TIM_NONE},		// PD11
	{12, GPIOD, ADC_NONE, TIM(4,1)},		// PD12
	{13, GPIOD, ADC_NONE, TIM(4,2)},		// PD13
	{14, GPIOD, ADC_NONE, TIM(4,3)},		// PD14
	{15, GPIOD, ADC_NONE, TIM(4,4)},		// PD15
												//
												// ---  PEx  ---
												//
	#ifdef GPIOE
	{0,  GPIOE, ADC_NONE, TIM_NONE},		// PE0
	{1,  GPIOE, ADC_NONE, TIM_NONE},		// PE1
	{2,  GPIOE, ADC_NONE, TIM_NONE},		// PE2
	{3,  GPIOE, ADC_NONE, TIM_NONE},		// PE3
	{4,  GPIOE, ADC_NONE, TIM_NONE},		// PE4
	{5,  GPIOE, ADC_NONE, TIM(9,1)},		// PE5
	{6,  GPIOE, ADC_NONE, TIM(9,2)},		// PE6
	{7,  GPIOE, ADC_NONE, TIM_NONE},		// PE7
	{8,  GPIOE, ADC_NONE, TIM_NONE},		// PE8
	{9,  GPIOE, ADC_NONE, TIM_NONE},		// PE9
	{10, GPIOE, ADC_NONE, TIM_NONE},		// PE10
	{11, GPIOE, ADC_NONE, TIM_NONE},		// PE11
	{12, GPIOE, ADC_NONE, TIM_NONE},		// PE12
	{13, GPIOE, ADC_NONE, TIM_NONE},		// PE13
	{14, GPIOE, ADC_NONE, TIM_NONE},		// PE14
	{15, GPIOE, ADC_NONE, TIM_NONE},		// PE15
	#endif
};
