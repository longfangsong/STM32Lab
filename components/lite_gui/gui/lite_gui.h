#ifndef _LITE_GUI_H
#define _LITE_GUI_H

#include <rtthread.h>
#include <device_dc.h>

extern lite_gui_dc_t hardware_dc;

void lite_gui_hw_init(rt_device_t lcd_dc);

#endif //_LITE_GUI_H
