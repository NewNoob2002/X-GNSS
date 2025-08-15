/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2014 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Arduino_h
#define Arduino_h
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "binary.h"
#include "avr/pgmspace.h"
//wiring
#ifdef __cplusplus
#include "adc/adc.h"
#include "gpio/gpio.h"
#include "usart/usart.h"
#endif

#include "wiring_constants.h"
#include "delay.h"

#define delay(ms)                    delay_ms(ms)
#define delayMicroseconds(us)        delay_us(us)

#ifdef __cplusplus
#include "SPI.h"
#endif

#endif
