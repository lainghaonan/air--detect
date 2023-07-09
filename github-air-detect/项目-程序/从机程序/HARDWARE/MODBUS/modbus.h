#ifndef _modbus_
#define _modbus_

#include "stm32f1xx.h"
#include "include.h"
#define MODBUFLEN 100
void Function_0x06(u16 reg);
void Function_0x03(u16 reg);
void RS485_BYTE(u8 zz);
void RS485_CMD(u8* zz,u8 length);
extern void SetServo_Reg(u8 reg,u8 cmd,u16 dat) ;
extern void GetServo_Reg(u8 reg,u8 cmd,u8 rcount);
extern void Group_power_control(u8 reg,u8 cmd,u16 dat);  //将伺服的reg寄存器设置为dat
extern void Modbus_Init(void);
extern void Modbus_Event(void);
extern void Modbud_fun3(void);
extern void Modbud_fun6(void);

extern u8 save_flag;

typedef struct
{
 u8 myadd;//本设备的地址
 u8 rcbuf[100]; //MODBUS接收缓冲区
 u16 timout;//MODbus的数据断续时间	
 u8 recount;//MODbus端口已经收到的数据个数
 u8 timrun;//MODbus定时器是否计时的标志
 u8  reflag;//收到一帧数据的标志
 u8 Sendbuf[100]; //MODbus发送缓冲区	

}MODBUS;

typedef struct
{
 u16 CO2;
 u16 TVOC;
 u16 CH20;//甲醛
 u16 PW2_5;
 u16 HUM;
 u16 TEM;
	
}GAS;

extern GAS gas;
extern void Sensor_Event() ;
extern MODBUS modbus1;
extern MODBUS modbus;
void Mosbus_Init(void);
void Mosbus_Event(void);
void type_BYTE(u8 zz);
void senor_get(void);
#endif

