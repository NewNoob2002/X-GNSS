/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "HAL.h"
#include "at32f435_437_wk_config.h"
#include "wk_system.h"
//#include "lv_port.h"
//#include "ui/ui.h"


SystemInfo_t systemInfo;
BatteryState batteryState;
/* add user code end private includes */
/**
  * @brief main function.
  * @param  none
  * @retval none
  */
int main(void)
{
	/* system clock config. */
  system_clock_init();
	
	Delay_Init();

  /* nvic config. */
  nvic_config();
	
	Serial.begin(115200);
	pinMode(PC0, OUTPUT);
	HAL::Display_Init();
//	lv_init();
//	lv_port_init();
//  /* add user code end 2 */
//	ui_init();
  while(1)
  {
		digitalToggle(PC0);
		delay_ms(100);
  }
}
