#include "device_dc.h"

static struct rt_device_graphic_ops *get_graphic_ops(lite_gui_device_dc_t dc) {
    return (struct rt_device_graphic_ops *) dc->device->user_data;
}

static void wrapped_draw_point(lite_gui_dc_t dc, rt_uint16_t x, rt_uint16_t y) {
    get_graphic_ops((lite_gui_device_dc_t) dc)->set_pixel((const char *) &(dc->brush_color), x, y);
}
// 针对9341的优化，直接调用底层
#ifdef LCD_9341

#include <lcd_basic.h>
#include <stm32f10x.h>

static void wrapped_clear(lite_gui_dc_t dc) {
    LCD_Clear(dc->background_color);
}

static void wrapped_draw_circle(lite_gui_dc_t dc,
                                rt_uint16_t center_x, rt_uint16_t center_y,
                                rt_uint16_t radius) {
    POINT_COLOR = dc->brush_color;
    LCD_Draw_Circle(center_x, center_y, (rt_uint8_t) radius);
}

static void wrapped_draw_rectangle(lite_gui_dc_t dc,
                                   rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                                   rt_uint16_t right_bottom_x, rt_uint16_t right_bottom_y) {
    POINT_COLOR = dc->brush_color;
    LCD_DrawRectangle(left_top_x, left_top_y, right_bottom_x, right_bottom_y);
}

static void
wrapped_draw_vertical_line(lite_gui_dc_t dc, rt_uint16_t x, rt_uint16_t from_y, rt_uint16_t to_y) {
    if (from_y > to_y) {
        wrapped_draw_vertical_line(dc, x, to_y, from_y);
    } else {
        for (rt_uint16_t i = from_y; i <= to_y; i++) {
            dc->draw_point(dc, x, i);
        }
    }
}

static void
wrapped_draw_horizontal_line(lite_gui_dc_t dc, rt_uint16_t from_x, rt_uint16_t to_x, rt_uint16_t y) {
    if (from_x > to_x) {
        wrapped_draw_horizontal_line(dc, to_x, from_x, y);
    } else {
        for (rt_uint16_t i = from_x; i <= to_x; i++) {
            dc->draw_point(dc, i, y);
        }
    }
}

static void wrapped_fill_rectangle(lite_gui_dc_t dc,
                                   rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                                   rt_uint16_t right_bottom_x, rt_uint16_t right_bottom_y) {
    for (rt_uint16_t i = left_top_y; i <= right_bottom_y; i++) {
        LCD_SetCursor(left_top_x, i);
        LCD_WriteRAM_Prepare();
        for (int _ = left_top_x; _ <= right_bottom_x; ++_) {
            LCD_RS_SET;
            LCD_CS_CLR;
            DATAOUT(dc->brush_color);
            LCD_WR_CLR;
            LCD_WR_SET;
            LCD_CS_SET;
        }
    }
}

static void wrapped_draw_ascii_char(lite_gui_dc_t dc,
                                    rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                                    char ch) {
    LCD_ShowChar(left_top_x, left_top_y, (rt_uint8_t) ch, 16, 0);
}

static void wrapped_draw_ascii_string(lite_gui_dc_t dc,
                                      rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                                      char *string) {
    LCD_ShowString(left_top_x, left_top_y, (rt_uint16_t) -1, (rt_uint16_t) -1, 16, (rt_uint8_t *) string);
}

#endif

lite_gui_device_dc_t dc_of_device(rt_device_t device) {
    RT_ASSERT(device->type == RT_Device_Class_Graphic);
    rt_device_open(device, RT_DEVICE_OFLAG_WRONLY);
    lite_gui_device_dc_t device_dc = malloc(sizeof(struct lite_gui_device_dc));
    gui_dc_init(&(device_dc->base));
    device_dc->base.draw_point = wrapped_draw_point;
#ifdef LCD_9341
    device_dc->base.clear = wrapped_clear;
    device_dc->base.draw_vertical_line = wrapped_draw_vertical_line;
    device_dc->base.draw_horizontal_line = wrapped_draw_horizontal_line;
    device_dc->base.draw_rectangle = wrapped_draw_rectangle;
    device_dc->base.draw_circle = wrapped_draw_circle;
    device_dc->base.fill_rectangle = wrapped_fill_rectangle;
#endif
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