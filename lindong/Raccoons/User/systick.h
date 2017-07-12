#ifndef _SYSTICK_H__

#define _SYSTICK_H__

#include "Global.h"



extern unsigned char Init_Status;

void Systick_Init(void);

uint64_t GetSysTime_us(void);
float getCycleTime(unsigned item); 

void delay_us(unsigned int nus);
void delay_ms(unsigned short nms);


#define GET_TIME_NUM 	(5)		//设置获取时间的数组数量





#endif



