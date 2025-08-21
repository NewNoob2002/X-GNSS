#include <string.h>
#include <stdio.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

// #include "CommonMacro.h"
// #include "delay.h"

bool GuiNeedUpdate = false;

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

void create_screen_main()
{
  lv_obj_t *main_screen = lv_obj_create(0);
  objects.main = main_screen;
  lv_obj_set_pos(main_screen, 0, 0);
  lv_obj_set_size(main_screen, 294, 126);
  lv_obj_clear_flag(main_screen, LV_OBJ_FLAG_SCROLLABLE);
  {
    lv_obj_t *main_screen_obj = main_screen;
    {
      lv_obj_t *main_status_bar = lv_obj_create(main_screen_obj);
      lv_obj_remove_style_all(main_status_bar);
      lv_obj_set_size(main_status_bar, 294, 36);
      lv_obj_clear_flag(main_status_bar, LV_OBJ_FLAG_SCROLLABLE);
      objects.page_main.status_bar_appWindow = main_status_bar;
      {
        lv_obj_t *main_status_bar_obj = main_status_bar;
        {
          lv_obj_t *icon_4g = lv_label_create(main_status_bar_obj);
          lv_label_set_text(icon_4g, LV_SYMBOL_4G);
          lv_obj_set_style_text_font(icon_4g, &main_symbol2, 0);
          lv_obj_align(icon_4g, LV_ALIGN_TOP_LEFT, 15, 10);
          lv_obj_set_style_text_color(
              icon_4g, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);

          objects.page_main.status_bar_obj.icon_4g = icon_4g;
        }
        {
          lv_obj_t *icon_bluetooth = lv_label_create(main_status_bar_obj);
          lv_label_set_text(icon_bluetooth, LV_SYMBOL_BLUETOOTH);
          lv_obj_set_style_text_font(icon_bluetooth, &lv_font_montserrat_18, 0);
          lv_obj_align(icon_bluetooth, LV_ALIGN_TOP_LEFT, 45, 10);
          lv_obj_set_style_text_color(
              icon_bluetooth, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);

          objects.page_main.status_bar_obj.icon_bluetooth = icon_bluetooth;
        }
        {
          lv_obj_t *icon_wifi = lv_label_create(main_status_bar_obj);
          lv_label_set_text(icon_wifi, LV_SYMBOL_WIFI);
          lv_obj_set_style_text_font(icon_wifi, &lv_font_montserrat_18, 0);
          lv_obj_align(icon_wifi, LV_ALIGN_TOP_LEFT, 70, 10);
          lv_obj_set_style_text_color(
              icon_wifi, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);

          objects.page_main.status_bar_obj.icon_wifi = icon_wifi;
        }
        {
          lv_obj_t *icon_position = lv_label_create(main_status_bar_obj);
          lv_label_set_text(icon_position, LV_SYMBOL_GPS);
          lv_obj_set_style_text_font(icon_position, &lv_font_montserrat_18, 0);
          lv_obj_align(icon_position, LV_ALIGN_TOP_LEFT, 100, 10);
          lv_obj_set_style_text_color(
              icon_position, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);

          objects.page_main.status_bar_obj.icon_position = icon_position;

          lv_obj_t *label_position_t = lv_label_create(main_status_bar_obj);
          lv_label_set_text(label_position_t, "FLOAT");
          lv_obj_set_style_text_font(label_position_t, &lv_font_montserrat_12,
                                     0);
          lv_obj_align_to(label_position_t, icon_position,
                          LV_ALIGN_OUT_RIGHT_MID, 5, 0);
          lv_obj_set_style_text_color(label_position_t, lv_color_hex(0xffffff),
                                      0);

          objects.page_main.status_bar_obj.label_position_t = label_position_t;
        }

        {
          lv_obj_t *icon_radio_status = lv_label_create(main_status_bar_obj);
          lv_label_set_text(icon_radio_status, LV_SYMBOL_RADIO_TRANSMITTER);
          lv_obj_set_style_text_font(icon_radio_status, &main_symbol,
                                     0);
          lv_obj_align(icon_radio_status, LV_ALIGN_TOP_LEFT, 167, 5);
          lv_obj_set_style_text_color(
              icon_radio_status, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);

          objects.page_main.status_bar_obj.icon_radio_status = icon_radio_status;
        }

        {
          lv_obj_t *icon_sd = lv_label_create(main_status_bar_obj);
          lv_label_set_text(icon_sd, LV_SYMBOL_SD_CARD);
          lv_obj_set_style_text_font(icon_sd, &lv_font_montserrat_18, 0);
          lv_obj_align(icon_sd, LV_ALIGN_TOP_LEFT, 205, 10);
          lv_obj_set_style_text_color(icon_sd, lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);

          objects.page_main.status_bar_obj.icon_sd = icon_sd;
        }

        {
          lv_style_init(&objects.page_main.status_bar_obj.battery.style_battery);

          lv_obj_t *img = lv_label_create(main_status_bar_obj);
          lv_label_set_recolor(img, true);
          lv_obj_align(img, LV_ALIGN_TOP_LEFT, 225, 10);
          lv_label_set_text(img, LV_SYMBOL_BATTERY_EMPTY);
          lv_obj_set_style_text_font(img, &lv_font_montserrat_18, 0);
          lv_obj_add_style(
              img, &objects.page_main.status_bar_obj.battery.style_battery, 0);

          lv_obj_t *useage = lv_obj_create(img);
          lv_obj_remove_style_all(useage);
          lv_obj_set_style_bg_opa(useage, LV_OPA_COVER, 0);
          lv_obj_set_style_opa(useage, LV_OPA_COVER, 0);
          lv_obj_set_size(useage, BATT_USAGE_WIDTH, BATT_USAGE_HEIGHT);
          lv_obj_align_to(useage, img, LV_ALIGN_LEFT_MID, 2, 0);
          lv_obj_add_style(
              useage, &objects.page_main.status_bar_obj.battery.style_battery,
              0);

          objects.page_main.status_bar_obj.battery.label_useage = useage;
          objects.page_main.status_bar_obj.battery.icon_battery = img;

          lv_style_set_text_color(
              &objects.page_main.status_bar_obj.battery.style_battery,
              lv_color_hex(0xF44336));

          lv_obj_t *label = lv_label_create(main_status_bar_obj);
          lv_label_set_text(label, "50%");
          lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
          lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
          lv_obj_align_to(label, img, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

          objects.page_main.status_bar_obj.battery.label_text = label;
        }
      }
    }
    {
      lv_obj_t *main_status_bottom = lv_obj_create(main_screen_obj);
      lv_obj_remove_style_all(main_status_bottom);
      lv_obj_set_size(main_status_bottom, 294, 90);
      lv_obj_align(main_status_bottom, LV_ALIGN_BOTTOM_MID, 0, 0);
      lv_obj_clear_flag(main_status_bottom, LV_OBJ_FLAG_SCROLLABLE);
      objects.page_main.status_bottom_appWindow = main_status_bottom;

      lv_obj_t *main_status_bottom_obj = main_status_bottom;
      {
        // satellite
        lv_obj_t *icon_satellite_big = lv_label_create(main_status_bottom_obj);
        lv_label_set_text(icon_satellite_big, LV_SYMBOL_SATELLITE);
        lv_obj_set_style_text_font(icon_satellite_big, &symbol_satellite, 0);
        lv_obj_set_style_text_color(icon_satellite_big,
                                    lv_palette_main(LV_PALETTE_LIGHT_BLUE), 0);
        lv_obj_align(icon_satellite_big, LV_ALIGN_LEFT_MID, 10, 0);

        objects.page_main.status_bottom_obj.satellite.icon_satellite_big =
            icon_satellite_big;
      }

      {
        // satellite_num
        lv_obj_t *label_satellite_num = lv_label_create(main_status_bottom_obj);
        lv_label_set_text(label_satellite_num, "10");
        lv_obj_set_style_text_font(label_satellite_num, &lv_font_montserrat_48,
                                   0);
        lv_obj_set_style_text_color(label_satellite_num,
                                    lv_palette_main(LV_PALETTE_GREY), 0);
        lv_obj_align(label_satellite_num, LV_ALIGN_LEFT_MID, 70, 0);

        objects.page_main.status_bottom_obj.satellite.label_sat_num = label_satellite_num;
      }

      {
        // satellite_num_real
        lv_obj_t *label_satellite_num_real = lv_label_create(main_status_bottom_obj);
        lv_label_set_text(label_satellite_num_real, "/32");
        lv_obj_set_style_text_font(label_satellite_num_real,
                                   &lv_font_montserrat_24, 0);
        lv_obj_set_style_text_color(label_satellite_num_real,
                                    lv_palette_main(LV_PALETTE_GREY), 0);
        lv_obj_align(label_satellite_num_real, LV_ALIGN_LEFT_MID, 120, 10);

        objects.page_main.status_bottom_obj.satellite.label_sat_num_real =
            label_satellite_num_real;
      }

      {
        // button_settings
        lv_obj_t *button_settings = lv_btn_create(main_status_bottom_obj);
        lv_obj_align(button_settings, LV_ALIGN_LEFT_MID, 165, 0);
        lv_obj_set_size(button_settings, 50, 50);

        objects.page_main.status_bottom_obj.radio_workmode.button_radio_workmode = button_settings;
        lv_obj_add_event_cb(button_settings, action_switch_to_work_config,
                            LV_EVENT_PRESSED, (void *)0);
        // icon_workMode
        {
          lv_obj_t *parent_obj = button_settings;
          lv_obj_t *icon_workMode = lv_label_create(parent_obj);
          lv_label_set_text(icon_workMode, LV_SYMBOL_BASE);
          lv_obj_set_style_text_font(icon_workMode, &main_symbol, 0);
          lv_obj_set_style_align(icon_workMode, LV_ALIGN_CENTER,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
          objects.page_main.status_bottom_obj.workmode.icon_settings = icon_workMode;
        }
      }

      {
        // button_radioconfig
        lv_obj_t *button_radioconfig = lv_btn_create(main_status_bottom_obj);
        lv_obj_align(button_radioconfig, LV_ALIGN_LEFT_MID, 225, 0);
        lv_obj_set_size(button_radioconfig, 50, 50);

        objects.page_main.status_bottom_obj.radio_workmode.button_radio_workmode = button_radioconfig;
        lv_obj_add_event_cb(button_radioconfig, action_switch_to_radio_config,
                            LV_EVENT_PRESSED, (void *)0);
        {
          lv_obj_t *parent_obj = button_radioconfig;
          {
            // icon_radioconfig
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, LV_SYMBOL_RADIO_RECEIVER);
            lv_obj_set_style_text_font(obj, &main_symbol, 0);
            objects.page_main.status_bottom_obj.radio_workmode.icon_radio_workmode = obj;
          }
        }
      }
    }
  }

  tick_screen_main();
  Page_Main_Update();
}

void tick_screen_main()
{
  //lv_obj_clear_flag(objects.main, LV_OBJ_FLAG_HIDDEN);
	//Page_Main_Update();
  if (GuiNeedUpdate)
  {
    GuiNeedUpdate = false;
    Page_Main_Update();
  }
}

void create_screen_radio_config()
{
  lv_obj_t *screen_radio = lv_obj_create(0);
  objects.radio_config = screen_radio;
  lv_obj_set_pos(screen_radio, 0, 0);
  lv_obj_set_size(screen_radio, 294, 126);
  {
    lv_obj_t *screen_radio_obj = screen_radio;

    {
      // button_return
      lv_obj_t *button_return_t = lv_btn_create(screen_radio_obj);
      lv_obj_align(button_return_t, LV_ALIGN_TOP_LEFT, 10, 10);
      lv_obj_set_size(button_return_t, 50, 30);
      lv_obj_add_event_cb(button_return_t, action_switch_to_main,
                          LV_EVENT_PRESSED, (void *)0);

      objects.page_work.button_return = button_return_t;
      {
        lv_obj_t *button_return_obj = button_return_t;
        {
          // label_back
          lv_obj_t *label_return_obj = lv_label_create(button_return_obj);
          // objects.page_work. = label_return_obj;
          lv_obj_set_pos(label_return_obj, 0, 0);
          lv_obj_set_size(label_return_obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
          lv_obj_set_style_align(label_return_obj, LV_ALIGN_CENTER,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_label_set_text(label_return_obj, LV_SYMBOL_LEFT);
        }
      }
    }

    {
      // label_radioStatus
      lv_obj_t *label_radioStatus = lv_label_create(screen_radio_obj);
      lv_label_set_text(label_radioStatus, LV_SYMBOL_RADIO_RECEIVER);
      lv_obj_set_style_text_font(label_radioStatus, &main_symbol, 0);
      lv_obj_set_style_text_color(label_radioStatus, lv_palette_main(LV_PALETTE_GREEN), 0);
      lv_obj_align(label_radioStatus, LV_ALIGN_TOP_RIGHT, -13, 5);

      objects.page_work.lable_radio_status = label_radioStatus;
    }
    {
      lv_obj_t *icon_radioconfig_t = lv_label_create(screen_radio_obj);
      lv_label_set_text(icon_radioconfig_t, LV_SYMBOL_FREQUENCE);
      lv_obj_set_style_text_font(icon_radioconfig_t, &symbol_radio, 0);
      lv_obj_set_style_text_color(icon_radioconfig_t,
                                  lv_palette_main(LV_PALETTE_GREEN), 0);
      lv_obj_align(icon_radioconfig_t, LV_ALIGN_LEFT_MID, 10, -5);

      lv_obj_t *label_radioFreq = lv_label_create(screen_radio_obj);
      lv_label_set_text(label_radioFreq, "Freq");
      lv_obj_set_style_text_font(label_radioFreq, &lv_font_montserrat_16, 0);
      lv_obj_set_style_text_color(label_radioFreq,
                                  lv_palette_main(LV_PALETTE_GREEN), 0);
      lv_obj_align(label_radioFreq, LV_ALIGN_LEFT_MID, 40, -5);

      const char *opts = "1[455.05]\n2[456.05]\n3[457.05]\n4[458.05]\n5[459.05]"
                         "\n6[460.05]\n7[461.05]\n8[462.05]\n9[463.05]";

      lv_obj_t *roller = lv_roller_create(screen_radio_obj);
      lv_roller_set_options(roller, opts, LV_ROLLER_MODE_INFINITE);
      lv_roller_set_visible_row_count(roller, 2);
      lv_obj_set_size(roller, 125, 50);
      lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_LEFT, 0);
      lv_obj_align(roller, LV_ALIGN_LEFT_MID, 90, -25);
      lv_obj_add_event_cb(roller, action_radio_roller, LV_EVENT_ALL, NULL);
      //   lv_roller_set_selected(roller, 2, LV_ANIM_OFF);

      objects.page_work.roller_radioFreq = roller;
    }

    {
      lv_obj_t *icon_radioProtocol = lv_label_create(screen_radio_obj);
      lv_label_set_text(icon_radioProtocol, LV_SYMBOL_SHUFFLE);
      lv_obj_set_style_text_font(icon_radioProtocol, &lv_font_montserrat_18,
                                 0);
      lv_obj_set_style_text_color(icon_radioProtocol,
                                  lv_palette_main(LV_PALETTE_GREEN), 0);
      lv_obj_align(icon_radioProtocol, LV_ALIGN_LEFT_MID, 10, 40);

      lv_obj_t *label_radioProtocol = lv_label_create(screen_radio_obj);
      lv_label_set_text(label_radioProtocol, "Prot");
      lv_obj_set_style_text_font(label_radioProtocol, &lv_font_montserrat_16, 0);
      lv_obj_set_style_text_color(label_radioProtocol,
                                  lv_palette_main(LV_PALETTE_GREEN), 0);
      lv_obj_align(label_radioProtocol, LV_ALIGN_LEFT_MID, 40, 40);

      lv_obj_t *dropdown_radioProtocol_t = lv_roller_create(screen_radio_obj);
      lv_roller_set_options(dropdown_radioProtocol_t,
                            "TRIMTALK\nTRANSEOT\nSOUTH", LV_ROLLER_MODE_INFINITE);
      lv_roller_set_visible_row_count(dropdown_radioProtocol_t, 2);
      lv_obj_set_size(dropdown_radioProtocol_t, 125, 50);
      lv_obj_set_style_text_align(dropdown_radioProtocol_t, LV_TEXT_ALIGN_LEFT, 0);
      lv_obj_align(dropdown_radioProtocol_t, LV_ALIGN_LEFT_MID, 90, 32);
      lv_obj_add_event_cb(dropdown_radioProtocol_t,
                          action_radio_protocol_roller, LV_EVENT_ALL, NULL);

      objects.page_work.dropdown_radioProtocol = dropdown_radioProtocol_t;
    }

    {
      // button_confirm
      lv_obj_t *button_confirm_t = lv_btn_create(screen_radio_obj);
      lv_obj_set_size(button_confirm_t, 50, 30);
      lv_obj_align(button_confirm_t, LV_ALIGN_TOP_RIGHT, -5, 40);
      lv_obj_add_event_cb(button_confirm_t, action_switch_to_main,
                          LV_EVENT_PRESSED, (void *)0);
      {
        lv_obj_t *button_confirm_obj = button_confirm_t;
        {
          // label_back
          lv_obj_t *label_return_obj = lv_label_create(button_confirm_obj);
          // objects.icon_worktomain = label_return_obj;
          lv_obj_set_pos(label_return_obj, 0, 0);
          lv_obj_set_size(label_return_obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
          lv_obj_set_style_align(label_return_obj, LV_ALIGN_CENTER,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_label_set_text(label_return_obj, "Set");
        }
      }
      objects.page_work.Button_workmodeSet = button_confirm_t;
    }

    {
      // button_close
      lv_obj_t *button_close_t = lv_btn_create(screen_radio_obj);
      lv_obj_set_size(button_close_t, 50, 30);
      lv_obj_align(button_close_t, LV_ALIGN_TOP_RIGHT, -5, 80);
      lv_obj_add_event_cb(button_close_t, action_radio_close,
                          LV_EVENT_PRESSED, (void *)0);
      {
        lv_obj_t *button_close_obj = button_close_t;
        {
          // label_back
          lv_obj_t *label_return_obj = lv_label_create(button_close_obj);
          // objects.icon_worktomain = label_return_obj;
          lv_obj_set_pos(label_return_obj, 0, 0);
          lv_obj_set_size(label_return_obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
          lv_obj_set_style_align(label_return_obj, LV_ALIGN_CENTER,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_label_set_text(label_return_obj, "Close");
        }
      }
      objects.page_work.Button_workmodeClose = button_close_t;
    }
  }

  tick_screen_radio_config();
  Page_RadioConfig_Update();
}

void tick_screen_radio_config()
{
  lv_obj_clear_flag(objects.radio_config, LV_OBJ_FLAG_HIDDEN);
  if (GuiNeedUpdate)
  {
    GuiNeedUpdate = false;
    Page_RadioConfig_Update();
  }
}

void create_screen_work_config()
{
  lv_obj_t *screen_work_config = lv_obj_create(0);
  objects.work_config = screen_work_config;
  lv_obj_set_pos(screen_work_config, 0, 0);
  lv_obj_set_size(screen_work_config, 294, 126);

  {
    lv_obj_t *parent_obj = screen_work_config;

    // First child: Back button
    {
      lv_obj_t *button_settingsBackMain = lv_btn_create(parent_obj);
      lv_obj_set_size(button_settingsBackMain, 50, 30);
      lv_obj_align(button_settingsBackMain, LV_ALIGN_TOP_LEFT, 10, 10);
      lv_obj_add_event_cb(button_settingsBackMain, action_switch_to_main, LV_EVENT_PRESSED, (void *)0);

      lv_obj_t *label_settingsBackMain = lv_label_create(button_settingsBackMain);
      lv_label_set_text(label_settingsBackMain, LV_SYMBOL_LEFT);
      lv_obj_set_style_text_font(label_settingsBackMain, &lv_font_montserrat_18, 0);
      lv_obj_set_style_text_color(label_settingsBackMain, lv_color_hex(0xffffff), 0);
      lv_obj_center(label_settingsBackMain);
    }

    // Second child: 4G status and switch
    {
      // Static status icon
      lv_obj_t *icon_static = lv_label_create(parent_obj);
      lv_label_set_text(icon_static, LV_SYMBOL_STATIC);
      lv_obj_set_style_text_font(icon_static, &main_symbol2, 0);
      lv_obj_set_style_text_color(icon_static, lv_palette_main(LV_PALETTE_BLUE), 0);
      lv_obj_align(icon_static, LV_ALIGN_TOP_LEFT, 68, 20);

      // Static status label
      lv_obj_t *button_static = lv_btn_create(parent_obj);
      //      lv_obj_set_style_bg_color(button_static, lv_color_hex(0x000000), 0);
      lv_obj_align(button_static, LV_ALIGN_TOP_LEFT, 95, 10);
      lv_obj_add_event_cb(button_static, action_switch_to_static_settings, LV_EVENT_PRESSED, (void *)0);

      lv_obj_t *label_static = lv_label_create(button_static);
      lv_label_set_text(label_static, "Static Record");
      lv_obj_set_style_text_font(label_static, &lv_font_montserrat_16, 0);
      lv_obj_set_style_text_color(label_static, lv_color_hex(0xffffff), 0);
      lv_obj_center(label_static);

      // 4G status icon
      lv_obj_t *icon_4g = lv_label_create(parent_obj);
      lv_label_set_text(icon_4g, LV_SYMBOL_4G);
      lv_obj_set_style_text_font(icon_4g, &main_symbol2, 0);
      lv_obj_set_style_text_color(icon_4g, lv_palette_main(LV_PALETTE_GREEN), 0);
      lv_obj_align(icon_4g, LV_ALIGN_LEFT_MID, 68, 15);

      lv_obj_t *button_4g = lv_btn_create(parent_obj);
      //      lv_obj_set_style_bg_color(button_4g, lv_color_hex(0x000000), 0);
      lv_obj_align(button_4g, LV_ALIGN_LEFT_MID, 95, 15);
      lv_obj_add_event_cb(button_4g, action_switch_to_network_settings, LV_EVENT_PRESSED, (void *)0);

      lv_obj_t *label_4g = lv_label_create(button_4g);
      lv_label_set_text(label_4g, "Auto Start");
      lv_obj_set_style_text_font(label_4g, &lv_font_montserrat_16, 0);
      lv_obj_set_style_text_color(label_4g, lv_color_hex(0xffffff), 0);
      lv_obj_center(label_4g);
    }

    // Third child: Firmware version and compile time
    {
      lv_obj_t *firmware_container = lv_obj_create(parent_obj);
      lv_obj_remove_style_all(firmware_container);
      lv_obj_set_size(firmware_container, 280, 25);
      lv_obj_align(firmware_container, LV_ALIGN_BOTTOM_MID, 0, 0);
      lv_obj_clear_flag(firmware_container, LV_OBJ_FLAG_SCROLLABLE);

      // Firmware version label
      lv_obj_t *label_firmware = lv_label_create(firmware_container);
      lv_label_set_text_fmt(label_firmware, "Firmware: V%s", "1.0.0");
      lv_obj_set_style_text_font(label_firmware, &lv_font_montserrat_12, 0);
      lv_obj_set_style_text_color(label_firmware, lv_palette_main(LV_PALETTE_GREY), 0);
      lv_obj_align(label_firmware, LV_ALIGN_LEFT_MID, 10, 0);

      // Compile time label
      lv_obj_t *label_compile_time = lv_label_create(firmware_container);
      lv_label_set_text(label_compile_time, __DATE__ " " __TIME__);
      lv_obj_set_style_text_font(label_compile_time, &lv_font_montserrat_12, 0);
      lv_obj_set_style_text_color(label_compile_time, lv_palette_main(LV_PALETTE_GREY), 0);
      lv_obj_align(label_compile_time, LV_ALIGN_RIGHT_MID, -10, 0);
    }
  }

  tick_screen_work_config();
}

void tick_screen_work_config()
{
  lv_obj_clear_flag(objects.work_config, LV_OBJ_FLAG_HIDDEN);
}

void create_screen_static_settings()
{
  lv_obj_t *screen_static_settings = lv_obj_create(0);
  objects.static_settings = screen_static_settings;
  lv_obj_set_pos(screen_static_settings, 0, 0);
  lv_obj_set_size(screen_static_settings, 294, 126);

  {
    lv_obj_t *parent_obj = screen_static_settings;
    {
      /*back button*/
      lv_obj_t *button_static_back = lv_btn_create(parent_obj);
      lv_obj_set_size(button_static_back, 50, 30);
      lv_obj_align(button_static_back, LV_ALIGN_TOP_LEFT, 10, 10);
      lv_obj_add_event_cb(button_static_back, action_switch_to_work_config, LV_EVENT_PRESSED, (void *)0);

      lv_obj_t *label_static_back = lv_label_create(button_static_back);
      lv_label_set_text(label_static_back, LV_SYMBOL_LEFT);
      lv_obj_set_style_text_font(label_static_back, &lv_font_montserrat_18, 0);
      lv_obj_set_style_text_color(label_static_back, lv_color_hex(0xffffff), 0);
      lv_obj_center(label_static_back);

      objects.page_static.button_static_back = button_static_back;
    }

    {
      lv_obj_t *icon_static = lv_label_create(parent_obj);
      lv_label_set_text(icon_static, LV_SYMBOL_STATIC);
      lv_obj_set_style_text_font(icon_static, &main_symbol2, 0);
      lv_obj_set_style_text_color(icon_static, lv_palette_main(LV_PALETTE_BLUE), 0);
      lv_obj_align(icon_static, LV_ALIGN_TOP_LEFT, 70, 15);

      lv_obj_t *label_static = lv_label_create(parent_obj);
      lv_label_set_text_fmt(label_static, "Status: %s", systemInfo.record_status ? "ON" : "OFF");
      lv_obj_set_style_text_font(label_static, &lv_font_montserrat_16, 0);
      lv_obj_set_style_text_color(label_static, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(label_static, icon_static, LV_ALIGN_LEFT_MID, 25, 0);

      lv_obj_t *switch_static = lv_switch_create(parent_obj);
      lv_obj_align_to(switch_static, label_static, LV_ALIGN_RIGHT_MID, 70, 0);
      lv_obj_add_event_cb(switch_static, action_turn_onoff_static, LV_EVENT_VALUE_CHANGED, (void *)0);
      

      lv_obj_t *icon_static_name = lv_label_create(parent_obj);
      lv_label_set_text(icon_static_name, LV_SYMBOL_FILE);
      lv_obj_set_style_text_font(icon_static_name, &lv_font_montserrat_18, 0);
      lv_obj_set_style_text_color(icon_static_name, lv_palette_main(LV_PALETTE_GREY), 0);
      lv_obj_align(icon_static_name, LV_ALIGN_TOP_LEFT, 10, 45);

      lv_obj_t *lable_static_name = lv_label_create(parent_obj);
      lv_label_set_text(lable_static_name, "REC: -");
      lv_obj_set_style_text_font(lable_static_name, &lv_font_montserrat_16, 0);
      lv_obj_set_style_text_color(lable_static_name, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(lable_static_name, icon_static_name, LV_ALIGN_LEFT_MID, 20, 0);


      lv_obj_t *lable_static_type = lv_label_create(parent_obj);
      lv_label_set_text(lable_static_type, "Type: -");
      lv_obj_set_style_text_font(lable_static_type, &lv_font_montserrat_16, 0);
      lv_obj_set_style_text_color(lable_static_type, lv_color_hex(0xffffff), 0);
      lv_obj_align(lable_static_type, LV_ALIGN_TOP_LEFT, 10, 70);

      lv_obj_t *lable_static_time = lv_label_create(parent_obj);
      lv_label_set_text(lable_static_time, "Time: -");
      lv_obj_set_style_text_font(lable_static_time, &lv_font_montserrat_16, 0);
      lv_obj_set_style_text_color(lable_static_time, lv_color_hex(0xffffff), 0);
      lv_obj_align(lable_static_time, LV_ALIGN_TOP_LEFT, 150, 70);


      lv_obj_t *icon_sd = lv_label_create(parent_obj);
      lv_label_set_text(icon_sd, LV_SYMBOL_SD_CARD);
      lv_obj_set_style_text_font(icon_sd, &lv_font_montserrat_18, 0);
      lv_obj_set_style_text_color(icon_sd, lv_palette_main(LV_PALETTE_GREY), 0);
      lv_obj_align(icon_sd, LV_ALIGN_TOP_LEFT, 10, 95);

      lv_obj_t *label_sd = lv_label_create(parent_obj);
      lv_label_set_text_fmt(label_sd, "RSC: -MB");
      lv_obj_set_style_text_font(label_sd, &lv_font_montserrat_16, 0);
      lv_obj_set_style_text_color(label_sd, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(label_sd, icon_sd, LV_ALIGN_LEFT_MID, 20, 0);

      objects.page_static.label_static = label_static;
      objects.page_static.switch_static_onoff = switch_static;
      objects.page_static.icon_static_name = icon_static_name;
      objects.page_static.label_static_name = lable_static_name;
      objects.page_static.label_static_type = lable_static_type;
      objects.page_static.label_static_time = lable_static_time;
      objects.page_static.icon_sd = icon_sd;
      objects.page_static.label_sd = label_sd;
    }
  }
}

void tick_screen_static_settings()
{
  lv_obj_clear_flag(objects.static_settings, LV_OBJ_FLAG_HIDDEN);
}

void create_screen_network_settings()
{
  lv_obj_t *screen_network_settings = lv_obj_create(0);
  objects.network_settings = screen_network_settings;
  lv_obj_set_pos(screen_network_settings, 0, 0);
  lv_obj_set_size(screen_network_settings, 294, 126);
  {
    lv_obj_t *parent_obj = screen_network_settings;
    {
      /*back button*/
      lv_obj_t *button_network_back = lv_btn_create(parent_obj);
      lv_obj_set_size(button_network_back, 50, 30);
      lv_obj_align(button_network_back, LV_ALIGN_TOP_LEFT, 10, 10);
      lv_obj_add_event_cb(button_network_back, action_switch_to_work_config, LV_EVENT_PRESSED, (void *)0);

      lv_obj_t *label_network_back = lv_label_create(button_network_back);
      lv_label_set_text(label_network_back, LV_SYMBOL_LEFT);
      lv_obj_set_style_text_font(label_network_back, &lv_font_montserrat_18, 0);
      lv_obj_set_style_text_color(label_network_back, lv_color_hex(0xffffff), 0);
      lv_obj_center(label_network_back);

      objects.page_network.button_network_back = button_network_back;
    }

    {
      lv_obj_t *icon_4g = lv_label_create(parent_obj);
      lv_label_set_text(icon_4g, LV_SYMBOL_4G);
      lv_obj_set_style_text_font(icon_4g, &main_symbol2, 0);
      lv_obj_set_style_text_color(icon_4g, lv_palette_main(LV_PALETTE_GREY), 0);
      lv_obj_align(icon_4g, LV_ALIGN_TOP_LEFT, 70, 15);

      lv_obj_t *label_4g_status = lv_label_create(parent_obj);
      lv_label_set_text(label_4g_status, "CSQ: 0");
      lv_obj_set_style_text_font(label_4g_status, &lv_font_montserrat_16, 0);
      lv_obj_set_style_text_color(label_4g_status, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(label_4g_status, icon_4g, LV_ALIGN_TOP_LEFT, 25, 0);

      lv_obj_t *label_4g = lv_label_create(parent_obj);
      lv_label_set_text(label_4g, "-OFF");
      lv_obj_set_style_text_font(label_4g, &lv_font_montserrat_16, 0);
      lv_obj_set_style_text_color(label_4g, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(label_4g, label_4g_status, LV_ALIGN_TOP_LEFT, 75, 0);

      lv_obj_t *switch_4g = lv_switch_create(parent_obj);
      lv_obj_align_to(switch_4g, label_4g, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
      lv_obj_add_event_cb(switch_4g, action_turn_onoff_4g, LV_EVENT_VALUE_CHANGED, (void *)0);

      lv_obj_t *lable_ntrip_server = lv_label_create(parent_obj);
      lv_label_set_text(lable_ntrip_server, "NTRIP Server:");
      lv_obj_set_style_text_font(lable_ntrip_server, &lv_font_montserrat_16, 0);
      lv_obj_set_style_text_color(lable_ntrip_server, lv_color_hex(0xffffff), 0);
      lv_obj_align(lable_ntrip_server, LV_ALIGN_LEFT_MID, 5, 0);

      lv_obj_t *ip_ntrip_server = lv_label_create(parent_obj);
      lv_label_set_text(ip_ntrip_server, "-");
      lv_obj_set_style_text_font(ip_ntrip_server, &lv_font_montserrat_16, 0);
      lv_obj_set_style_text_color(ip_ntrip_server, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(ip_ntrip_server, lable_ntrip_server, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

      lv_obj_t *mount_nrtip_server = lv_label_create(parent_obj);
      lv_label_set_text(mount_nrtip_server, "MountPoint: -");
      lv_obj_set_style_text_font(mount_nrtip_server, &lv_font_montserrat_16, 0);
      lv_obj_set_style_text_color(mount_nrtip_server, lv_color_hex(0xffffff), 0);
      lv_obj_align_to(mount_nrtip_server, lable_ntrip_server, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

      lv_obj_t *buttom_onestep_start = lv_btn_create(parent_obj);
      lv_obj_set_size(buttom_onestep_start, 130, 25);
      lv_obj_align(buttom_onestep_start, LV_ALIGN_BOTTOM_LEFT, 10, -5);
      lv_obj_add_event_cb(buttom_onestep_start, action_switch_to_work_config, LV_EVENT_PRESSED, (void *)0);

      lv_obj_t *label_onestep_start = lv_label_create(buttom_onestep_start);
      lv_label_set_text(label_onestep_start, "OneStep-Start");
      lv_obj_set_style_text_font(label_onestep_start, &lv_font_montserrat_16, 0);
      lv_obj_set_style_text_color(label_onestep_start, lv_color_hex(0xffffff), 0);
      lv_obj_center(label_onestep_start);

      objects.page_network.icon_4g = icon_4g;
      objects.page_network.label_4g = label_4g;
      objects.page_network.switch_4g_onoff = switch_4g;
    }
  }
}

void tick_screen_network_settings()
{
  lv_obj_clear_flag(objects.network_settings, LV_OBJ_FLAG_HIDDEN);
}

void create_screen_poweroff()
{
  lv_obj_t *screen_poweroff = lv_obj_create(0);
  objects.power_off = screen_poweroff;
  lv_obj_set_pos(screen_poweroff, 0, 0);
  lv_obj_set_size(screen_poweroff, 294, 126);
  {
    lv_obj_t *parent_obj = screen_poweroff;

    // Power off label
    lv_obj_t *label_poweroff = lv_label_create(parent_obj);
    lv_label_set_text(label_poweroff, "Power Off......\nWait Board Sync");
    lv_obj_set_style_text_font(label_poweroff, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(label_poweroff, lv_color_hex(0xffffff), 0);
    lv_obj_align(label_poweroff, LV_ALIGN_CENTER, 0, 0);
  }
}

void tick_screen_poweroff()
{
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_work_config,
    tick_screen_radio_config,
    tick_screen_static_settings,
    tick_screen_network_settings,
    tick_screen_poweroff,
};
void tick_screen(int screen_index)
{
  tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId)
{
  tick_screen_funcs[screenId - 1]();
}

void create_screens()
{
  lv_disp_t *dispp = lv_disp_get_default();
  lv_theme_t *theme = lv_theme_default_init(
      dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
      true, LV_FONT_DEFAULT);
  lv_disp_set_theme(dispp, theme);

  create_screen_main();
  create_screen_work_config();
  create_screen_radio_config();
  create_screen_static_settings();
  create_screen_network_settings();
  create_screen_poweroff();
}