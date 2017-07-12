#ifndef _SCHEDULE_H__
#define _SCHEDULE_H__

#include "Global.h"









typedef struct
{
	unsigned char check_flag;
	unsigned char err_flag;
	unsigned short cnt_1ms;
	unsigned short cnt_2ms;
	unsigned short cnt_5ms;
	unsigned short cnt_10ms;
	unsigned short cnt_20ms;
	unsigned short cnt_50ms;
	unsigned long time;
}LOOP_T;


typedef struct
{
	unsigned char flag_mag_cali;
	unsigned char flag_send_mag;
	short speed;
	short direction;
	int soft_watchdog;
}sys_status_t;


#define MAX_SPEED  500





extern sys_status_t local_data;

void system_init(void);

void schedule_check(void);

void Duty_Loop(void);




#endif

