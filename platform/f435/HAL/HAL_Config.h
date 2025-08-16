#ifndef __HAL_CONFIG_H
#define __HAL_CONFIG_H
/*=========================
   Hardware Configuration
 *=========================*/
 /* Screen */
#define CONFIG_SCREEN_CS_PIN        PA15
#define CONFIG_SCREEN_DC_PIN        PB4
#define CONFIG_SCREEN_RST_PIN       PB6
//#define CONFIG_SCREEN_SCK_PIN       PA5
//#define CONFIG_SCREEN_MOSI_PIN      PA7
#define CONFIG_SCREEN_BLK_PIN       PB7  // TIM3
#define CONFIG_SCREEN_SPI           SPI

#define CONFIG_SCREEN_WIDTH  				172
#define CONFIG_SCREEN_HIEGHT				320

#endif //__HAL_CONFIG_H