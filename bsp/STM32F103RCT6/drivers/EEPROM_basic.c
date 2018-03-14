//
// Created by 龙方淞 on 2018/3/14.
//

#include <rthw.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "EEPROM_basic.h"

void IIC_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    //RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN | IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IIC_GPIO, &GPIO_InitStructure);

    GPIO_SetBits(IIC_GPIO, IIC_SCL_PIN);
    GPIO_SetBits(IIC_GPIO, IIC_SDA_PIN);
}

void IIC_Start(void) {
    SDA_OUT();     //sda线输出
    GPIO_SetBits(IIC_GPIO, IIC_SCL_PIN);
    GPIO_SetBits(IIC_GPIO, IIC_SDA_PIN);
    rt_hw_us_delay(4);
    GPIO_ResetBits(IIC_GPIO, IIC_SDA_PIN);
    rt_hw_us_delay(4);
    GPIO_ResetBits(IIC_GPIO, IIC_SCL_PIN);
}

void IIC_Stop(void) {
    SDA_OUT();//sda线输出
    GPIO_ResetBits(IIC_GPIO, IIC_SCL_PIN);
    GPIO_ResetBits(IIC_GPIO, IIC_SDA_PIN);
    rt_hw_us_delay(4);
    GPIO_SetBits(IIC_GPIO, IIC_SCL_PIN);
    GPIO_SetBits(IIC_GPIO, IIC_SDA_PIN);
    rt_hw_us_delay(4);
}

u8 IIC_Wait_Ack(void) {
    u8 ucErrTime = 0;
    SDA_IN();      //SDA设置为输入  
    GPIO_SetBits(IIC_GPIO, IIC_SDA_PIN);
    rt_hw_us_delay(1);
    GPIO_SetBits(IIC_GPIO, IIC_SCL_PIN);
    rt_hw_us_delay(1);
    while (GPIO_ReadInputDataBit(IIC_GPIO, IIC_SDA_PIN)) {
        ucErrTime++;
        if (ucErrTime > 250) {
            IIC_Stop();
            return 1;
        }
    }
    GPIO_ResetBits(IIC_GPIO, IIC_SCL_PIN);//时钟输出0
    return 0;
}

void IIC_Ack(void) {
    GPIO_ResetBits(IIC_GPIO, IIC_SCL_PIN);
    SDA_OUT();
    GPIO_ResetBits(IIC_GPIO, IIC_SDA_PIN);
    rt_hw_us_delay(2);
    GPIO_SetBits(IIC_GPIO, IIC_SCL_PIN);
    rt_hw_us_delay(2);
    GPIO_ResetBits(IIC_GPIO, IIC_SCL_PIN);
}

void IIC_NAck(void) {
    GPIO_ResetBits(IIC_GPIO, IIC_SCL_PIN);
    SDA_OUT();
    GPIO_SetBits(IIC_GPIO, IIC_SDA_PIN);
    rt_hw_us_delay(2);
    GPIO_SetBits(IIC_GPIO, IIC_SCL_PIN);
    rt_hw_us_delay(2);
    GPIO_ResetBits(IIC_GPIO, IIC_SCL_PIN);
}

void IIC_Send_Byte(u8 txd) {
    u8 t;
    SDA_OUT();
    GPIO_ResetBits(IIC_GPIO, IIC_SCL_PIN);//拉低时钟开始数据传输
    for (t = 0; t < 8; t++) {
        GPIO_WriteBit(GPIOC, IIC_SDA_PIN, (BitAction) ((txd & 0x80) >> 7));
        txd <<= 1;
        rt_hw_us_delay(2);   //对TEA5767这三个延时都是必须的
        GPIO_SetBits(IIC_GPIO, IIC_SCL_PIN);
        rt_hw_us_delay(2);
        GPIO_ResetBits(IIC_GPIO, IIC_SCL_PIN);
        rt_hw_us_delay(2);
    }
}

u8 IIC_Read_Byte(unsigned char ack) {
    unsigned char i, receive = 0;
    SDA_IN();//SDA设置为输入
    for (i = 0; i < 8; i++) {
        GPIO_ResetBits(IIC_GPIO, IIC_SCL_PIN);
        rt_hw_us_delay(2);
        GPIO_SetBits(IIC_GPIO, IIC_SCL_PIN);
        receive <<= 1;
        if (GPIO_ReadInputDataBit(IIC_GPIO, IIC_SDA_PIN))receive++;
        rt_hw_us_delay(1);
    }
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址
//返回值  :读到的数据
u8 AT24CXX_ReadOneByte(u16 ReadAddr) {
    u8 temp = 0;
    IIC_Start();
    IIC_Send_Byte(0XA0 + ((ReadAddr / 256) << 1));   //发送器件地址0XA0,写数据
    IIC_Wait_Ack();
    IIC_Send_Byte(ReadAddr % 256);   //发送低地址
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(0XA1);           //进入接收模式
    IIC_Wait_Ack();
    temp = IIC_Read_Byte(0);
    IIC_Stop();//产生一个停止条件
    return temp;
}

void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite) {
    IIC_Start();
    IIC_Send_Byte(0XA0 + ((WriteAddr / 256) << 1));   //发送器件地址0XA0,写数据
    IIC_Wait_Ack();
    IIC_Send_Byte(WriteAddr % 256);   //发送低地址
    IIC_Wait_Ack();
    IIC_Send_Byte(DataToWrite);     //发送字节
    IIC_Wait_Ack();
    IIC_Stop();//产生一个停止条件
    rt_thread_delay(1);
}