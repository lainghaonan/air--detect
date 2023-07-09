#ifndef __SHT30_H
#define	__SHT30_H


#include "include.h"

extern void Sht30_Init(void);

extern void Sht30_WriteCmd(uint8_t msb,uint8_t lsb);
extern void Sht30_ReadData(void);
extern void Convert_sht30(void);
extern u16 Temperature ;
extern u16 Humidity ;
extern volatile u8 Buff[6];
extern void SGP30_ad_write(u8 a, u8 b);
u16 IIC_Read_Byte(u8 ack);
extern void SGP30_Init(void);
#endif
