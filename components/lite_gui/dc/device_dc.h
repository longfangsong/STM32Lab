#ifndef LITE_GUI_HARDWARE_DC_H
#define LITE_GUI_HARDWARE_DC_H

#include "dc.h"

struct lite_gui_device_dc {
    struct lite_gui_dc base;
    rt_device_t device;
};

typedef struct lite_gui_device_dc *lite_gui_device_dc_t;

lite_gui_device_dc_t dc_of_device(rt_device_t device);


#endif //LITE_GUI_HARDWARE_DC_H
