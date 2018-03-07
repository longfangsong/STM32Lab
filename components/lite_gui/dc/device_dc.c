#include "device_dc.h"

static struct rt_device_graphic_ops *get_graphic_ops(lite_gui_device_dc_t dc) {
    return (struct rt_device_graphic_ops *) dc->device->user_data;
}

static void wrapped_draw_point(lite_gui_dc_t dc, rt_uint16_t x, rt_uint16_t y) {
    get_graphic_ops((lite_gui_device_dc_t) dc)->set_pixel((const char *) &(dc->brush_color), x, y);
}

lite_gui_device_dc_t dc_of_device(rt_device_t device) {
    RT_ASSERT(device->type == RT_Device_Class_Graphic);
    lite_gui_device_dc_t device_dc = malloc(sizeof(struct lite_gui_device_dc));
    gui_dc_init(&(device_dc->base));
    device_dc->base.draw_point = wrapped_draw_point;
    device_dc->base.background_color = 0x0000;
    device_dc->base.brush_color = 0xFFFF;
    struct rt_device_graphic_info info;
    device->control(device, RTGRAPHIC_CTRL_GET_INFO, &info);
    device_dc->base.width = info.width;
    device_dc->base.height = info.height;
    device_dc->device = device;
    device_dc->base.clear((struct lite_gui_dc *) device_dc);
    return device_dc;
}