#include "modbus.h"
#include "include.h"
#include "stm32f1xx.h"
MODBUS modbus,modbus1;
extern u16 Reg[];
u8 save_flag;
GAS gas;
/*********����ָ��֡Э��*******************

ѯ������ṹ
01 
03 
0x x= 1,2,3,4,5,6,7  //1->SHT30_SENOR  2->TGS2602_SENOR  3->SGP30_SENOR 
                     //4->ZC05_SENOR   5->PM10_SENOR     6->���λַ    7->����λַ
xx xx�����ַ  



AA
BB   ����ش�������

crc
*************************************************
�޸�����ṹ
01 
06 
0x x= 1,2,3,4,5,6,7  //1->SHT30_SENOR  2->TGS2602_SENOR  3->SGP30_SENOR 
                     //4->ZC05_SENOR   5->PM10_SENOR     6->���λַ    7->����λַ
xx xx�����ַ  

MM   //��������/��λ�� 
AA
BB   ������Ҫ�޸ĵ�����

crc


*****************************************/


void Modbud_fun3()  //3�Ź����봦��  ---����Ҫ��ȡ���ӻ��ļĴ���
{
    u16 Regadd;
	u16 Reglen;
	u16 byte;
	u16 j;
	u16 crc;
	u8  i=0;
	modbus.Sendbuf[i++]=LOCAL_ID;
  modbus.Sendbuf[i++]=0x03;        //������   
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

void Modbud_fun6()  //6�Ź����봦��
{
    u16 Regadd;
	u16 val;
	u16 i,crc,j;
	i=0;
    Regadd=modbus.rcbuf[3];  //�õ�Ҫ�޸ĵĵ�ַ 
	val=modbus.rcbuf[4]*256+modbus.rcbuf[5];     //�޸ĺ��ֵ
//	if(Regadd == 4)
//	{
//		AC_Board.Power_state = val;
//	}
//	else if(Regadd == 5)
//	{
//		AC_Board.Power_state =1;
//		AC_Board.Speed_state = val;
//	}
//	//����Ϊ��Ӧ����
//	
//	
//	modbus.Sendbuf[i++]=0xd3;//���豸��ַ
//    modbus.Sendbuf[i++]=0x06;        //������ 

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
    if(modbus.reflag==0)  //û���յ�MODbus�����ݰ�
	{
		return;
	}
	  crc= crc16(&modbus.rcbuf[0], modbus.recount-2);       //����У����
    rccrc=modbus.rcbuf[modbus.recount-2]*256 + modbus.rcbuf[modbus.recount-1];  //�յ���У����
    if(crc ==  rccrc)  //���ݰ�����CRCУ�����
	{ 
	  if(modbus.rcbuf[0] == LOCAL_ID)//ȷ�����ݰ��Ƿ��Ƿ������豸�� 
		{
		  switch(modbus.rcbuf[1])  //����������
			{
				case 0:     break;
				case 1:     break;
				case 2:     break;
				case 3:     Modbud_fun3();    break;   //3�Ź����봦��
				case 4:     break;
				case 5:     break;
				case 6:     Modbud_fun6();     break;   //6�Ź����봦��
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
    if(modbus1.reflag==0)  //û���յ�MODbus�����ݰ�
	{
		return;
	}
	 crc= crc16(&modbus1.rcbuf[0], modbus1.recount-2);       //����У����
   rccrc=modbus1.rcbuf[modbus1.recount-2]*256 + modbus1.rcbuf[modbus1.recount-1];  //�յ���У����
   if(crc ==  rccrc)  //���ݰ�����CRCУ�����
	{ 
	  if(modbus1.rcbuf[0] == DEVICE_ID)  //ȷ�����ݰ��Ƿ��Ƿ������豸�� 
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


