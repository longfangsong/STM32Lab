#include "dc.h"

int abs(int a) {
    return a >= 0 ? a : -a;
}

static void clear_default_impl(lite_gui_dc_t dc) {
    rt_uint16_t origin_brush_color = dc->brush_color;
    dc->brush_color = dc->background_color;
    for (rt_uint16_t i = 0; i < dc->width; ++i) {
        for (rt_uint16_t j = 0; j < dc->height; ++j) {
            dc->draw_point(dc, i, j);
        }
    }
    dc->brush_color = origin_brush_color;
}

static void draw_horizontal_line_default_impl(lite_gui_dc_t dc, rt_uint16_t from_x, rt_uint16_t to_x, rt_uint16_t y) {
    if (from_x > to_x) {
        draw_horizontal_line_default_impl(dc, to_x, from_x, y);
    } else {
        for (rt_uint16_t i = from_x; i <= to_x; ++i) {
            dc->draw_point(dc, i, y);
        }
    }
}

static void
draw_vertical_line_default_impl(lite_gui_dc_t dc, rt_uint16_t x, rt_uint16_t from_y, rt_uint16_t to_y) {
    if (from_y > to_y) {
        draw_vertical_line_default_impl(dc, x, to_y, from_y);
    } else {
        for (rt_uint16_t i = from_y; i <= to_y; ++i) {
            dc->draw_point(dc, x, i);
        }
    }
}

static void draw_rectangle_impl(lite_gui_dc_t dc,
                                rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                                rt_uint16_t right_bottom_x, rt_uint16_t right_bottom_y) {
    dc->draw_horizontal_line(dc, left_top_x, right_bottom_x, left_top_y);
    dc->draw_vertical_line(dc, left_top_x, left_top_y, right_bottom_y);
    dc->draw_vertical_line(dc, right_bottom_x, left_top_y, right_bottom_y);
    dc->draw_horizontal_line(dc, left_top_x, right_bottom_x, right_bottom_y);
}

static void fill_rectangle_default_impl(lite_gui_dc_t dc,
                                        rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                                        rt_uint16_t right_bottom_x, rt_uint16_t right_bottom_y) {
    for (rt_uint16_t i = left_top_x; i <= right_bottom_x; ++i) {
        for (rt_uint16_t j = left_top_y; j <= right_bottom_y; ++j) {
            dc->draw_point(dc, i, j);
        }
    }
}

static void draw_line_default_impl(lite_gui_dc_t dc,
                                   rt_uint16_t x0, rt_uint16_t y0,
                                   rt_uint16_t x1, rt_uint16_t y1) {
    if (x0 == x1) {
        dc->draw_vertical_line(dc, x0, y0, y1);
        return;
    } else if (y0 == y1) {
        dc->draw_horizontal_line(dc, x0, x1, y0);
        return;
    }
    int dx = abs((int) x1 - (int) x0), sx = ((int) x0 < (int) x1 ? 1 : -1);
    int dy = -abs((int) y1 - (int) y0), sy = ((int) y0 < (int) y1 ? 1 : -1);
    int err = dx + dy, e2; /* error value e_xy */
    while (1) {
        dc->draw_point(dc, x0, y0);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        } /* e_xy+e_x > 0 */
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        } /* e_xy+e_y < 0 */
    }
}

static void draw_circle_part(lite_gui_dc_t dc, rt_uint16_t center_x, rt_uint16_t center_y,
                             rt_uint16_t radius, rt_uint8_t quadrant) {
    int r = radius;
    int x = -r, y = 0, err = 2 - 2 * r; /* II. Quadrant */
    do {
        switch (quadrant) {
            case 1:
                dc->draw_point(dc, (rt_uint16_t) (center_x - x), (rt_uint16_t) (center_y + y));
                break;
            case 2:
                dc->draw_point(dc, (rt_uint16_t) (center_x - y), (rt_uint16_t) (center_y - x));
                break;
            case 3:
                dc->draw_point(dc, (rt_uint16_t) (center_x + x), (rt_uint16_t) (center_y - y));
                break;
            case 4:
                dc->draw_point(dc, (rt_uint16_t) (center_x + y), (rt_uint16_t) (center_y + x));
                break;
            default:
                RT_ASSERT(0);
                break;
        }
        r = err;
        if (r <= y) err += ++y * 2 + 1;           /* e_xy+e_y < 0 */
        if (r > x || err > y) err += ++x * 2 + 1; /* e_xy+e_x > 0 or no 2nd y-step */
    } while (x < 0);
}

static void fill_circle_part(lite_gui_dc_t dc, rt_uint16_t center_x, rt_uint16_t center_y,
                             rt_uint16_t radius, rt_uint8_t quadrant) {
    int r = radius;
    int x = -r, y = 0, err = 2 - 2 * r; /* II. Quadrant */
    do {
        switch (quadrant) {
            case 1:
                dc->draw_horizontal_line(dc, (rt_uint16_t) (center_x - x), center_x, (rt_uint16_t) (center_y + y));
                break;
            case 2:
                dc->draw_horizontal_line(dc, (rt_uint16_t) (center_x - y), center_x, (rt_uint16_t) (center_y - x));
                break;
            case 3:
                dc->draw_horizontal_line(dc, (rt_uint16_t) (center_x + x), center_x, (rt_uint16_t) (center_y - y));
                break;
            case 4:
                dc->draw_horizontal_line(dc, (rt_uint16_t) (center_x + y), center_x, (rt_uint16_t) (center_y + x));
                break;
            default:
                RT_ASSERT(0);
                break;
        }
        r = err;
        if (r <= y) err += ++y * 2 + 1;           /* e_xy+e_y < 0 */
        if (r > x || err > y) err += ++x * 2 + 1; /* e_xy+e_x > 0 or no 2nd y-step */
    } while (x < 0);
}

static void draw_circle_default_impl(lite_gui_dc_t dc, rt_uint16_t center_x, rt_uint16_t center_y,
                                     rt_uint16_t radius) {
    int r = radius;
    int x = -r, y = 0, err = 2 - 2 * r; /* II. Quadrant */
    do {
        dc->draw_point(dc, (rt_uint16_t) (center_x - x), (rt_uint16_t) (center_y + y));
        dc->draw_point(dc, (rt_uint16_t) (center_x - y), (rt_uint16_t) (center_y - x));
        dc->draw_point(dc, (rt_uint16_t) (center_x + x), (rt_uint16_t) (center_y - y));
        dc->draw_point(dc, (rt_uint16_t) (center_x + y), (rt_uint16_t) (center_y + x));
        r = err;
        if (r <= y) err += ++y * 2 + 1;           /* e_xy+e_y < 0 */
        if (r > x || err > y) err += ++x * 2 + 1; /* e_xy+e_x > 0 or no 2nd y-step */
    } while (x < 0);
}

static void fill_circle_default_impl(lite_gui_dc_t dc, rt_uint16_t center_x, rt_uint16_t center_y,
                                     rt_uint16_t radius) {
    int r = radius;
    int x = -r, y = 0, err = 2 - 2 * r; /* II. Quadrant */
    do {
        dc->draw_horizontal_line(dc, (rt_uint16_t) (center_x - x), (rt_uint16_t) (center_x + x),
                                 (rt_uint16_t) (center_y + y));
        dc->draw_horizontal_line(dc, (rt_uint16_t) (center_x - x), (rt_uint16_t) (center_x + x),
                                 (rt_uint16_t) (center_y - y));
        r = err;
        if (r <= y) err += ++y * 2 + 1;           /* e_xy+e_y < 0 */
        if (r > x || err > y) err += ++x * 2 + 1; /* e_xy+e_x > 0 or no 2nd y-step */
    } while (x < 0);
}

static void draw_rounded_rectangle_default_impl(lite_gui_dc_t dc,
                                                rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                                                rt_uint16_t right_bottom_x, rt_uint16_t right_bottom_y,
                                                rt_uint16_t round_radius) {
    rt_uint16_t l_top_x = (rt_uint16_t) ((int) left_top_x + (int) round_radius);
    rt_uint16_t l_top_y = (rt_uint16_t) ((int) left_top_y + (int) round_radius);
    rt_uint16_t r_btm_x = (rt_uint16_t) ((int) right_bottom_x - (int) round_radius);
    rt_uint16_t r_btm_y = (rt_uint16_t) ((int) right_bottom_y - (int) round_radius);
    dc->draw_horizontal_line(dc, l_top_x, r_btm_x, left_top_y);
    dc->draw_vertical_line(dc, left_top_x, l_top_y, r_btm_y);
    dc->draw_vertical_line(dc, right_bottom_x, l_top_y, r_btm_y);
    dc->draw_horizontal_line(dc, l_top_x, r_btm_x, right_bottom_y);
    draw_circle_part(dc, l_top_x, l_top_y, round_radius, 3);
    draw_circle_part(dc, r_btm_x, l_top_y, round_radius, 4);
    draw_circle_part(dc, l_top_x, r_btm_y, round_radius, 2);
    draw_circle_part(dc, r_btm_x, r_btm_y, round_radius, 1);
}

static void fill_rounded_rectangle_default_impl(lite_gui_dc_t dc,
                                                rt_uint16_t left_top_x, rt_uint16_t left_top_y,
                                                rt_uint16_t right_bottom_x, rt_uint16_t right_bottom_y,
                                                rt_uint16_t round_radius) {
    rt_uint16_t l_top_x = (rt_uint16_t) ((int) left_top_x + (int) round_radius);
    rt_uint16_t l_top_y = (rt_uint16_t) ((int) left_top_y + (int) round_radius);
    rt_uint16_t r_btm_x = (rt_uint16_t) ((int) right_bottom_x - (int) round_radius);
    rt_uint16_t r_btm_y = (rt_uint16_t) ((int) right_bottom_y - (int) round_radius);
    dc->fill_rectangle(dc, l_top_x, left_top_y, r_btm_x, right_bottom_y);
    dc->fill_rectangle(dc, left_top_x, l_top_y, l_top_x, r_btm_y);
    dc->fill_rectangle(dc, r_btm_x, l_top_y, right_bottom_x, r_btm_y);
    fill_circle_part(dc, l_top_x, l_top_y, round_radius, 3);
    fill_circle_part(dc, r_btm_x, l_top_y, round_radius, 4);
    fill_circle_part(dc, l_top_x, r_btm_y, round_radius, 2);
    fill_circle_part(dc, r_btm_x, r_btm_y, round_radius, 1);
}

lite_gui_dc_t gui_dc_init(lite_gui_dc_t dc) {
    dc->clear = clear_default_impl;
    dc->draw_horizontal_line = draw_horizontal_line_default_impl;
    dc->draw_vertical_line = draw_vertical_line_default_impl;
    dc->draw_rectangle = draw_rectangle_impl;
    dc->fill_rectangle = fill_rectangle_default_impl;
    dc->draw_line = draw_line_default_impl;
    dc->draw_circle = draw_circle_default_impl;
    dc->fill_circle = fill_circle_default_impl;
    dc->draw_rounded_rectangle = draw_rounded_rectangle_default_impl;
    dc->fill_rounded_rectangle = fill_rounded_rectangle_default_impl;
    return dc;
}
