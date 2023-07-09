#ifndef _modbus_
#define _modbus_

#include "stm32f1xx.h"
#include "include.h"
#define MODBUFLEN 100
void Function_0x06(u16 reg);
void Function_0x03(u16 reg);
void RS485_BYTE(u8 zz);
void RS485_CMD(u8* zz,u8 length);
extern void SetServo_Reg(u8 add,u8 reg,u8 cmd,u16 dat)  ;
extern void GetServo_Reg(u8 add,u8 reg,u8 cmd,u8 rcount);
extern void Group_power_control(u8 reg,u8 cmd,u16 dat);  //将伺服的reg寄存器设置为dat
extern void Modbus_Init(void);
extern void Modbus_Event(void);
extern void Modbud_fun3(void);
extern void Modbud_fun6(void);

extern u8 save_flag;

typedef struct
{
 u8 myadd;//本设备的地址
 u8 rcbuf[200]; //MODBUS接收缓冲区
 u16 timout;//MODbus的数据断续时间	
 u8 recount;//MODbus端口已经收到的数据个数
 u8 timrun;//MODbus定时器是否计时的标志
 u8  reflag;//收到一帧数据的标志
 u8 Sendbuf[100]; //MODbus发送缓冲区	

}MODBUS;

typedef struct
{
  u16 Tempreture;
  u16 Humidity;
	u16 TGS2602_SENOR ;
  u16 CH2O[2] ; 
	u16 PM25[2];    
  u16 CO2[2];
	u16 TVOC[2];
	u8  motor[10];
	u16 alarm_set[5];
}ADC_DATA;
extern void control_handle(void);
extern ADC_DATA senor;
extern MODBUS modbus;
void Mosbus_Init(void);
void Mosbus_Event(void);
#endif

