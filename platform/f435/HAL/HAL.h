#ifndef __HAL_H
#define __HAL_H

#include <stdint.h>
#include "mcu_types.h"
#include "Arduino.h"
#include "HAL_Config.h"


namespace HAL{
	
void HAL_Init();
void HAL_Update();
	
/* Backlight */
void Backlight_Init();
uint16_t Backlight_GetValue();
void Backlight_SetValue(int16_t val);
void Backlight_SetGradual(uint16_t target, uint16_t time = 500);
void Backlight_ForceLit(bool en);

/* Display */
void Display_Init();
void Display_DumpCrashInfo(const char* info);
void Display_SetAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
void Display_SendPixels(const uint16_t* pixels, uint32_t len);
    
typedef void(*Display_CallbackFunc_t)(void);
void Display_SetSendFinishCallback(Display_CallbackFunc_t func);
}

#endif //__HAL_H