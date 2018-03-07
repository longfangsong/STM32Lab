/**
 ******************************************************************************
 * @file    SDIO/sdcard.h
 * @author  MCD Application Team
 * @version V3.1.2
 * @date    09/28/2009
 * @brief   This file contains all the functions prototypes for the SD Card
 *          driver firmware library.
 ******************************************************************************
 * @copy
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
 */

/** @addtogroup STM32F10x_StdPeriph_Examples
 * @{
 */

/** @addtogroup SDIO_Example
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDCARD_H
#define __SDCARD_H
#include <stm32f10x.h>
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
// ALIENTEK MiniSTM32开发板
// SD卡 驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/13
//版本：V1.0
//版权所有，盗版必究。
// Copyright(C) 广州市星翼电子科技有限公司 2009-2019
// All rights reserved
//////////////////////////////////////////////////////////////////////////////////

// SD卡类型定义
#define SD_TYPE_ERR 0X00
#define SD_TYPE_MMC 0X01
#define SD_TYPE_V1 0X02
#define SD_TYPE_V2 0X04
#define SD_TYPE_V2HC 0X06
// SD卡指令表
#define CMD0 0 //卡复位
#define CMD1 1
#define CMD8 8   //命令8 ，SEND_IF_COND
#define CMD9 9   //命令9 ，读CSD数据
#define CMD10 10 //命令10，读CID数据
#define CMD12 12 //命令12，停止数据传输
#define CMD16 16 //命令16，设置SectorSize 应返回0x00
#define CMD17 17 //命令17，读sector
#define CMD18 18 //命令18，读Multi sector
#define CMD23 23 //命令23，设置多sector写入前预先擦除N个block
#define CMD24 24 //命令24，写sector
#define CMD25 25 //命令25，写Multi sector
#define CMD41 41 //命令41，应返回0x00
#define CMD55 55 //命令55，应返回0x01
#define CMD58 58 //命令58，读OCR信息
#define CMD59 59 //命令59，使能/禁止CRC，应返回0x00
//数据写入回应字意义
#define MSD_DATA_OK 0x05
#define MSD_DATA_CRC_ERROR 0x0B
#define MSD_DATA_WRITE_ERROR 0x0D
#define MSD_DATA_OTHER_ERROR 0xFF
// SD卡回应标记字
#define MSD_RESPONSE_NO_ERROR 0x00
#define MSD_IN_IDLE_STATE 0x01
#define MSD_ERASE_RESET 0x02
#define MSD_ILLEGAL_COMMAND 0x04
#define MSD_COM_CRC_ERROR 0x08
#define MSD_ERASE_SEQUENCE_ERROR 0x10
#define MSD_ADDRESS_ERROR 0x20
#define MSD_PARAMETER_ERROR 0x40
#define MSD_RESPONSE_FAILURE 0xFF

//这部分应根据具体的连线来修改!
// MiniSTM32开发板使用的是PA3作为SD卡的CS脚.

extern u8 SD_Type; // SD卡的类型

//函数声明区
u8 SD_SPI_ReadWriteByte(u8 data);
void SD_SPI_SpeedLow(void);
void SD_SPI_SpeedHigh(void);
u8 SD_WaitReady(void);                        //等待SD卡准备
u8 SD_GetResponse(u8 Response);               //获得响应
u8 SD_Initialize(void);                       //初始化
u8 SD_ReadDisk(u8 *buf, u32 sector, u8 cnt);  //读块
u8 SD_WriteDisk(u8 *buf, u32 sector, u8 cnt); //写块
u32 SD_GetSectorCount(void);                  //读扇区数
u8 SD_GetCID(u8 *cid_data);                   //读SD卡CID
u8 SD_GetCSD(u8 *csd_data);                   //读SD卡CSD

#endif /* __SDCARD_H */

/**
 * @}
 */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
