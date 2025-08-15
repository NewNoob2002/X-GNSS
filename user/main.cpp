/* add user code begin Header */
/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
/* add user code end Header */

/* Includes ------------------------------------------------------------------*/
#include "at32f435_437_wk_config.h"
#include "wk_system.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

typedef Adafruit_ST7789 SCREEN_CLASS;

static SCREEN_CLASS screen(
	PA15,
	PB4,
	PB6);
/* add user code end private includes */
/**
  * @brief main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  /* add user code begin 1 */

  /* add user code end 1 */

  /* system clock config. */
  wk_system_clock_config();
	
	Delay_Init();
  /* config periph clock. */
  wk_periph_clock_config();

  /* nvic config. */
  wk_nvic_config();

  /* add user code begin 2 */
	pinMode(PC0, OUTPUT);
//	pinMode(PB7, OUTPUT);
//	digitalWrite(PA2, HIGH);
	Serial.begin(115200);
	
	screen.init(172, 320);
	screen.setRotation(0);
	screen.setTextSize(2);
	screen.fillScreen(BLUE);
	screen.setCursor(10,10);
	screen.printf("Hello,World\n");
  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
		digitalToggle(PC0);
		Serial.printf("HelloWorld\n");
		screen.fillScreen(BLUE);
		screen.setCursor(10,30);
		screen.printf("Tick :%d\n", millis());
		delay_ms(1000);
    /* add user code end 3 */
  }
}

  /* add user code begin 4 */

  /* add user code end 4 */
