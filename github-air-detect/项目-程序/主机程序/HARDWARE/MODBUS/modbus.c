#include "modbus.h"
#include "include.h"
#include "stm32f1xx.h"
MODBUS modbus;
extern u16 Reg[];
u8 save_flag;
ADC_DATA senor;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
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
BB   代表需要记录的数据

CC
DD   代表需要记录的数据

EE

FF

crc


*****************************************/
void SetServo_Reg(u8 add, u8 reg, u8 cmd, u16 dat)
{
    u16 crc;
    static u8 i, ch;
    i = 0;
    modbus.recount = 0;
    modbus.Sendbuf[i++] = add;
    modbus.Sendbuf[i++] = 0x06;
    modbus.Sendbuf[i++] = reg;//地址
    modbus.Sendbuf[i++] = cmd;//地址
    modbus.Sendbuf[i++] = dat / 256;
    modbus.Sendbuf[i++] = dat % 256; //数据

    crc = crc16(modbus.Sendbuf, i);
    modbus.Sendbuf[i++] = crc / 256;
    modbus.Sendbuf[i++] = crc % 256; //CRC校验
    RS485_CMD(modbus.Sendbuf, i);

}
void GetServo_Reg(u8 add, u8 reg, u8 cmd, u8 rcount) //读取伺服某寄存器的值
{
    u16 crc;
    u8 i, ch;
    i = 0;
    //modbus.CurrReg = reg;
    modbus.recount = 0;
    modbus.Sendbuf[i++] = add;
    modbus.Sendbuf[i++] = 0x03;

    modbus.Sendbuf[i++] = reg;//地址
    modbus.Sendbuf[i++] = cmd;//地址
//	  modbus.Sendbuf[i++] = rcount/256;//数据
//	  modbus.Sendbuf[i++] = rcount%256;//数据

    crc = crc16(modbus.Sendbuf, i);
    modbus.Sendbuf[i++] = crc / 256;
    modbus.Sendbuf[i++] = crc % 256; //CRC校验
    RS485_CMD(modbus.Sendbuf, i);

//	  delay_ms(100);  //此处延时时间非常重要，过短可能导致一直接收不上来数据
//	  Mosbus_Event(); //处理伺服返回的数据包
}


void Function_0x03(u16 reg)
{

//	switch(modbus.rcbuf[2])
//	{
//		case 1:// SHT30_SENOR
//			      senor.Tempreture = modbus.rcbuf[4]*256+modbus.rcbuf[5];
//		        senor.Humidity = modbus.rcbuf[6]*256+modbus.rcbuf[7];
//
//		break;
//		case 2://TGS2602_SENOR
//			      senor.TGS2602_SENOR = modbus.rcbuf[4]*256+modbus.rcbuf[5];
//
//		break;
//		case 3://SGP30_SENOR
//			      senor.SGP30_SENOR = modbus.rcbuf[4]*256+modbus.rcbuf[5];
//
//		break;
//		case 4://ZC05_SENOR
//			      senor.ZC05_SENOR =  modbus.rcbuf[4]*256+modbus.rcbuf[5];
//
//		break;
//		case 5://PM10_SENOR
//			      senor.PM10_SENOR =  modbus.rcbuf[4]*256+modbus.rcbuf[5];
//
//		break;
//		case 6:
//		break;
//
//	  case 7:
//		break;
//		default:break;
//	}


}
void Function_0x06(u16 reg)//暂时先不写呗
{



}

void control_handle(void)
{
    static u8 Set_cnt = 0;
    GetServo_Reg(Set_cnt + 1, 0x00, 0x00, 0x00);
    Set_cnt = !Set_cnt;
}


void Mosbus_Event()
{
    u8  TempFun;
    u8 k;
    u16 sum = 0;
    static u16 mm = 0;
    u16 crc;
    u16 rccrc;
    if(modbus.reflag == 0) //没有收到MODbus的数据包
    {
        return;
    }
    if(modbus.recount <= 2)
    {
        modbus.reflag = 0;
        HAL_UART_Receive_DMA(&huart1, modbus.rcbuf, 256);//使能串口2 DMA接收 -》
        return;
    }
    crc = crc16(&modbus.rcbuf[0], modbus.recount - 2);    //计算校验码
    rccrc = modbus.rcbuf[modbus.recount - 2] * 256 + modbus.rcbuf[modbus.recount - 1]; //收到的校验码
    if(crc != rccrc)
    {

        modbus.recount = 0;
        modbus.reflag = 0;
        HAL_UART_Receive_DMA(&huart1, modbus.rcbuf, 256);//使能串口2DMA接收 -》
        return;
    }
    if(modbus.rcbuf[0] == 0x01) //1号发送的数据
    {
        senor_translate(0X0003, (modbus.rcbuf[4] * 256 + modbus.rcbuf[5]) * 10 + rand() % 10); //TVOC浓度
        senor_translate(0X0004, (modbus.rcbuf[8] * 256 + modbus.rcbuf[9]) * 10 + rand() % 10); //PM2.5浓度
        senor_translate(0X0005, (modbus.rcbuf[6] * 256 + modbus.rcbuf[7]) * 10 + rand() % 10); //甲醛浓度
        senor_translate(0X0006, (modbus.rcbuf[2] * 256 + modbus.rcbuf[3]) * 10 + rand() % 10); //CO2浓度
        senor_translate(0X0007, (modbus.rcbuf[12] * 256 + modbus.rcbuf[13]) * 10 + rand() % 5 + (Tem_comp - 10) * 10); //温度
        senor_translate(0X0008, (modbus.rcbuf[10] * 256 + modbus.rcbuf[11]) * 10 + rand() % 5 + (Hum_comp - 10) * 10); //湿度


        senor.PM25[0] = modbus.rcbuf[8] * 256 + modbus.rcbuf[9];
        senor.CH2O[0] = modbus.rcbuf[6] * 256 + modbus.rcbuf[7];
        senor.CO2[0] = modbus.rcbuf[2] * 256 + modbus.rcbuf[3];
        senor.TVOC[0] = modbus.rcbuf[4] * 256 + modbus.rcbuf[5];

        OFFLINE_FLAG[0] = 0;//防离线程序
    }
    else if(modbus.rcbuf[0] == 0x02) //2号发送的数据
    {
        senor_translate(0X0003 + 6, (modbus.rcbuf[4] * 256 + modbus.rcbuf[5]) * 10 + rand() % 10); //TVOC浓度
        senor_translate(0X0004 + 6, (modbus.rcbuf[8] * 256 + modbus.rcbuf[9]) * 10 + rand() % 10); //PM2.5浓度
        senor_translate(0X0005 + 6, (modbus.rcbuf[6] * 256 + modbus.rcbuf[7]) * 10 + rand() % 10); //甲醛浓度
        senor_translate(0X0006 + 6, (modbus.rcbuf[2] * 256 + modbus.rcbuf[3]) * 10 + rand() % 10); //CO2浓度
        senor_translate(0X0007 + 6, (modbus.rcbuf[12] * 256 + modbus.rcbuf[13]) * 10 + rand() % 5 + (Tem_comp1 - 10) * 10); //温度
        senor_translate(0X0008 + 6, (modbus.rcbuf[10] * 256 + modbus.rcbuf[11]) * 10 + rand() % 5 + (Hum_comp1 - 10) * 10); //湿度

        senor.PM25[1] = modbus.rcbuf[8] * 256 + modbus.rcbuf[9];
        senor.CH2O[1] = modbus.rcbuf[6] * 256 + modbus.rcbuf[7];
        senor.CO2[1] = modbus.rcbuf[2] * 256 + modbus.rcbuf[3];
        senor.TVOC[1] = modbus.rcbuf[4] * 256 + modbus.rcbuf[5];

        OFFLINE_FLAG[1] = 0;//防离线程序

    }

    HAL_UART_Receive_DMA(&huart1, modbus.rcbuf, 256);//使能串口2DMA接收 -》
    modbus.recount = 0;
    modbus.reflag = 0;

}
void RS485_BYTE(u8 zz)
{
    HAL_UART_Transmit(&huart1, (u8*)&zz, 1, 0xff);
    //while((USART1->SR&0X40)==0);
}

void RS485_CMD(u8* zz, u8 length)
{
    u8 j;

    for(j = 0; j < length; j++)
    {
        RS485_BYTE(zz[j]);
    }
}











