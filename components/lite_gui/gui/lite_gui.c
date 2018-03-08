#include "lite_gui.h"

lite_gui_dc_t hardware_dc = RT_NULL;

void lite_gui_hw_init(rt_device_t lcd_dc) {
    hardware_dc = (lite_gui_dc_t) dc_of_device(lcd_dc);
}