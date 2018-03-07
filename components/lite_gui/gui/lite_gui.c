
#include "lite_gui.h"
#include "../dc/device_dc.h"

lite_gui_device_dc_t hardware_dc = RT_NULL;

void lite_gui_hw_init(rt_device_t lcd_dc) {
    hardware_dc = dc_of_device(lcd_dc);
}