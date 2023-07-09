#ifndef _interrupt_H
#define _interrupt_H
#include "sys.h"




extern u8 Usart_data[200];
extern u8 RC_BUF[10];
extern u16 ADC_ConvertedValue;
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;
extern IWDG_HandleTypeDef hiwdg;
extern u16 Z05_VALUE;
extern volatile u16 PM2_5_VALUE;
extern u16 PM10_VALUE;
extern u16 VOC_VALUE;
extern float now_air;
extern u8 PM25_H;
extern u8 PM25_L;




#endif

