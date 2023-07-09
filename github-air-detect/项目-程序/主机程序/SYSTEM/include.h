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
/******************条件编译，用到何种传感器，去掉注释即可******************/
#define LOCAL_TYPE  0x01
#define LOCAL_ID    0x01
/*****************************************************************************/
extern u8 success_flag;










#define  DISABLE_INTRUPET __set_PRIMASK(1)
#define  ENABLE_INTRUPET  __set_PRIMASK(0)


#define  FLASH_WriteAddress     0x0800F000            // 写在靠后位置，防止破坏程序
#define  FLASH_ReadAddress      0x0800F000
#define  FLASH_TESTSIZE         512                 //实际是512*2=1024字节


#endif
