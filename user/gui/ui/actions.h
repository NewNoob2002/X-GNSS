#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>
#include "screens.h"
#include "mcu_define.h"
#ifdef __cplusplus
extern "C" {
#endif

extern void action_switch_to_work_config(lv_event_t * e);
extern void action_switch_to_radio_config(lv_event_t * e);
	
extern void action_switch_to_static_settings(lv_event_t * e);
extern void action_turn_onoff_static(lv_event_t *e);
	
extern void action_switch_to_network_settings(lv_event_t * e);
extern void action_turn_onoff_4g(lv_event_t *e);
	
extern void action_switch_to_main(lv_event_t * e);
	
extern void action_radio_roller(lv_event_t *e) ;
extern void action_radio_protocol_roller(lv_event_t *e);
extern void action_radio_close(lv_event_t *e);

extern void lv_update_battery(pBatteryState p_batteryState, status_bar *bar);
extern void Page_Main_Update();


extern void Page_RadioConfig_Update();

extern void lv_focus_init(lv_obj_t *screen);
#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/