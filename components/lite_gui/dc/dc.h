#ifndef LITE_GUI_DC_H
#define LITE_GUI_DC_H

#include <rtthread.h>
#include <stdlib.h>
#include "../font/font.h"

struct lite_gui_dc {
    rt_uint16_t brush_color;
    rt_uint16_t background_color;
    rt_uint16_t width;
    rt_uint16_t height;

    struct lite_gui_font_size font_size;

    void (*clear)(struct lite_gui_dc *dc);

    void (*draw_point)(struct lite_gui_dc *dc, rt_uint16_t x, rt_uint16_t y);

    void (*draw_horizontal_line)(struct lite_gui_dc *dc, rt_uint16_t from_x, rt_uint16_t to_x, rt_uint16_t y);

    void (*draw_vertical_line)(struct lite_gui_dc *dc, rt_uint16_t x, rt_uint16_t from_y, rt_uint16_t to_y);

    void (*draw_line)(struct lite_gui_dc *dc,
                      rt_uint16_t x0, rt_uint16_t y0,
                      rt_uint16_t x1, rt_uint16_t y1);

    void (*draw_rectangle)(struct lite_gui_dc *dc,
                           rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                           rt_uint16_t right_bottom_x, rt_uint16_t right_bottom_y);

    void (*fill_rectangle)(struct lite_gui_dc *dc,
                           rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                           rt_uint16_t right_bottom_x, rt_uint16_t right_bottom_y);

    void (*draw_circle)(struct lite_gui_dc *dc,
                        rt_uint16_t center_x, rt_uint16_t center_y,
                        rt_uint16_t radius);

    void (*fill_circle)(struct lite_gui_dc *dc, rt_uint16_t center_x, rt_uint16_t center_y,
                        rt_uint16_t radius);

    void (*draw_rounded_rectangle)(struct lite_gui_dc *dc,
                                   rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                                   rt_uint16_t right_bottom_x, rt_uint16_t right_bottom_y,
                                   rt_uint16_t round_radius);

    void (*fill_rounded_rectangle)(struct lite_gui_dc *dc,
                                   rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                                   rt_uint16_t right_bottom_x, rt_uint16_t right_bottom_y,
                                   rt_uint16_t round_radius);

    void (*draw_ascii_char)(struct lite_gui_dc *dc,
                            rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                            char ch);

    void (*draw_ascii_string)(struct lite_gui_dc *dc, rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                              char *string);
};

typedef struct lite_gui_dc *lite_gui_dc_t;

lite_gui_dc_t gui_dc_init(lite_gui_dc_t dc);


#endif //LITE_GUI_DC_H
