#ifndef _DGUS_
#define _DGUS_
#include "sys.h"
extern void TTL_byte(u8 zz);
extern void senor_translate(u16 add,u16 x);
extern void Light_Set(void);
extern void LCD_Data_Del(void);
void Change_page(u8  xx);
extern void Usart_search_time(void);
extern unsigned char Rx_end_flag; 
extern u8 Usart_data[200];;
extern u8 allow_control[2];
extern u16 control_point[6];
extern u8 time[10];;
extern void time_control_handle(void);
extern u8 open_flag[2];;
extern u8 success_flag;
extern void color_change(u8 xx,u8 mode);
extern u8 OFFLINE_FLAG[2];
extern u16 death_count;
extern void font_quan_handle(void);
void Param_Init(void);
extern u8 Tem_comp,Hum_comp,Tem_comp1,Hum_comp1;
#endif

