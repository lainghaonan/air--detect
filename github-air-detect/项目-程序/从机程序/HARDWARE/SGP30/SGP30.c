#include "SGP30.h"
#include "delay.h"


//**********************************操作函数****************************

//初始化IIC接口
void SGP30_Init(void)
{
	IIC_Init();
	SGP30_ad_write(0x20,0x03);

}

void SGP30_ad_write(u8 a, u8 b)
{
  	IIC_Start();
  	IIC_Send_Byte(SGP30_write); //发送器件地址+写指令
		IIC_Wait_Ack();
  	IIC_Send_Byte(a);		//发送控制字节
		IIC_Wait_Ack();
		IIC_Send_Byte(b);
		IIC_Wait_Ack();
		IIC_Stop();
		delay_ms(100);
}

u32 SGP30_ad_read(void)
{
  	u32 dat;
		u8 crc;
  	IIC_Start();
  	IIC_Send_Byte(SGP30_read); //发送器件地址+读指令
		IIC_Wait_Ack();
  	dat = IIC_Read_Byte(1);
		dat <<= 8;
		dat += IIC_Read_Byte(1);
		crc = IIC_Read_Byte(1); //crc数据，舍去
	  crc = crc;  //为了不让出现编译警告
		dat <<= 8;
		dat += IIC_Read_Byte(1);
		dat <<= 8;
		dat += IIC_Read_Byte(0);
  	IIC_Stop();
  	return(dat);
}
