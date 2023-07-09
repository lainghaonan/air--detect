#include "modbus.h"
#include "include.h"
#include "stm32f1xx.h"
MODBUS modbus,modbus1;
extern u16 Reg[];
u8 save_flag;
GAS gas;
/*********数据指令帧协议*******************

询问命令结构
01 
03 
0x x= 1,2,3,4,5,6,7  //1->SHT30_SENOR  2->TGS2602_SENOR  3->SGP30_SENOR 
                     //4->ZC05_SENOR   5->PM10_SENOR     6->风机位址    7->待用位址
xx xx代表地址  



AA
BB   代表回传的数据

crc
*************************************************
修改命令结构
01 
06 
0x x= 1,2,3,4,5,6,7  //1->SHT30_SENOR  2->TGS2602_SENOR  3->SGP30_SENOR 
                     //4->ZC05_SENOR   5->PM10_SENOR     6->风机位址    7->待用位址
xx xx代表地址  

MM   //代表报警点/复位点 
AA
BB   代表需要修改的数据

crc


*****************************************/


void Modbud_fun3()  //3号功能码处理  ---主机要读取本从机的寄存器
{
    u16 Regadd;
	u16 Reglen;
	u16 byte;
	u16 j;
	u16 crc;
	u8  i=0;
	modbus.Sendbuf[i++]=LOCAL_ID;
  modbus.Sendbuf[i++]=0x03;        //功能码   
/**/
	
//			gas.CO2 = modbus1.rcbuf[3]*256 + modbus1.rcbuf[4];
//			gas.TVOC = (modbus1.rcbuf[5]*256 + modbus1.rcbuf[6])/10;
//			gas.CH20 = (modbus1.rcbuf[7]*256 + modbus1.rcbuf[8])/10;
//			gas.PW2_5 = modbus1.rcbuf[9]*256 + modbus1.rcbuf[10];
//			
//			gas.HUM = -6+125*((float)(modbus1.rcbuf[11]*256 + modbus1.rcbuf[12])/65535.0f);
//			gas.TEM = -46.85+175.72*(((float)(modbus1.rcbuf[13]*256 + modbus1.rcbuf[14]))/65535.0f);
	
	modbus.Sendbuf[i++] = gas.CO2/256;
	modbus.Sendbuf[i++] = gas.CO2%256;
	modbus.Sendbuf[i++] = gas.TVOC/256;
	modbus.Sendbuf[i++] = gas.TVOC%256;
	modbus.Sendbuf[i++] = gas.CH20/256;
	modbus.Sendbuf[i++] = gas.CH20%256;
	modbus.Sendbuf[i++] = gas.PW2_5/256;
	modbus.Sendbuf[i++] = gas.PW2_5%256;
  modbus.Sendbuf[i++] = gas.HUM/256;
	modbus.Sendbuf[i++] = gas.HUM%256;
	modbus.Sendbuf[i++] = gas.TEM/256;
	modbus.Sendbuf[i++] = gas.TEM%256;
	
	crc=crc16(modbus.Sendbuf,i);
	modbus.Sendbuf[i++]=crc/256;            
	modbus.Sendbuf[i++]=crc%256;
	
	
	for(j=0;j<i;j++)
	{
	 RS485_BYTE(modbus.Sendbuf[j]);
	}
	
	
}

void Modbud_fun6()  //6号功能码处理
{
    u16 Regadd;
	u16 val;
	u16 i,crc,j;
	i=0;
    Regadd=modbus.rcbuf[3];  //得到要修改的地址 
	val=modbus.rcbuf[4]*256+modbus.rcbuf[5];     //修改后的值
//	if(Regadd == 4)
//	{
//		AC_Board.Power_state = val;
//	}
//	else if(Regadd == 5)
//	{
//		AC_Board.Power_state =1;
//		AC_Board.Speed_state = val;
//	}
//	//以下为回应主机
//	
//	
//	modbus.Sendbuf[i++]=0xd3;//本设备地址
//    modbus.Sendbuf[i++]=0x06;        //功能码 

//	modbus.Sendbuf[i++]=AC_Board.IP_Set;
//	if(Regadd == 4)
//	{
//		modbus.Sendbuf[i++] =4;
//	}
//	else if(Regadd == 5)
//	{
//		modbus.Sendbuf[i++]=0X05;
//	}
//	modbus.Sendbuf[i++]=val/256;
//	modbus.Sendbuf[i++]=val%256;
//    crc=crc16(modbus.Sendbuf,i);
//	modbus.Sendbuf[i++]=crc/256;  //
//	modbus.Sendbuf[i++]=crc%256;
//	
//    RS485_MO_EN = 1;
//	
//	for(j=0;j<i;j++)
//	{
//	 RS485_BYTE(modbus.Sendbuf[j]);
//	}
//	
//	RS485_MO_EN = 0;
}




void Mosbus_Event() 
{
		
	u16 crc;
	u16 rccrc;
	static u16 mm=0;
    if(modbus.reflag==0)  //没有收到MODbus的数据包
	{
		return;
	}
	  crc= crc16(&modbus.rcbuf[0], modbus.recount-2);       //计算校验码
    rccrc=modbus.rcbuf[modbus.recount-2]*256 + modbus.rcbuf[modbus.recount-1];  //收到的校验码
    if(crc ==  rccrc)  //数据包符号CRC校验规则
	{ 
	  if(modbus.rcbuf[0] == LOCAL_ID)//确认数据包是否是发给本设备的 
		{
		  switch(modbus.rcbuf[1])  //分析功能码
			{
				case 0:     break;
				case 1:     break;
				case 2:     break;
				case 3:     Modbud_fun3();    break;   //3号功能码处理
				case 4:     break;
				case 5:     break;
				case 6:     Modbud_fun6();     break;   //6号功能码处理
				case 7:     break;			
        //....				
			}
		}
	
		
	}
	
	modbus.recount=0;   //
  modbus.reflag=0;	
}

void Sensor_Event() 
{
  
	u16 crc;
	u16 rccrc;
	static u16 mm=0;
    if(modbus1.reflag==0)  //没有收到MODbus的数据包
	{
		return;
	}
	 crc= crc16(&modbus1.rcbuf[0], modbus1.recount-2);       //计算校验码
   rccrc=modbus1.rcbuf[modbus1.recount-2]*256 + modbus1.rcbuf[modbus1.recount-1];  //收到的校验码
   if(crc ==  rccrc)  //数据包符号CRC校验规则
	{ 
	  if(modbus1.rcbuf[0] == DEVICE_ID)  //确认数据包是否是发给本设备的 
		{
		  gas.CO2 = modbus1.rcbuf[3]*256 + modbus1.rcbuf[4];
			gas.TVOC = (modbus1.rcbuf[5]*256 + modbus1.rcbuf[6])/10;
			gas.CH20 = (modbus1.rcbuf[7]*256 + modbus1.rcbuf[8])/10;
			gas.PW2_5 = modbus1.rcbuf[9]*256 + modbus1.rcbuf[10];
			
			gas.HUM = -6+125*((float)(modbus1.rcbuf[11]*256 + modbus1.rcbuf[12])/65535.0f);
			gas.TEM = -46.85+175.72*(((float)(modbus1.rcbuf[13]*256 + modbus1.rcbuf[14]))/65535.0f);
		}
	
		
	}
	
	modbus1.recount=0;   //
  modbus1.reflag=0;	
}
void RS485_BYTE(u8 zz)
{
	HAL_UART_Transmit(&huart1,(u8 *)&zz,1,0);
	while((USART1->SR&0X40)==0);   
}
void RS485_CMD(u8* zz,u8 length)
{
	u8 j;

	for(j=0;j<length;j++) 
	{
		RS485_BYTE(zz[j]);
	}

}


//02 03 00 00 00 06 C5 FB 
void senor_get(void)
{
	type_BYTE(DEVICE_ID);
	type_BYTE(0x03);
	type_BYTE(0x00);
	type_BYTE(0x00);
	type_BYTE(0x00);
	type_BYTE(0x06);
	type_BYTE(0xc5);
	if(DEVICE_ID == 0x01) type_BYTE(0xc8);
	else type_BYTE(0xFB);
}

void type_BYTE(u8 zz)
{
	HAL_UART_Transmit(&huart2,(u8 *)&zz,1,0);
	while((USART2->SR&0X40)==0);   
}


