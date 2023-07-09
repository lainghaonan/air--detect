#include "interrupt.h"
#include "include.h"
#include "sys.h"
//这是用来存放各种终端处理的程序
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
IWDG_HandleTypeDef hiwdg;


UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;



//extern IWDG_HandleTypeDef IWDG_Handler;

u8 RC_BUF[10];
u16 ADC_ConvertedValue=0;
float ADC_ConvertedValueLocal=0,R0,RS=0;
float voltage=0.85,air,error_air=0,now_air;
//ADC中断
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
}  
//定时器中断
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	u8 st;
	static u16 cnt=0,cnt2=0,cnt3=0;
	  if(htim==(&htim3))
    {
         HAL_UART_Receive_IT(&huart2,&RC_BUF[0],1);
	       HAL_UART_Receive_IT(&huart1,&RC_BUF[0],1);
			  if(modbus.timrun!=0)
				{
					modbus.timout++; 
					if(modbus.timout>=8)  //间隔时间达到了时间
					{
						modbus.timrun=0;//关闭定时器--停止定时
						modbus.reflag=1;  //收到一帧数据
					}
				} 	
				if(modbus1.timrun!=0)
				{
					modbus1.timout++; 
					if(modbus1.timout>=8)  //间隔时间达到了时间
					{
						modbus1.timrun=0;//关闭定时器--停止定时
						modbus1.reflag=1;  //收到一帧数据
					}
				} 
				
    }
	if(htim==(&htim4))
	{
		senor_get();  
	
	 }
			
}	
unsigned char Rx_end_flag=0; 
u8 Usart_data[200];
volatile u16 PM2_5_VALUE;
u8 PM25_H;
u8 PM25_L;
u16 PM10_VALUE;
u16 Z05_VALUE;
 u16 VOC_VALUE;

//02 03    0C     03 75      04 18   01 C7    00 27    5C 26   68 3D     DA 20 
//			数据长度（CO2数值）（TVOC）  (CH20)   (PM2.5)  (湿度)  （温度）（校验）
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{
	u8 sbuf;
	static u8 ii=0;
	if(huart==&huart2)//串口二中断   A2  ->TX A3 ->RX  
	{ 
		sbuf =USART2->DR;
    if(modbus1.reflag == 1)
		{
			HAL_UART_Receive_IT(&huart2,&RC_BUF[0],1);
			return;
		}
	    modbus1.rcbuf[modbus1.recount ++]=sbuf;
		modbus1.timout =0;
		if(modbus1.recount == 1)
		{
			 modbus1.timrun=1;
		}
		HAL_UART_Receive_IT(&huart2,&RC_BUF[0],1);
	}
	if(huart==&huart1)//串口一中断  A9->TX   A10 ->RX 
	{ 
    sbuf =USART1->DR;
	   if(modbus.reflag == 1)
		{
			return;
		}
	  modbus.rcbuf[modbus.recount ++]=sbuf;
		modbus.timout =0;
		if(modbus.recount == 1)
		{
			 modbus.timrun=1;
		}
	}
}
int fput(int ch ,FILE *f)
{
	u8 temp[1] = {ch};
	HAL_UART_Transmit(&huart1,(u8 *)&ch,1,0xFFFF);
}


//定向重定位
#ifdef __GNUC_

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

#else

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

#endif
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart1,(u8 *)&ch,1,0xFFFF);
	return ch;
}	



