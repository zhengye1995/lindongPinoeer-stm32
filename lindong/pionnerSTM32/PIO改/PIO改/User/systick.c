
#include "systick.h"




unsigned char Init_Status = 0;

unsigned long sysTickUptime = 0;



void Systick_Init(void)
{
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
}


uint64_t GetSysTime_us(void)
{
	register uint32_t ms;
	uint64_t value;
	ms = sysTickUptime;
	value = ms * 1000 + (SysTick->LOAD - SysTick->VAL) * 1000 / SysTick->LOAD;
	return value;
}

void delay_us(unsigned int nus)
{
	uint32_t now = GetSysTime_us();
    while (GetSysTime_us() - now < nus);
}

void delay_ms(unsigned short nms)
{
	while(nms--)
	{
		delay_us(1000);
	}	
}


int time_1h,time_1m,time_1s,time_1ms;
void sys_time()
{ 
	if( !Init_Status )
	{
		return;
	}

	if(time_1ms < 999)
	{
		time_1ms++;

		schedule_check();
	}
	else
	{

		time_1ms =0;
	}
}
void SysTick_Handler(void)
{
	sysTickUptime ++;
	
 	sys_time();
}


volatile float cycleTime[GET_TIME_NUM][3];
enum
{
	oldTime = 0,
	newTime,
	value
};

float getCycleTime(unsigned item)
{
	cycleTime[item][oldTime] = cycleTime[item][newTime];
	cycleTime[item][newTime] = GetSysTime_us()*0.000001f;
	cycleTime[item][value] = cycleTime[item][newTime] - cycleTime[item][oldTime];
	return cycleTime[item][value];
}





