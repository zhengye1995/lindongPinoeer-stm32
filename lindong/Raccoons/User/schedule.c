#include "schedule.h"

int soft_watchdog = 0;

LOOP_T loop;
sys_status_t local_data;



void schedule_init(void)
{
	loop.check_flag = 0;
	loop.err_flag = 0;
	loop.time = 0;
	loop.cnt_1ms = 0;
	loop.cnt_2ms = 0;
	loop.cnt_5ms = 0;
	loop.cnt_10ms = 0;
	loop.cnt_20ms = 0;
	loop.cnt_50ms = 0;
	loop.err_flag = 0;
	
	local_data.flag_mag_cali = 0;
	local_data.soft_watchdog = 0;
	local_data.flag_send_mag = 0;
}


void schedule_check(void)
{
	loop.time ++;
	loop.cnt_1ms++;
	loop.cnt_2ms++;
	loop.cnt_5ms++;
	loop.cnt_10ms++;
	loop.cnt_20ms++;
	loop.cnt_50ms++;
	if(loop.check_flag == 1)
	{
		loop.err_flag ++;
	}
	else
	{
		loop.check_flag = 1;	// 该标志位应在循环执行完毕后清零
	}
}




void system_init(void)
{
	
	
	


}




void duty_1ms()
{
	uint8_t dat;
	if(USART_GetFlagStatus(UART4, USART_FLAG_RXNE))
	{
		dat = USART_ReceiveData(UART4);
		uart4_receiveProcess(dat);
		
		
		USART_ClearFlag(UART4, USART_FLAG_RXNE );
	}
	
}
void duty_2ms()
{
	
}

void duty_5ms()
{
	DT_U4_Send();
}





// 电机控制
void duty_10ms()
{
	short motor1,motor2;
	if(local_data.flag_mag_cali != 0x01)
	{
		if(local_data.soft_watchdog-- > 0)
		{
			motor1 = local_data.speed + local_data.direction;
			motor2 = local_data.speed - local_data.direction;
			
			if(motor1 > MAX_SPEED) motor1 = MAX_SPEED;
			if(motor1 < -MAX_SPEED) motor1 = -MAX_SPEED;
			if(motor2 > MAX_SPEED) motor2 = MAX_SPEED;
			if(motor2 < -MAX_SPEED) motor2 = -MAX_SPEED;
			
			if(local_data.speed > 0)
			{
				Motor_CtrlLR(motor1, motor2);
			}
			else
			{
				Motor_CtrlLR(motor2, motor1);
			}
		}
		else
		{
			local_data.soft_watchdog = 0;
			SetMotor1PWM(0);
			SetMotor2PWM(0);
		}
	}
	else
	{
		Motor_CtrlLR(200, -200);
	}
	
	mpu_data_prepare();
	sensor_process();
}

void duty_20ms()
{

}

void duty_50ms()
{

}


void Duty_Loop(void)
{
	if(Init_Status == 1)
	{
		if(loop.check_flag == 1)
		{
			duty_1ms();
			if(loop.cnt_2ms >= 2)
			{
				loop.cnt_2ms = 0;
				duty_2ms();
			}
			
			if(loop.cnt_5ms >= 5)
			{
				loop.cnt_5ms = 0;
				duty_5ms();
			}
			
			if(loop.cnt_10ms >= 10)
			{
				loop.cnt_10ms = 0;
				duty_10ms();
			}
			
			if(loop.cnt_20ms >= 20)
			{
				loop.cnt_20ms = 0;
				duty_20ms();
			}
			
			if(loop.cnt_50ms >= 50)
			{
				loop.cnt_50ms = 0;
				duty_50ms();
			}
			
			loop.check_flag = 0;
		}	
	}
}
















