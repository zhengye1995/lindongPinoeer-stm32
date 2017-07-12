#include "Global.h"

void TimerInit(void)
{
	//数据类型定义
  TIM_TimeBaseInitTypeDef timBaseInitStructure;//基本定时对象
 	NVIC_InitTypeDef nvicInitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  timBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timBaseInitStructure.TIM_Prescaler = 36 - 1;
  timBaseInitStructure.TIM_Period = 1000 - 1;
  timBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM3, &timBaseInitStructure);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
	

  nvicInitStructure.NVIC_IRQChannel = TIM3_IRQn;
  nvicInitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  nvicInitStructure.NVIC_IRQChannelSubPriority = 1;
  nvicInitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvicInitStructure);
}

int num;
int flag = 0;
int l=40;
void TIM3_IRQHandler(void)//左右轮与实际相反
{
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		num++;
		//GetEncoder1_Pulse(ENCODER_L_TIMER);
		//Speed_L_Get_NEW = GetEncoder1_Pulse(ENCODER_L_TIMER);
		//Speed_R_Get_NEW = GetEncoder2_Pulse(ENCODER_R_TIMER);
		Speed_L_Get = GetEncoder2_Pulse(ENCODER_L_TIMER);
		Speed_R_Get = GetEncoder1_Pulse(ENCODER_R_TIMER);
		L_Sum_Step += ((float)Speed_L_Get/1438.85);
				
				R_Sum_Step += ((float)Speed_R_Get/1438.85);
			
				Sum_Step = R_Sum_Step + L_Sum_Step;
		if(num==5)//5ms
		{
			num=0;
		  //轮子直径222mm， 左右轮轴心距381mm，近似为380mm
			Robot_Loaction((Speed_L_Get+Speed_R_Get)/2.0/1433.8, (Speed_R_Get-Speed_L_Get)/38.0/1433.8);
			
		}
		
//			if(num==5)//5ms
//			{
//				//PCout(13) = !PCout(13);
//				Speed_L_Get = TIM_GetCounter(ENCODER_L_TIMER);		//采集左轮速度
//				ENCODER_L_TIMER->CNT = COUNTER_RESET;			//计数器清零
//				if ((ENCODER_L_TIMER->CR1 & (1 << 4)))
//				{
//						Speed_L_Get = Speed_L_Get - 65537;
//				}
//				//Speed_L_Get = -Speed_L_Get;							//根据电路更改符号
//				
//			
//			
//			
//				Speed_R_Get = TIM_GetCounter(ENCODER_R_TIMER);		//采集右轮速度
//				ENCODER_R_TIMER->CNT = COUNTER_RESET;			//计数器清零

//				if ((ENCODER_R_TIMER->CR1 & (1 << 4)))
//				{
//						Speed_R_Get = Speed_R_Get - 65537;
//				}
//				//Speed_R_Get = -Speed_R_Get;

//				



//				if(Speed_L_Get > 1000 || Speed_L_Get < -1000)
//				{
//						Speed_L_Get = Last_Speed_L_Get;
//				}
//				if(Speed_R_Get > 1000 || Speed_R_Get < -1000)
//				{
//						Speed_R_Get = Last_Speed_R_Get;
//				}


//				Last_Speed_L_Get = Speed_L_Get;
//				Last_Speed_R_Get = Speed_R_Get;			//保留上次的速度值

//				L_PID.vi_FeedBack = Speed_L_Get;
//				R_PID.vi_FeedBack = Speed_R_Get;


				
				
//				flag ++;
//				num = 0;
//			}
	 } 
}
