#include <stdio.h>
#include "actions.h"
#include "ui.h"
#include "vars.h"
#include "anim.h"
#include "fonts.h"

void action_switch_to_work_config(lv_event_t *e)
{
	loadScreen(SCREEN_ID_WORK_CONFIG);
	// TODO: Implement action switch_to_work_config here
}
void action_switch_to_radio_config(lv_event_t *e)
{
	loadScreen(SCREEN_ID_RADIO_CONFIG);
}

void action_switch_to_static_settings(lv_event_t *e)
{
	loadScreen(SCREEN_ID_STATIC_SETTINGS);
}

void action_turn_onoff_static(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_target(e);
	if (code == LV_EVENT_VALUE_CHANGED)
	{
		if (lv_obj_has_state(obj, LV_STATE_CHECKED))
		{
			systemInfo.record_status = 1;
			systemInfo.record_op = 1;
			lv_label_set_text_fmt(objects.page_static.label_static, "Status: ON");
		}
		else
		{
			systemInfo.record_status = 0;
			systemInfo.record_op = 1;
			lv_label_set_text_fmt(objects.page_static.label_static, "Status: OFF");
		}
	}
}

void action_switch_to_network_settings(lv_event_t *e)
{
	loadScreen(SCREEN_ID_NETWORK_SETTINGS);
}

void action_turn_onoff_4g(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_target(e);
	if (code == LV_EVENT_VALUE_CHANGED)
	{
		char buf[32];
		printf("4g: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "ON" : "OFF");
	}
}

void action_switch_to_main(lv_event_t *e)
{
	loadScreen(SCREEN_ID_MAIN);
}

void action_radio_roller(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_target(e);
	if (code == LV_EVENT_VALUE_CHANGED)
	{
		char buf[32];
		lv_roller_get_selected_str(obj, buf, sizeof(buf));
		uint16_t index =lv_roller_get_selected(obj);
		printf("Selected value: %s, index:%d\n", buf, index);
	}
}

void action_radio_protocol_roller(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_target(e);
	if (code == LV_EVENT_VALUE_CHANGED)
	{
		char buf[32];
		lv_roller_get_selected_str(obj, buf, sizeof(buf));
		uint16_t index =lv_roller_get_selected(obj);
		printf("Selected value: %s, index:%d\n", buf, index);
	}
}

void action_radio_close(lv_event_t *e)
{
	loadScreen(SCREEN_ID_POWER_OFF);
}

void lv_update_battery(pBatteryState p_batteryState, status_bar *bar)
{
	uint8_t battery_percentage = p_batteryState->Percent;
	/*battery*/
	lv_label_set_text_fmt(bar->battery.label_text, "%d%%", battery_percentage);

	bool Is_BattCharging = p_batteryState->chargeStatus == 1 || p_batteryState->chargeStatus == 2; // 数据接口
	bool is_FullCharge = (p_batteryState->Percent >= 100);
	lv_obj_t *battery_useage = bar->battery.label_useage;
	lv_obj_t *battery_img = bar->battery.icon_battery;
	static bool Is_BattChargingAnimActive = false;
	if (1)
	{
		if (!Is_BattChargingAnimActive && !is_FullCharge)
		{
			lv_style_set_bg_color(&bar->battery.style_battery, lv_color_hex(0x4CAF50));
			lv_style_set_text_color(&bar->battery.style_battery, lv_color_hex(0x4CAF50));
			StatusBar_AnimCreate(battery_useage);
			Is_BattChargingAnimActive = true;
		}
	}
	else
	{
		if (Is_BattChargingAnimActive)
		{
			lv_anim_del(battery_useage, NULL);
			StatusBar_ConBattSetOpa(battery_useage, LV_OPA_COVER);
			Is_BattChargingAnimActive = false;
		}
		lv_coord_t width = lv_map(battery_percentage, 0, 100, 0, BATT_USAGE_WIDTH);

		lv_color_t battery_color;
		if (battery_percentage > 50)
		{
			battery_color = lv_color_hex(0x4CAF50); // 绿色
		}
		else if (battery_percentage > 20)
		{
			battery_color = lv_color_hex(0xFF9800); // 橙色
		}
		else
		{
			battery_color = lv_color_hex(0xF44336); // 红色
		}
		lv_style_set_bg_color(&bar->battery.style_battery, battery_color);
		lv_style_set_text_color(&bar->battery.style_battery, battery_color);
		lv_obj_set_width(battery_useage, width);
	}
}

static void page_mainTopUpdate(status_bar *bar)
{
	// 4G_Icon
	if (systemInfo.gprs_status)
		lv_obj_set_style_text_color(bar->icon_4g, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);
	else
		lv_obj_set_style_text_color(bar->icon_4g, lv_palette_main(LV_PALETTE_GREY), 0);
	// Bluetooth
	if (1)
		lv_obj_set_style_text_color(bar->icon_bluetooth, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);
	else
		lv_obj_set_style_text_color(bar->icon_bluetooth, lv_palette_main(LV_PALETTE_GREY), 0);
	// Wifi
	if (systemInfo.wifi_status)
		lv_obj_set_style_text_color(bar->icon_wifi, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);
	else
		lv_obj_set_style_text_color(bar->icon_wifi, lv_palette_main(LV_PALETTE_GREY), 0);
	// Position
	switch (systemInfo.coordinate_status)
	{
	case 0: // NONE
		lv_obj_set_style_text_color(bar->icon_position, lv_palette_main(LV_PALETTE_RED), 0);
		lv_label_set_text(bar->label_position_t, "NONE");
		break;
	case 1: // Single
		lv_obj_set_style_text_color(bar->icon_position, lv_palette_main(LV_PALETTE_YELLOW), 0);
		lv_label_set_text(bar->label_position_t, "SINGLE");
		break;
	case 4: // FIX
		lv_obj_set_style_text_color(bar->icon_position, lv_palette_main(LV_PALETTE_GREEN), 0);
		lv_label_set_text(bar->label_position_t, "FIX");
		break;
	case 5: // FLOAT
		lv_obj_set_style_text_color(bar->icon_position, lv_palette_main(LV_PALETTE_YELLOW), 0);
		lv_label_set_text(bar->label_position_t, "FLOAT");
		break;
	case 7:
		lv_obj_set_style_text_color(bar->icon_position, lv_palette_main(LV_PALETTE_BLUE), 0);
		lv_label_set_text(bar->label_position_t, "BASE");
		break;
	}

	// SD
	if (systemInfo.record_status)
	{
		lv_obj_set_style_text_color(bar->icon_sd, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);
	}
	else
	{
		lv_obj_set_style_text_color(bar->icon_sd, lv_palette_main(LV_PALETTE_GREY), 0);
	}
	/*Battery*/
	lv_update_battery(&batteryState, bar);
}
void Page_Main_Update()
{
	// TOP Update
	page_mainTopUpdate(&objects.page_main.status_bar_obj);
	//satallite update
	lv_label_set_text_fmt(objects.page_main.status_bottom_obj.satellite.label_sat_num, "%d", systemInfo.satellite_number_used);
	lv_label_set_text_fmt(objects.page_main.status_bottom_obj.satellite.label_sat_num_real, "/%d", systemInfo.satellite_number_track);

	switch (systemInfo.work_mode)
	{
	case rover_mode:
		lv_label_set_text(objects.page_main.status_bottom_obj.workmode.icon_settings, LV_SYMBOL_ROVER);
		break;
	case single_mode:
		lv_label_set_text(objects.page_main.status_bottom_obj.workmode.icon_settings, LV_SYMBOL_ROVER);
		break;
	case base_mode:
		lv_label_set_text(objects.page_main.status_bottom_obj.workmode.icon_settings, LV_SYMBOL_BASE);
		break;
	case autobase_mode:
		lv_label_set_text(objects.page_main.status_bottom_obj.workmode.icon_settings, LV_SYMBOL_BASE);
		break;
	}
}


void Page_RadioConfig_Update()
{
	// Update radio status
	if (systemInfo.radio_status)
	{
		lv_obj_set_style_text_color(objects.page_work.lable_radio_status, lv_palette_main(LV_PALETTE_GREEN), 0);
	}
	else
	{
		lv_obj_set_style_text_color(objects.page_work.lable_radio_status, lv_palette_main(LV_PALETTE_RED), 0);
	}

	// // Update radio frequency
	// lv_label_set_text_fmt(objects.page_work.roller_radioFreq, "%dMHz", systemInfo.radio_freq);

	// // Update radio protocol
	// lv_roller_set_selected(objects.page_work.dropdown_radioProtocol, systemInfo.radio_protocol, LV_ANIM_ON);
}

void add_objs_to_group(lv_obj_t *parent, lv_group_t *group)
{
	// 如果父对象为空，返回
	if (parent == NULL)
		return;

	// 获取子对象数量
	uint32_t child_cnt = lv_obj_get_child_cnt(parent);

	// 遍历所有子对象
	for (uint32_t i = 0; i < child_cnt; i++)
	{
		lv_obj_t *child = lv_obj_get_child(parent, i);

		// 检查是否是可聚焦控件类型
		if (
			lv_obj_check_type(child, &lv_roller_class) || // roller
			//            lv_obj_check_type(child, &lv_checkbox_class) ||   //checkbox
			lv_obj_check_type(child, &lv_btn_class) || // Button
													   //            lv_obj_check_type(child, &lv_dropdown_class) || // Dropdown
			lv_obj_check_type(child, &lv_switch_class) // Switch
		)
		{

			// 添加到组
			lv_group_add_obj(group, child);
			// printf("Added control to focus group\n");
		}

		// 递归处理子对象的子对象
		add_objs_to_group(child, group);
	}
}

// 当屏幕创建后调用此函数添加其控件到焦点组
void lv_focus_init(lv_obj_t *screen)
{
	lv_group_t *defualt_group = lv_group_get_default();
	// 先清除之前的所有对象
	lv_group_remove_all_objs(defualt_group);

	// 添加新屏幕上的所有控件
	add_objs_to_group(screen, defualt_group);

	//    // 设置第一个对象为焦点
	//    lv_group_focus_next(defualt_group);
}