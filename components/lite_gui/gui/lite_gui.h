#ifndef _LITE_GUI_H
#define _LITE_GUI_H

#include <rtthread.h>
#include <device_dc.h>
#include <stm32f10x.h>

extern lite_gui_dc_t hardware_dc;

void lite_gui_hw_init(rt_device_t lcd_dc);

void touch_calibrate(lite_gui_device_dc_t dc);

#endif //_LITE_GUI_H
