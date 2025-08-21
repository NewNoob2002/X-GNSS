#ifndef LV_PORT_H
#define LV_PORT_H
#include "lvgl.h"
#include "demos/lv_demos.h"

void lv_port_disp_init();
void lv_port_indev_init();
void lv_port_fs_init();

inline void lv_port_init(){
	lv_port_disp_init();
//	lv_port_indev_init();
//	lv_port_fs_init();
}
#endif