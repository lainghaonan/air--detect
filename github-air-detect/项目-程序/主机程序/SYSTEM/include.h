#ifndef __INCLUDE_H
#define __INCLUDE_H

#include "main.h"
#include "stm32f1xx_hal.h"
#include "sys.h"
#include "delay.h"
#include "stm32f1xx_it.h"
#include "modbus.h"
#include "stm_flash.h"  
#include "DGUS.h"
/******************�������룬�õ����ִ�������ȥ��ע�ͼ���******************/
#define LOCAL_TYPE  0x01
#define LOCAL_ID    0x01
/*****************************************************************************/
extern u8 success_flag;










#define  DISABLE_INTRUPET __set_PRIMASK(1)
#define  ENABLE_INTRUPET  __set_PRIMASK(0)


#define  FLASH_WriteAddress     0x0800F000            // д�ڿ���λ�ã���ֹ�ƻ�����
#define  FLASH_ReadAddress      0x0800F000
#define  FLASH_TESTSIZE         512                 //ʵ����512*2=1024�ֽ�


#endif
