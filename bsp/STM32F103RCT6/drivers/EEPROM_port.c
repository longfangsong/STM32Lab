//
// Created by 龙方淞 on 2018/3/14.
//

#include <stdlib.h>
#include "EEPROM_port.h"
#include "EEPROM_basic.h"

rt_device_t EEPROM;

static rt_err_t EEPROM_init(rt_device_t _) {
    IIC_Init();
    return RT_EOK;
}

static rt_err_t EEPROM_open(rt_device_t dev, rt_uint16_t oflag) {
    return RT_EOK;
}

static rt_err_t EEPROM_close(rt_device_t dev) {
    return RT_EOK;
}

static rt_size_t EEPROM_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size) {
    for (rt_size_t i = 0; i < size; ++i) {
        ((u8 *) buffer)[i] = AT24CXX_ReadOneByte((u16) (pos + i));
    }
    return size;
}

static rt_size_t EEPROM_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size) {
    for (rt_size_t i = 0; i < size; ++i) {
        AT24CXX_WriteOneByte((u16) (pos + i), ((const u8 *) buffer)[i]);
    }
    return size;
}

void EEPROM_hw_init() {
    IIC_Init();
    EEPROM = malloc(sizeof(struct rt_device));
    EEPROM->type = RT_Device_Class_I2CBUS;
    EEPROM->init = EEPROM_init;
    EEPROM->open = EEPROM_open;
    EEPROM->close = EEPROM_close;
    EEPROM->read = EEPROM_read;
    EEPROM->write = EEPROM_write;
}