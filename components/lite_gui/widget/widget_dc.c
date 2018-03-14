#include "widget_dc.h"

static void clear_default_impl(lite_gui_dc_t dc) {
    lite_gui_dc_t parent = ((lite_gui_widget_dc_t) (dc))->parent;
    rt_int16_t x0 = ((lite_gui_widget_dc_t) (dc))->x_from_parent;
    rt_int16_t y0 = ((lite_gui_widget_dc_t) (dc))->y_from_parent;
    rt_uint16_t parent_origin_color = parent->brush_color;
    rt_uint16_t parent_origin_background_color = parent->background_color;
    parent->brush_color = dc->brush_color;
    parent->background_color = dc->background_color;
    parent->fill_rectangle(dc, (rt_uint16_t) x0, (rt_uint16_t) y0, x0 + dc->width, y0 + dc->height);
    parent->brush_color = parent_origin_color;
    parent->background_color = parent_origin_background_color;
}

static void draw_point_default_impl(lite_gui_dc_t dc, rt_uint16_t x, rt_uint16_t y) {
    lite_gui_dc_t parent = ((lite_gui_widget_dc_t) (dc))->parent;
    rt_uint16_t parent_origin_color = parent->brush_color;
    rt_uint16_t parent_origin_background_color = parent->background_color;
    parent->brush_color = dc->background_color;
    parent->background_color = dc->background_color;
    parent->draw_point(parent,
                       x + ((lite_gui_widget_dc_t) (dc))->x_from_parent,
                       y + ((lite_gui_widget_dc_t) (dc))->y_from_parent);
    parent->brush_color = parent_origin_color;
    parent->background_color = parent_origin_background_color;
}

static void draw_horizontal_line_default_impl(lite_gui_dc_t dc, rt_uint16_t from_x, rt_uint16_t to_x, rt_uint16_t y) {
    lite_gui_dc_t parent = ((lite_gui_widget_dc_t) (dc))->parent;
    rt_int16_t dx = ((lite_gui_widget_dc_t) (dc))->x_from_parent;
    rt_int16_t dy = ((lite_gui_widget_dc_t) (dc))->y_from_parent;
    rt_uint16_t parent_origin_color = parent->brush_color;
    rt_uint16_t parent_origin_background_color = parent->background_color;
    parent->brush_color = dc->brush_color;
    parent->background_color = dc->background_color;
    parent->draw_horizontal_line(parent, from_x + dx, to_x + dx, y + dy);
    parent->brush_color = parent_origin_color;
    parent->background_color = parent_origin_background_color;
}

static void draw_vertical_line_default_impl(lite_gui_dc_t dc, rt_uint16_t x, rt_uint16_t from_y, rt_uint16_t to_y) {
    lite_gui_dc_t parent = ((lite_gui_widget_dc_t) (dc))->parent;
    rt_int16_t dx = ((lite_gui_widget_dc_t) (dc))->x_from_parent;
    rt_int16_t dy = ((lite_gui_widget_dc_t) (dc))->y_from_parent;
    rt_uint16_t parent_origin_color = parent->brush_color;
    rt_uint16_t parent_origin_background_color = parent->background_color;
    parent->brush_color = dc->brush_color;
    parent->background_color = dc->background_color;
    parent->draw_vertical_line(parent, x + dx, from_y + dy, to_y + dy);
    parent->brush_color = parent_origin_color;
    parent->background_color = parent_origin_background_color;
}

static void draw_rectangle_default_impl(lite_gui_dc_t dc,
                                        rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                                        rt_uint16_t right_bottom_x, rt_uint16_t right_bottom_y) {
    lite_gui_dc_t parent = ((lite_gui_widget_dc_t) (dc))->parent;
    rt_int16_t dx = ((lite_gui_widget_dc_t) (dc))->x_from_parent;
    rt_int16_t dy = ((lite_gui_widget_dc_t) (dc))->y_from_parent;
    rt_uint16_t parent_origin_color = parent->brush_color;
    rt_uint16_t parent_origin_background_color = parent->background_color;
    parent->brush_color = dc->brush_color;
    parent->background_color = dc->background_color;
    parent->draw_rectangle(parent, left_top_x + dx, left_top_y + dy, right_bottom_x + dx,
                           right_bottom_y + dy);
    parent->brush_color = parent_origin_color;
    parent->background_color = parent_origin_background_color;
}

static void fill_rectangle_default_impl(lite_gui_dc_t dc,
                                        rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                                        rt_uint16_t right_bottom_x, rt_uint16_t right_bottom_y) {
    lite_gui_dc_t parent = ((lite_gui_widget_dc_t) (dc))->parent;
    rt_int16_t dx = ((lite_gui_widget_dc_t) (dc))->x_from_parent;
    rt_int16_t dy = ((lite_gui_widget_dc_t) (dc))->y_from_parent;
    rt_uint16_t parent_origin_color = parent->brush_color;
    rt_uint16_t parent_origin_background_color = parent->background_color;
    parent->brush_color = dc->brush_color;
    parent->background_color = dc->background_color;
    parent->fill_rectangle(parent, left_top_x + dx, left_top_y + dy, right_bottom_x + dx,
                           right_bottom_y + dy);
    parent->brush_color = parent_origin_color;
    parent->background_color = parent_origin_background_color;
}

static void draw_line_default_impl(lite_gui_dc_t dc,
                                   rt_uint16_t x0, rt_uint16_t y0,
                                   rt_uint16_t x1, rt_uint16_t y1) {
    lite_gui_dc_t parent = ((lite_gui_widget_dc_t) (dc))->parent;
    rt_int16_t dx = ((lite_gui_widget_dc_t) (dc))->x_from_parent;
    rt_int16_t dy = ((lite_gui_widget_dc_t) (dc))->y_from_parent;
    rt_uint16_t parent_origin_color = parent->brush_color;
    rt_uint16_t parent_origin_background_color = parent->background_color;
    parent->brush_color = dc->brush_color;
    parent->background_color = dc->background_color;
    parent->draw_line(parent, x0 + dx, y0 + dy, x1 + dx, y1 + dy);
    parent->brush_color = parent_origin_color;
    parent->background_color = parent_origin_background_color;
}

static void draw_circle_default_impl(lite_gui_dc_t dc, rt_uint16_t center_x, rt_uint16_t center_y,
                                     rt_uint16_t radius) {
    lite_gui_dc_t parent = ((lite_gui_widget_dc_t) (dc))->parent;
    rt_int16_t dx = ((lite_gui_widget_dc_t) (dc))->x_from_parent;
    rt_int16_t dy = ((lite_gui_widget_dc_t) (dc))->y_from_parent;
    rt_uint16_t parent_origin_color = parent->brush_color;
    rt_uint16_t parent_origin_background_color = parent->background_color;
    parent->brush_color = dc->brush_color;
    parent->background_color = dc->background_color;
    parent->draw_circle(parent, center_x + dx, center_y + dy, radius);
    parent->brush_color = parent_origin_color;
    parent->background_color = parent_origin_background_color;
}

static void fill_circle_default_impl(lite_gui_dc_t dc, rt_uint16_t center_x, rt_uint16_t center_y,
                                     rt_uint16_t radius) {
    lite_gui_dc_t parent = ((lite_gui_widget_dc_t) (dc))->parent;
    rt_int16_t dx = ((lite_gui_widget_dc_t) (dc))->x_from_parent;
    rt_int16_t dy = ((lite_gui_widget_dc_t) (dc))->y_from_parent;
    rt_uint16_t parent_origin_color = parent->brush_color;
    rt_uint16_t parent_origin_background_color = parent->background_color;
    parent->brush_color = dc->brush_color;
    parent->background_color = dc->background_color;
    parent->fill_circle(parent, center_x + dx, center_y + dy, radius);
    parent->brush_color = parent_origin_color;
    parent->background_color = parent_origin_background_color;
}

static void draw_rounded_rectangle_default_impl(lite_gui_dc_t dc,
                                                rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                                                rt_uint16_t right_bottom_x, rt_uint16_t right_bottom_y,
                                                rt_uint16_t round_radius) {
    lite_gui_dc_t parent = ((lite_gui_widget_dc_t) (dc))->parent;
    rt_int16_t dx = ((lite_gui_widget_dc_t) (dc))->x_from_parent;
    rt_int16_t dy = ((lite_gui_widget_dc_t) (dc))->y_from_parent;
    rt_uint16_t parent_origin_color = parent->brush_color;
    rt_uint16_t parent_origin_background_color = parent->background_color;
    parent->brush_color = dc->brush_color;
    parent->background_color = dc->background_color;
    parent->draw_rounded_rectangle(parent, left_top_x + dx, left_top_y + dy,
                                   right_bottom_x + dx, right_bottom_y + dy,
                                   round_radius);
    parent->brush_color = parent_origin_color;
    parent->background_color = parent_origin_background_color;
}

static void fill_rounded_rectangle_default_impl(lite_gui_dc_t dc,
                                                rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                                                rt_uint16_t right_bottom_x, rt_uint16_t right_bottom_y,
                                                rt_uint16_t round_radius) {
    lite_gui_dc_t parent = ((lite_gui_widget_dc_t) (dc))->parent;
    rt_int16_t dx = ((lite_gui_widget_dc_t) (dc))->x_from_parent;
    rt_int16_t dy = ((lite_gui_widget_dc_t) (dc))->y_from_parent;
    rt_uint16_t parent_origin_color = parent->brush_color;
    rt_uint16_t parent_origin_background_color = parent->background_color;
    parent->brush_color = dc->brush_color;
    parent->background_color = dc->background_color;
    parent->fill_rounded_rectangle(parent, left_top_x + dx, left_top_y + dy,
                                   right_bottom_x + dx, right_bottom_y + dy,
                                   round_radius);
    parent->brush_color = parent_origin_color;
    parent->background_color = parent_origin_background_color;
}

static void draw_ascii_char_default_impl(lite_gui_dc_t dc,
                                         rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                                         char ch) {
    lite_gui_dc_t parent = ((lite_gui_widget_dc_t) (dc))->parent;
    rt_int16_t dx = ((lite_gui_widget_dc_t) (dc))->x_from_parent;
    rt_int16_t dy = ((lite_gui_widget_dc_t) (dc))->y_from_parent;
    rt_uint16_t parent_origin_color = parent->brush_color;
    rt_uint16_t parent_origin_background_color = parent->background_color;
    parent->brush_color = dc->brush_color;
    parent->background_color = dc->background_color;
    parent->draw_ascii_char(parent, left_top_x + dx, left_top_y + dy, ch);
    parent->brush_color = parent_origin_color;
    parent->background_color = parent_origin_background_color;
}

static void draw_ascii_string_default_impl(lite_gui_dc_t dc, rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                                           char *string) {
    lite_gui_dc_t parent = ((lite_gui_widget_dc_t) (dc))->parent;
    rt_int16_t dx = ((lite_gui_widget_dc_t) (dc))->x_from_parent;
    rt_int16_t dy = ((lite_gui_widget_dc_t) (dc))->y_from_parent;
    rt_uint16_t parent_origin_color = parent->brush_color;
    rt_uint16_t parent_origin_background_color = parent->background_color;
    parent->brush_color = dc->brush_color;
    parent->background_color = dc->background_color;
    parent->draw_ascii_string(parent, left_top_x + dx, left_top_y + dy, string);
    parent->brush_color = parent_origin_color;
    parent->background_color = parent_origin_background_color;
}

lite_gui_widget_dc_t
lite_gui_widget_dc_init(lite_gui_widget_dc_t dc, lite_gui_dc_t parent, rt_int16_t x_from_parent,
                        rt_int16_t y_from_parent,
                        rt_uint16_t width, rt_uint16_t height) {
    gui_dc_init(&(dc->base));
    dc->base.clear = clear_default_impl;
    dc->base.draw_point = draw_point_default_impl;
    dc->base.draw_horizontal_line = draw_horizontal_line_default_impl;
    dc->base.draw_vertical_line = draw_vertical_line_default_impl;
    dc->base.draw_line = draw_line_default_impl;
    dc->base.draw_rectangle = draw_rectangle_default_impl;
    dc->base.fill_rectangle = fill_rectangle_default_impl;
    dc->base.draw_circle = draw_circle_default_impl;
    dc->base.fill_circle = fill_circle_default_impl;
    dc->base.draw_rounded_rectangle = draw_rounded_rectangle_default_impl;
    dc->base.fill_rounded_rectangle = fill_rounded_rectangle_default_impl;
    dc->base.draw_ascii_char = draw_ascii_char_default_impl;
    dc->base.draw_ascii_string = draw_ascii_string_default_impl;
    dc->base.width = width;
    dc->base.height = height;
    dc->parent = parent;
    dc->x_from_parent = x_from_parent;
    dc->y_from_parent = y_from_parent;
    return dc;
}