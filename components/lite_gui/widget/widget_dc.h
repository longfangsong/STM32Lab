#ifndef __LITE_GUI_WIDGET_DC_H
#define __LITE_GUI_WIDGET_DC_H

#include "dc.h"

struct lite_gui_widget_dc {
    struct lite_gui_dc base;
    lite_gui_dc_t parent;
    rt_int16_t x_from_parent;
    rt_int16_t y_from_parent;
};
typedef struct lite_gui_widget_dc *lite_gui_widget_dc_t;

lite_gui_widget_dc_t
lite_gui_widget_dc_init(lite_gui_widget_dc_t dc, lite_gui_dc_t parent, rt_int16_t x_from_parent,
                        rt_int16_t y_from_parent,
                        rt_uint16_t width, rt_uint16_t height);

#endif