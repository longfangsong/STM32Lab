#ifndef _EEPROM_BASIC_H
#define _EEPROM_BASIC_H

#include <stm32f10x.h>

#define SDA_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}
#define SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}

#define IIC_GPIO GPIOC
#define IIC_SCL_PIN GPIO_Pin_12
#define IIC_SDA_PIN GPIO_Pin_11

//IIC所有操作函数
void IIC_Init(void);                    //初始化IIC的IO口
void IIC_Start(void);                   //发送IIC开始信号
void IIC_Stop(void);                    //发送IIC停止信号
void IIC_Send_Byte(u8 txd);             //IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);    //IIC读取一个字节
u8 IIC_Wait_Ack(void);                  //IIC等待ACK信号
void IIC_Ack(void);                     //IIC发送ACK信号
void IIC_NAck(void);                    //IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data);

u8 IIC_Read_One_Byte(u8 daddr, u8 addr);

u8 AT24CXX_ReadOneByte(u16 ReadAddr);

void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite);

#endif //_EEPROM_BASIC_H
