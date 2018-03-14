//
// Created by 龙方淞 on 2018/3/13.
//

#include <stdlib.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_exti.h>
#include "touch_port.h"
#include "touch_basic.h"

rt_device_t touch_device;

static rt_err_t touch_init_imp(rt_device_t dev) {
    touch_hw_init();
    return RT_EOK;
}

static rt_err_t touch_open_imp(rt_device_t dev, rt_uint16_t _) {
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    return RT_EOK;
}

static rt_err_t touch_close_imp(rt_device_t dev) {
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
    return RT_EOK;
}

#define UNINIT 0
#define CLICK 1
#define DRAG 2
static u8 state = UNINIT;
static u8 drag_vote = 0;
#define HISTORY_SIZE 5
static u16 pen_history[HISTORY_SIZE][3];

static int abs(int x) {
    return x >= 0 ? x : -x;
}


void pen_down(u16 x, u16 y, u16 z) {
    rt_kprintf("r:%d,%d,%d\n", x, y, z);
    if (state == UNINIT) {
        for (u8 i = 0; i < HISTORY_SIZE; ++i) {
            pen_history[i][0] = x;
            pen_history[i][1] = y;
            pen_history[i][2] = z;
        }
        state = CLICK;
    }
    for (u8 i = HISTORY_SIZE - 1; i > 0; --i) {
        pen_history[i][0] = pen_history[i - 1][0];
        pen_history[i][1] = pen_history[i - 1][1];
        pen_history[i][2] = pen_history[i - 1][2];
    }
    pen_history[0][0] = x;
    pen_history[0][1] = y;
    pen_history[0][2] = z;
    if (state == CLICK && (abs(pen_history[0][0] - pen_history[1][0]) > 5 ||
                           abs(pen_history[0][1] - pen_history[1][1]) > 5)
        && (abs(pen_history[1][0] - pen_history[2][0]) > 3 ||
            abs(pen_history[1][1] - pen_history[2][1]) > 3)) {
        ++drag_vote;
        if (drag_vote > 5)
            state = DRAG;
    } else if (state == DRAG) {
        ((touch_event_handler_t) (touch_device->user_data))->on_drag(pen_history[0][0],
                                                                     pen_history[0][1],
                                                                     pen_history[0][2],
                                                                     pen_history[1][0],
                                                                     pen_history[1][1],
                                                                     pen_history[1][2]);
    }
}

void pen_up() {
    if (state == CLICK) {
        ((touch_event_handler_t) (touch_device->user_data))->on_press(pen_history[0][0],
                                                                      pen_history[0][1],
                                                                      pen_history[0][2]);
    }
    state = UNINIT;
    drag_vote = 0;
}

void touch_init(struct touch_event_handler handler) {
    touch_hw_init();
    touch_device = malloc(sizeof(struct rt_device));
    touch_device->type = RT_Device_Class_SPIDevice;
    touch_device->init = touch_init_imp;
    touch_device->open = touch_open_imp;
    touch_device->close = touch_close_imp;
    touch_device->read = RT_NULL;
    touch_device->write = RT_NULL;
    touch_device->control = RT_NULL;
    touch_event_handler_t handler_ = malloc(sizeof(struct touch_event_handler));
    *handler_ = handler;
    touch_device->user_data = handler_;
}