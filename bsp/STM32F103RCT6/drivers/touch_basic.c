#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_exti.h>
#include <rtm.h>
#include <misc.h>
#include <rthw.h>
#include <lite_gui.h>
#include "touch_basic.h"
#include "touch_port.h"
#include "lcd_basic.h"

#define TOUCH_GPIO GPIOC
#define PEN_PIN GPIO_Pin_1
#define MISO_PIN GPIO_Pin_2
#define MOSI_PIN GPIO_Pin_3
#define CLK_PIN GPIO_Pin_0
#define CS_PIN GPIO_Pin_13

static int abs(int x) {
    return x >= 0 ? x : -x;
}

rt_event_t touch_event;

void (*pen_down_handler)(u16 x, u16 y, u16 z);

void (*pen_up_handler)();

#define READ_X 0XD0
#define READ_Y 0X90
// Z1: 力量越大读数越大
#define READ_Z1 0XB0
// Z2: 力量越大读数越小
#define READ_Z2 0XC0
#define X_SIZE 240
#define Y_SIZE 320

static void __touch_init() {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = MOSI_PIN | CLK_PIN | CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TOUCH_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = PEN_PIN | MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(TOUCH_GPIO, &GPIO_InitStructure);
}

#define PEN_DOWN_EVENT (1 << 0)
#define PEN_UP_EVENT (1 << 1)

static u16 read_value_with_check(u8 read_what);

double x_fact, x_offset;
double y_fact, y_offset;

static u16 read_raw_value_of(u8 read_what);

void touch_calibrate(lite_gui_device_dc_t dc) {
    EXTI->IMR &= 0xFFFFFFFD;
    u16 lt_x, lt_y;
    u16 rt_x, rt_y;
    u16 lb_x, lb_y;
    u16 dc_origin_background = dc->base.background_color;
    dc->base.brush_color = rgb(255, 0, 0);
    dc->base.background_color = rgb(255, 255, 255);
    dc->base.clear((struct lite_gui_dc *) dc);
    dc->base.fill_circle((struct lite_gui_dc *) dc, 20, 20, 2);
    dc->base.draw_circle((struct lite_gui_dc *) dc, 20, 20, 5);
    dc->base.draw_horizontal_line((struct lite_gui_dc *) dc, 10, 30, 20);
    dc->base.draw_vertical_line((struct lite_gui_dc *) dc, 20, 10, 30);
    while (GPIO_ReadInputDataBit(TOUCH_GPIO, PEN_PIN));
    rt_thread_delay(1);
    lt_x = read_raw_value_of(READ_X);
    lt_y = read_raw_value_of(READ_Y);
    while (!GPIO_ReadInputDataBit(TOUCH_GPIO, PEN_PIN));
    rt_thread_delay(1);
    dc->base.clear((struct lite_gui_dc *) dc);
    dc->base.fill_circle((struct lite_gui_dc *) dc, 220, 20, 2);
    dc->base.draw_circle((struct lite_gui_dc *) dc, 220, 20, 5);
    dc->base.draw_horizontal_line((struct lite_gui_dc *) dc, 210, 230, 20);
    dc->base.draw_vertical_line((struct lite_gui_dc *) dc, 220, 10, 30);
    while (GPIO_ReadInputDataBit(TOUCH_GPIO, PEN_PIN));
    rt_thread_delay(1);
    rt_x = read_raw_value_of(READ_X);
    rt_y = read_raw_value_of(READ_Y);
    while (!GPIO_ReadInputDataBit(TOUCH_GPIO, PEN_PIN));
    rt_thread_delay(1);
    dc->base.clear((struct lite_gui_dc *) dc);
    dc->base.fill_circle((struct lite_gui_dc *) dc, 20, 300, 2);
    dc->base.draw_circle((struct lite_gui_dc *) dc, 20, 300, 5);
    dc->base.draw_horizontal_line((struct lite_gui_dc *) dc, 10, 30, 300);
    dc->base.draw_vertical_line((struct lite_gui_dc *) dc, 20, 290, 310);
    while (GPIO_ReadInputDataBit(TOUCH_GPIO, PEN_PIN));
    rt_thread_delay(1);
    lb_x = read_raw_value_of(READ_X);
    lb_y = read_raw_value_of(READ_Y);
    while (!GPIO_ReadInputDataBit(TOUCH_GPIO, PEN_PIN));
    x_fact = 200.0 / (rt_x - lt_x);
    y_fact = 280.0 / (lb_y - lt_y);
    x_offset = 20 * (11 * lt_x - rt_x) / (lt_x - rt_x);
    y_offset = 20 * (15 * lt_y - lb_y) / (lt_y - lb_y);
    dc->base.brush_color = dc_origin_background;
    dc->base.clear((struct lite_gui_dc *) dc);
    EXTI->IMR = 0xFFFFFFFF;
}

static void process_pen(void *parameter) {
    while (1) {
        rt_uint32_t event;
        if (rt_event_recv(touch_event, PEN_DOWN_EVENT | PEN_UP_EVENT,
                          RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER,
                          &event) == RT_EOK) {
            if (event == PEN_DOWN_EVENT) {
                u16 x = read_value_with_check(READ_X);
                u16 y = read_value_with_check(READ_Y);
                u16 z = read_value_with_check(READ_Z2) - read_value_with_check(READ_Z1);
                pen_down(x, y, z);
            } else {
                pen_up();
            }
        }
    }
}

static void pen_interrupt_init() {
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    touch_event = rt_event_create("touch", RT_IPC_FLAG_FIFO);
    if (touch_event != RT_NULL) {
        rt_thread_startup(rt_thread_create("pen_waiting",
                                           process_pen, /* 线程入口是thread1_entry */
                                           RT_NULL, /* 入口参数是RT_NULL */
                                           1024, 10, 1));
    } else {
        rt_kprintf("Fuck");
    }
}

void touch_hw_init() {
    __touch_init();
    pen_interrupt_init();
}

static void write_byte(u8 byte) {
    u8 count = 0;
    for (count = 0; count < 8; count++) {
        if (byte & 0x80) {
            GPIO_SetBits(TOUCH_GPIO, MOSI_PIN);
        } else {
            GPIO_ResetBits(TOUCH_GPIO, MOSI_PIN);
        }
        byte <<= 1;
        GPIO_ResetBits(TOUCH_GPIO, CLK_PIN);
        GPIO_SetBits(TOUCH_GPIO, CLK_PIN);
    }
}

inline bool screen_touched() {
    return GPIO_ReadInputDataBit(TOUCH_GPIO, PEN_PIN) == 0;
}

void rt_hw_us_delay(rt_uint32_t us) {
    rt_uint32_t delta;

    /* 获得延时经过的tick数 */
    us = us * (SysTick->LOAD / (1000000 / RT_TICK_PER_SECOND));

    /* 获得当前时间 */
    delta = SysTick->VAL;

    /* 循环获得当前时间，直到达到指定的时间后退出循环 */
    while (delta - SysTick->VAL < us);
}

static u16 read_raw_value_of(u8 read_what) {
    u16 result = 0;
    GPIO_ResetBits(GPIOC, CLK_PIN);
    GPIO_ResetBits(GPIOC, MOSI_PIN);
    GPIO_ResetBits(GPIOC, CS_PIN);
    write_byte(read_what);
    rt_hw_us_delay(6);//ADS7846的转换时间最长为6us
    GPIO_ResetBits(GPIOC, CLK_PIN);
    rt_hw_us_delay(1);
    GPIO_SetBits(GPIOC, CLK_PIN);
    GPIO_ResetBits(GPIOC, CLK_PIN);
    for (u8 count = 0; count < 16; count++) {
        result <<= 1;
        GPIO_ResetBits(GPIOC, CLK_PIN);
        GPIO_SetBits(GPIOC, CLK_PIN);
        if (GPIO_ReadInputDataBit(GPIOC, MISO_PIN))
            result++;
    }
    result >>= 4;
    GPIO_SetBits(GPIOC, CS_PIN);
    return result;
}

static u16 read_value_of(u8 read_what) {
    u16 result = read_raw_value_of(read_what);
    if (read_what == READ_X) {
        if (result * x_fact + x_offset < 0) {
            return 0;
        }
        result = (u16) (result * x_fact + x_offset);
    } else if (read_what == READ_Y) {
        if (result * y_fact + y_offset < 0) {
            return 0;
        }
        result = (u16) (result * y_fact + y_offset);
    }
    return result;
}

#define SURENCE 50
#define TRY_COUNT_BEFORE_FAIL 10

static u16 read_value_with_check(u8 read_what) {
    u16 first, second;
    u8 try_count = 0;
    do {
        first = read_value_of(read_what);
        second = read_value_of(read_what);
        ++try_count;
        if (try_count >= TRY_COUNT_BEFORE_FAIL) {
            return (u16) -1;
        }
    } while (abs(first - second) > SURENCE);
    return (u16) ((first + second) / 2);
}

void EXTI1_IRQHandler(void) {
    /* enter interrupt */
    rt_interrupt_enter();
    if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
        rt_base_t interrupt_state = rt_hw_interrupt_disable();
        rt_hw_us_delay(100);
        rt_event_send(touch_event, 1U << GPIO_ReadInputDataBit(TOUCH_GPIO, PEN_PIN));
        rt_hw_interrupt_enable(interrupt_state);
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
    /* leave interrupt */
    rt_interrupt_leave();
}
