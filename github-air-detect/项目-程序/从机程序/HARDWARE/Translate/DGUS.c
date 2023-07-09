#include "DGUS.h"
#include "include.h"
#include "sys.h"
u8 Usart_data[200];

u8 Sendbuf[100];

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
//DGUS 发送一个字节函数
void TTL_byte(u8 zz)
{
	HAL_UART_Transmit(&huart1,(u8 *)&zz,1,0);
	while((USART1->SR&0X40)==0);    
}

//DGUS 发送处理函数
void senor_translate(u16 add,u16 x)
{
	int j;
	u8 BUF[10];
	BUF[0]=0x5A;
	BUF[1]=0xA5;
	BUF[2]=0x05;
	BUF[3]=0x82;
	BUF[4]=add/256;
	BUF[5]=add%256;
	BUF[6]=x/256;
	BUF[7]=x%256;
	for(j=0;j<8;j++)										
	{
	 TTL_byte(BUF[j]);     
	}

}
//亮度设置
void Light_Set(void)//x 可以找几组值 作为亮度调节的时间节点
{
	int j;
	const u8 xx[4]={20,50,80,100};
	static u8 i=0;
	i++;
	if(i>3) i=0;
	Sendbuf[0]=0x5A;
	Sendbuf[1]=0xA5;
	Sendbuf[2]=0x07;
	Sendbuf[3]=0x82;
  Sendbuf[4]=0x00;
	Sendbuf[5]=0x82; 
	Sendbuf[6]=xx[i];   //开启亮度
  Sendbuf[7]=0x00;//背光亮度
	Sendbuf[8]=0xFF;
	Sendbuf[9]=0xFF;//(Sendbuf[8]*256+Sendbuf[9])* 5ms
	for(j=0;j<10;j++)	
	{
	 TTL_byte(Sendbuf[j]);     
	}
}



