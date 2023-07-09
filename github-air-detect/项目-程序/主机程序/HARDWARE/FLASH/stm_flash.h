#ifndef __STMFLASH_H__
#define __STMFLASH_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
/************************** STM32 �ڲ� FLASH ���� *****************************/
#define STM32_FLASH_SIZE        64  // ��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN        1    // stm32оƬ����FLASH д��ʹ��(0������;1��ʹ��)
extern uint16_t Tx_Buffer[512];
extern uint16_t Rx_Buffer[512];
/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);		  //��������


void STMFLASH_WriteLenByte(uint32_t WriteAddr, uint32_t DataToWrite, uint16_t Len );	      //ָ����ַ��ʼд��ָ�����ȵ�����
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr, uint16_t Len );					                    	//ָ����ַ��ʼ��ȡָ����������
extern void STMFLASH_Write( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite );		//��ָ����ַ��ʼд��ָ�����ȵ�����
extern void STMFLASH_Read( uint32_t ReadAddr, uint16_t * pBuffer, uint16_t NumToRead );   	//��ָ����ַ��ʼ����ָ�����ȵ�����
extern void Test_Write(uint32_t WriteAddr,uint16_t WriteData)   	;
#endif /* __STMFLASH_H__ */

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
