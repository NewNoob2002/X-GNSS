#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>
// #include "lv_obj_ext_func.h"
// #include "lv_anim_timeline_wrapper.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/*Main Page*/
	typedef struct status_bar
	{
		/* main_status_bar */
		lv_obj_t *icon_4g;
		lv_obj_t *icon_bluetooth;
		lv_obj_t *icon_wifi;
		lv_obj_t *icon_position;
		lv_obj_t *label_position_t;
		lv_obj_t *icon_radio_status;
		lv_obj_t *icon_sd;

		struct
		{
			lv_obj_t *icon_battery;
			lv_obj_t *label_useage;
			lv_style_t style_battery;

			lv_obj_t *label_text;
		} battery;

	} status_bar;

	typedef struct status_bottom
	{
		struct satellite
		{
			lv_obj_t *icon_satellite_big;
			lv_obj_t *label_sat_num;
			lv_obj_t *label_sat_num_real;
		} satellite;

		struct workmode
		{
			lv_obj_t *button_settings;
			lv_obj_t *icon_settings;
		} workmode;

		struct radio_workmode
		{
			lv_obj_t *button_radio_workmode;
			lv_obj_t *icon_radio_workmode;
		} radio_workmode;

	} status_bottom;

	typedef struct page_main
	{
		lv_obj_t *status_bar_appWindow;
		lv_obj_t *status_bottom_appWindow;
		status_bar status_bar_obj;
		status_bottom status_bottom_obj;

	} page_main_t;

	/*workConfig Page*/

	typedef struct page_workconfig
	{
		lv_obj_t *Button_workmodeSet;
		lv_obj_t *Button_workmodeClose;
		lv_obj_t *button_return;

		lv_obj_t *lable_radio_status;

		lv_obj_t *roller_radioFreq;
		lv_obj_t *dropdown_radioProtocol;

		lv_obj_t *button_cofirm;
		lv_obj_t *button_close;

	} page_workconfig_t;

	typedef struct page_static_settings
	{
		lv_obj_t *button_static_back;

		lv_obj_t *label_static;
		lv_obj_t *switch_static_onoff;
		lv_obj_t *icon_static_name;
		lv_obj_t *label_static_name;
		lv_obj_t *label_static_type;
		lv_obj_t *label_static_time;
		lv_obj_t *icon_sd;
		lv_obj_t *label_sd;

	} page_static_settings_t;

	typedef struct page_network_settings
	{
		lv_obj_t *button_network_back;

		lv_obj_t *icon_4g;
		lv_obj_t *label_4g;
		lv_obj_t *switch_4g_onoff;

	} page_network_settings_t;

	typedef struct _objects_t
	{
		/*pages*/
		lv_obj_t *main;
		lv_obj_t *work_config;
		lv_obj_t *radio_config;
		lv_obj_t *static_settings;
		lv_obj_t *network_settings;
		lv_obj_t *power_off;

		/*pages_objects*/
		page_main_t page_main;

		page_workconfig_t page_work;

		page_static_settings_t page_static;
		page_network_settings_t page_network;
	} objects_t;

	extern objects_t objects;

	enum ScreensEnum
	{
		SCREEN_ID_MAIN = 1,
		SCREEN_ID_WORK_CONFIG = 2,
		SCREEN_ID_RADIO_CONFIG = 3,
		SCREEN_ID_STATIC_SETTINGS = 4,
		SCREEN_ID_NETWORK_SETTINGS = 5,
		SCREEN_ID_POWER_OFF = 6,
	};

	void create_screen_main();
	void tick_screen_main();

	void create_screen_work_config();
	void tick_screen_work_config();

	void create_screen_radio_config();
	void tick_screen_radio_config();

	void create_screen_static_settings();
	void tick_screen_static_settings();

	void create_screen_network_settings();
	void tick_screen_network_settings();

	void create_screen_poweroff();
	void tick_screen_poweroff();

	void tick_screen_by_id(enum ScreensEnum screenId);
	void tick_screen(int screen_index);

	void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/