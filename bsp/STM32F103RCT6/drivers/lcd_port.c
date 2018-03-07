#include <stdlib.h>
#include "lcd_basic.h"
#include "lcd_port.h"


struct rt_device;
typedef struct rt_device *rt_device_t;

rt_err_t lcd_init(rt_device_t dev) { return RT_EOK; }

rt_err_t lcd_open(rt_device_t dev, rt_uint16_t _) {
    LCD_DisplayOn();
    return RT_EOK;
}

rt_err_t lcd_close(rt_device_t dev) {
    LCD_DisplayOff();
    return RT_EOK;
}

rt_err_t lcd_control(rt_device_t dev, int cmd, void *args) {
    switch (cmd) {
        case RTGRAPHIC_CTRL_GET_INFO: {
            struct rt_device_graphic_info *info = args;
            RT_ASSERT(info != RT_NULL);
            info->bits_per_pixel = 16;
            info->pixel_format = RTGRAPHIC_PIXEL_FORMAT_RGB565P;
            info->framebuffer = RT_NULL;
            info->width = lcddev.width;
            info->height = lcddev.height;
        }
            break;
        default:
            break;
    }
    return RT_EOK;
}

rt_err_t lcd_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size) {
    return RT_EOK;
}

rt_err_t lcd_write(rt_device_t dev, rt_off_t pos, const void *buffer,
                   rt_size_t size) {
    return RT_EOK;
}

void rt_hw_lcd_set_pixel(const char *pixel, int x, int y) {
    LCD_Fast_DrawPoint(x, y, *(rt_uint16_t *) pixel);
}

void rt_hw_lcd_get_pixel(char *pixel, int x, int y) {
    *(rt_uint16_t *) pixel = LCD_ReadPoint(x, y);
}

void rt_hw_lcd_draw_hline(const char *pixel, int x1, int x2, int y) {
    rt_kprintf("rt_hw_lcd_draw_hline\n");
    LCD_SetCursor(x1, y);
    LCD_WriteRAM_Prepare();
    for (; x1 < x2; ++x1) {
        LCD_WriteRAM(*(rt_uint16_t *) pixel);
    }
}

void rt_hw_lcd_draw_vline(const char *pixel, int x, int y1, int y2) {
    rt_kprintf("rt_hw_lcd_draw_vline\n");
    LCD_SetCursor(x, y1);
    LCD_WriteRAM_Prepare();
    for (; y1 < y2; ++y1) {
        LCD_WriteRAM(*(rt_uint16_t *) pixel);
    }
}

void rt_hw_lcd_draw_raw_hline(const char *pixels, int x, int y,
                              rt_size_t size) {
    rt_kprintf("rt_hw_lcd_draw_raw_hline\n");
    rt_uint16_t *ptr = (rt_uint16_t *) pixels;
    LCD_SetCursor(x, y);
    LCD_WriteRAM_Prepare();
    for (; size != 0; --size) {
        LCD_WriteRAM(*(ptr++));
    }
}

struct rt_device_graphic_ops lcd_ops = {
        rt_hw_lcd_set_pixel, rt_hw_lcd_get_pixel, rt_hw_lcd_draw_hline,
        rt_hw_lcd_draw_vline, rt_hw_lcd_draw_raw_hline
};

void rt_hw_lcd_init() {
    struct rt_device *lcd_device = malloc(sizeof(struct rt_device));
    lcd_device->type = RT_Device_Class_Graphic;
    lcd_device->init = lcd_init;
    lcd_device->open = lcd_open;
    lcd_device->close = lcd_close;
    lcd_device->read = RT_NULL;
    lcd_device->write = RT_NULL;
    lcd_device->control = lcd_control;
    lcd_device->user_data = &lcd_ops;
    LCD_Init();
    rt_device_register(lcd_device, "lcd",
                       RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);
}
