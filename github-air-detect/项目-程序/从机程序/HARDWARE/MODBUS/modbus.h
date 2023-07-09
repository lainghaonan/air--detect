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
extern void Group_power_control(u8 reg,u8 cmd,u16 dat);  //���ŷ���reg�Ĵ�������Ϊdat
extern void Modbus_Init(void);
extern void Modbus_Event(void);
extern void Modbud_fun3(void);
extern void Modbud_fun6(void);

extern u8 save_flag;

typedef struct
{
 u8 myadd;//���豸�ĵ�ַ
 u8 rcbuf[100]; //MODBUS���ջ�����
 u16 timout;//MODbus�����ݶ���ʱ��	
 u8 recount;//MODbus�˿��Ѿ��յ������ݸ���
 u8 timrun;//MODbus��ʱ���Ƿ��ʱ�ı�־
 u8  reflag;//�յ�һ֡���ݵı�־
 u8 Sendbuf[100]; //MODbus���ͻ�����	

}MODBUS;

typedef struct
{
 u16 CO2;
 u16 TVOC;
 u16 CH20;//��ȩ
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

