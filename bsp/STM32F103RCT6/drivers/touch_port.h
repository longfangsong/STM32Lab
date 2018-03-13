//
// Created by 龙方淞 on 2018/3/13.
//

#ifndef _TOUCH_PORT_H
#define _TOUCH_PORT_H

#include <rtm.h>

extern rt_device_t touch_device;

struct touch_event_handler {
    void (*on_press)(u16 x, u16 y, u16 z);

    void (*on_drag)(u16 from_x, u16 from_y, u16 from_z,
                    u16 to_x, u16 to_y, u16 to_z);
};

typedef struct touch_event_handler *touch_event_handler_t;

void touch_init(struct touch_event_handler handler);


void pen_down(u16 x, u16 y, u16 z);

void pen_up();

#endif //_TOUCH_PORT_H
