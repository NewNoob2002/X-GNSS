#ifndef __MCU_TYPE_H
#define __MCU_TYPE_H
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "at32f435_437.h"

#define __STM32__

#define F_CPU                       SystemCoreClock
#define CYCLES_PER_MICROSECOND      (F_CPU / 1000000U)

typedef uint16_t gpio_pin_t;
#endif