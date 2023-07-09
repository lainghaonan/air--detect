#include "DGUS.h"
#include "include.h"
#include "sys.h"
u8 OFFLINE_FLAG[2];
u8 Sendbuf[100];
unsigned char Rx_end_flag=0; 
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
u8 Usart_data[200];
u16 control_point[6];
u8 allow_control[2];
u8 time[10];
u8 success_flag;;
u8 open_flag[2];
u16 death_count = 50;
u8 Tem_comp,Hum_comp,Tem_comp1,Hum_comp1;
//DGUS 发送一个字节函数
void TTL_byte(u8 zz)
{
	HAL_UART_Transmit(&huart2,(u8 *)&zz,1,0XFFFF);
}
//DGUS空闲中断处理函数







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
//
void color_change(u8 xx,u8 mode)
{
	int j;
	u8 BUF[10];
	BUF[0]=0x5A;
	BUF[1]=0xA5;
	BUF[2]=0x05;
	BUF[3]=0x82;
	BUF[4]=0x20;
	BUF[5]=xx+3;
	
	if(mode == 1)
	{
		BUF[6]=0XF8;
		BUF[7]=0X00;
	}
	else 
	{
		BUF[6]=0XFF;
		BUF[7]=0XFF;
	}

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
	Sendbuf[2]=0x03;
	Sendbuf[3]=0x80;
    Sendbuf[4]=0x01;
	Sendbuf[5]=0x20; 
	for(j=0;j<5;j++)	
	{
	 TTL_byte(Sendbuf[j]);     
	}
}

void Usart_search_time(void)
{
  TTL_byte(0X5A);
	TTL_byte(0XA5);
	TTL_byte(0X03);
	TTL_byte(0X81);
	TTL_byte(0x20);
	TTL_byte(0x07);
	//  [6] -> 年 【7】->月 【8】 ->日 【9】 ->周【10】->时  [12]->秒 【11】->分
}

void Change_page(u8  xx)
{
	 TTL_byte(0X5A);
	TTL_byte(0XA5);
	TTL_byte(0X04);
	TTL_byte(0X80);
	TTL_byte(0x03);
	TTL_byte(0x00);
	TTL_byte(xx);
}
u8 code[10]={0};
void LCD_Data_Del(void)
{
	u16  data ;
	u16  address= 0;
	if(Rx_end_flag)
	{	
		  if(Usart_data[0]!= 0x5A || Usart_data[1]!= 0xA5 )
			{
				  HAL_UART_Receive_DMA(&huart2, Usart_data, 256);//使能串口2DMA接收
	      	Rx_end_flag =0 ;
				  return ;
						
			}
			address=Usart_data[4]*256+Usart_data[5]; 
			switch(address)
			{
				case 0x0204: allow_control[0] = Usart_data[8];  senor_translate(0x0200,Usart_data[8]) ;Tx_Buffer[0]= Usart_data[8];STMFLASH_Write(FLASH_WriteAddress+20,Tx_Buffer,1);     break;    
        case 0x0205: if(allow_control[0]){senor_translate(0x0201,Usart_data[8]) ; PCout(13)=Usart_data[8]; }   break;
				case 0x0206: allow_control[1] = Usart_data[8];  senor_translate(0x0202,Usart_data[8]) ; Tx_Buffer[0]= Usart_data[8];STMFLASH_Write(FLASH_WriteAddress+22,Tx_Buffer,1);      break;    
        case 0x0207: if(allow_control[1]){senor_translate(0x0203,Usart_data[8]) ; PCout(14)=Usart_data[8];}     break;	
									
				case 0x0300: control_point[0]= Usart_data[7]*256+ Usart_data[8]; Tx_Buffer[0]=control_point[0];STMFLASH_Write(FLASH_WriteAddress,Tx_Buffer,1); break;//这几个数要记录到你内存里面
				case 0x0301: control_point[1]= Usart_data[7]*256+ Usart_data[8]; Tx_Buffer[0]=control_point[1];STMFLASH_Write(FLASH_WriteAddress+2,Tx_Buffer,1); break;
				case 0x0302: control_point[2]= Usart_data[7]*256+ Usart_data[8]; Tx_Buffer[0]=control_point[2];STMFLASH_Write(FLASH_WriteAddress+4,Tx_Buffer,1); break;
				case 0x0303: control_point[3]= Usart_data[7]*256+ Usart_data[8]; Tx_Buffer[0]=control_point[3];STMFLASH_Write(FLASH_WriteAddress+6,Tx_Buffer,1); break;
				//时间询问
				case 0x2007:	//  [6] -> 年 【7】->月 【8】 ->日 【9】 ->周【10】->时  [12]->秒 【11】->分
					          time[0] = Usart_data[6]/16*10 + Usart_data[6]%16;//年
				            time[1] = Usart_data[7]/16*10 + Usart_data[7]%16;//月
				            time[2] = Usart_data[8]/16*10 + Usart_data[8]%16;//日
				            break;
				case 0x0050:code[0] = Usart_data[7];code[1] = Usart_data[8];
					          
					          break;
				
				case 0x0400:if(code[0] == 0x08 && code[1] == 0x04){success_flag = 1;Change_page(8); Tx_Buffer[0] = 1;STMFLASH_Write(FLASH_WriteAddress+8,Tx_Buffer,1);}else {Change_page(9);}
					          break;
				case 0x0401: if(time[0] >=20 && time[1] >=10 &&time[2] >=10) {}else {Change_page(1);}break;
				
				case 0x1100:Tx_Buffer[0] = Usart_data[8];STMFLASH_Write(FLASH_WriteAddress+30,Tx_Buffer,1);Tem_comp = Usart_data[8];
										break;
				case 0x1101:Tx_Buffer[0] = Usart_data[8];STMFLASH_Write(FLASH_WriteAddress+32,Tx_Buffer,1);Hum_comp = Usart_data[8]; 
										break;
				case 0x1102:Tx_Buffer[0] = Usart_data[8];STMFLASH_Write(FLASH_WriteAddress+34,Tx_Buffer,1);Tem_comp1 = Usart_data[8];
										break;
				case 0x1103:Tx_Buffer[0] = Usart_data[8];STMFLASH_Write(FLASH_WriteAddress+36,Tx_Buffer,1);Hum_comp1 = Usart_data[8]; 
										break;				
				
				default:break;
			}
    HAL_UART_Receive_DMA(&huart2, Usart_data, 256);//使能串口2DMA接收
		Rx_end_flag =0;
	}
}


void time_control_handle(void)
{
	static u8 send_flag[5]={0};
	if(allow_control[0] == 0)//设定点问题解决
	{
	   if(control_point[0]<senor.TVOC[0] || control_point[1]<senor.PM25[0]||control_point[2]<senor.CO2[0]||control_point[3]<senor.CH2O[0])
		 {
			 open_flag[0] =1;
//		 PC00 0out(13) = 1;
		 }
//		 else
//		 {
//			 PCout(13) = 0;
//		 }

	}		
	if(allow_control[1] == 0)
	{
	   if(control_point[0]<senor.TVOC[1] || control_point[1]<senor.PM25[1]||control_point[2]<senor.CO2[1]||control_point[3]<senor.CH2O[1])
		 {
			 open_flag[1] =1;
			// PCout(14) = 1;
		 }
//		 else{PCout(14) = 0;}
	}

  if(success_flag == 0 )	
	{
			if(time[0] >= 21)
			{
				if(time[1] == 1 && send_flag[1] ==0 &&time[2] == 10)//第一次提醒
				{
					send_flag[1] = 1;
					Change_page(7);
				}
				
				if(time[1] == 2 && send_flag[2] ==0 &&time[2] == 10)//第一次提醒
				{
					send_flag[2] = 1;
					Change_page(7);
				}
				if(time[1] == 3 && send_flag[3] ==0 &&time[2] == 10)//第一次提醒
				{
					send_flag[3] = 1;
					Change_page(7);
				}				
				if(time[1]>=3 &&send_flag[4] ==0)
				{
					if(time[1] == 3 &&time[2] > 10)
					{
						send_flag[4] = 1;
						Change_page(7);
					}
					else
					{
						send_flag[4] = 1;
						Change_page(7);
					}
				}
		
	    }
	}
}

void font_quan_handle(void)
{
	//字体颜色控制
	
	
	if(OFFLINE_FLAG[0] < 8)
	{
		if(control_point[0]<senor.TVOC[0] )color_change(0x00,1); else color_change(0x00,0);
		if(control_point[1]<senor.PM25[0] )color_change(0x10,1); else color_change(0x10,0);	
		if(control_point[2]<senor.CO2[0] )color_change(0x30,1); else color_change(0x30,0);		
		if(control_point[3]<senor.CH2O[0])color_change(0x20,1); else color_change(0x20,0);		

		if(senor.TVOC[0]<50 &&senor.PM25[0]<80 && senor.CO2[0]<800 &&senor.CH2O[0]<50)
		{
			senor_translate(0X0010,0);
		}
		else if(senor.TVOC[0]<150 &&senor.PM25[0]<150 && senor.CO2[0]<1500 &&senor.CH2O[0]<150)
		{
			senor_translate(0X0010,1);
		}
		else if(senor.TVOC[0]<1000 &&senor.PM25[0]<1000 && senor.CO2[0]<3000 &&senor.CH2O[0]<1000)
		{
			senor_translate(0X0010,2);
		}	
		else
		{
			senor_translate(0X0010,3);
		}	
	}
  
	
	if(OFFLINE_FLAG[1] < 8)
	{
			if(control_point[0]<senor.TVOC[1] )color_change(0x60,1); else color_change(0x60,0);
			if(control_point[1]<senor.PM25[1] )color_change(0x70,1); else color_change(0x70,0);	
			if(control_point[2]<senor.CO2[1] )color_change(0x90,1); else color_change(0x90,0);		
			if(control_point[3]<senor.CH2O[1])color_change(0x80,1); else color_change(0x80,0);	




			if(senor.TVOC[1]<50 &&senor.PM25[1]<80 && senor.CO2[1]<800 &&senor.CH2O[1]<50)
			{
			senor_translate(0X0011,0);
			}
			else if(senor.TVOC[1]<150 &&senor.PM25[1]<150 && senor.CO2[1]<1500 &&senor.CH2O[1]<150)
			{
			senor_translate(0X0011,1);
			}
			else if(senor.TVOC[1]<1000 &&senor.PM25[1]<1000 && senor.CO2[1]<3000 &&senor.CH2O[1]<1000)
			{
			senor_translate(0X0011,2);
			}	
			else
			{
			senor_translate(0X0011,3);
			}	
	}
	
}

void Param_Init(void)
{
	//温度补偿 房间一
	STMFLASH_Read(FLASH_WriteAddress+30,Rx_Buffer,1);
	Tem_comp = Rx_Buffer[0];
	//湿度补偿 房间一
	STMFLASH_Read(FLASH_WriteAddress+32,Rx_Buffer,1);
	Hum_comp = Rx_Buffer[0];
	//温度补偿 房间二
	STMFLASH_Read(FLASH_WriteAddress+34,Rx_Buffer,1);
	Tem_comp1 = Rx_Buffer[0];
	//湿度补偿 房间二
	STMFLASH_Read(FLASH_WriteAddress+36,Rx_Buffer,1);
	Hum_comp1 = Rx_Buffer[0];	
	
	senor_translate(0x1100,Tem_comp);
	senor_translate(0x1101,Hum_comp);
	senor_translate(0x1102,Tem_comp1);
	senor_translate(0x1103,Hum_comp1);	
	
	
	
	
	//排风点获取
	STMFLASH_Read(FLASH_WriteAddress,Rx_Buffer,10);
	control_point[0] = Rx_Buffer[0];
	control_point[1] = Rx_Buffer[1];
	control_point[2] = Rx_Buffer[2];
	control_point[3] = Rx_Buffer[3];
	
	senor_translate(0x300,control_point[0]);
	senor_translate(0x301,control_point[1]);
	senor_translate(0x302,control_point[2]);
	senor_translate(0x303,control_point[3]);
	
	
}





