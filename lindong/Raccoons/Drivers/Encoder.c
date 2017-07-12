/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/
#include "Encoder.h"

/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/
s32 Speed_L_Get = 0;
s32 Speed_R_Get = 0;
s32 Speed_L_Get_NEW = 0;
s32 Speed_R_Get_NEW = 0;
s32 Last_Speed_L_Get = 0;
s32 Last_Speed_R_Get = 0;
double R_Sum_Step = 0;
double L_Sum_Step = 0;
double Sum_Step = 0;
int motorflag=0;
/********************************************************************************************************
Function Name: EncoderIOInit
Description  :
Inputs       : None
Outputs      : None
Notes        : Encoder IO初始化
Revision     :
********************************************************************************************************/
void EncoderIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);  
}

/********************************************************************************************************
Function Name: EncoderTIMInit
Description  :
Inputs       : None
Outputs      : None
Notes        : Encoder TIME初始化
Revision     :
********************************************************************************************************/
void EncoderTIMInit(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

    /***********************   ENCODER2  *************************/
    TIM_DeInit(TIM1);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0;  // No prescaling
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);	  //  编码器计数方式
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 6;	 //   滤波器
    TIM_ICInit(TIM1, &TIM_ICInitStructure);
    TIM_SetCounter(TIM1, 0);
    TIM_Cmd(TIM1, ENABLE);

    /**********************  ENCODER1  *************************/
    TIM_DeInit(TIM8);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0;  // No prescaling
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);	//选定T1和T2同时计数模式，上升沿计数
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 6;
    TIM_ICInit(TIM8, &TIM_ICInitStructure);
    TIM_SetCounter(TIM8, 0);
    TIM_Cmd(TIM8, ENABLE);
}

/********************************************************************************************************
Function Name: EncoderInit
Description  :
Inputs       : None
Outputs      : None
Notes        : Encoder初始化
Revision     :
********************************************************************************************************/
void EncoderInit(void)
{
    EncoderIOInit();
    EncoderTIMInit();
}

/********************************************************************************************************
Function Name: GetSpeed
Description  :
Inputs       : None
Outputs      : None
Notes        :
Revision     :
********************************************************************************************************/
SINT32 GetSpeedL(void)
{
	 	
		static SINT32 PrePulse = 0;
    SINT32 Pulse = 0;
    SINT32 DeltaPulse = 0;

    // Get the pulse
    Pulse = TIM_GetCounter(TIM1);
	  //ENCODER_L_TIMER->CNT = COUNTER_RESET;			//计数器清零
    DeltaPulse = Pulse - PrePulse;
    
    // Overflow processing
    if(DeltaPulse >= 32768)
    {
        DeltaPulse -= 65535;   
    }
    else if(DeltaPulse <= -32768)
    {
        DeltaPulse += 65535;
    }
		
    // Encoder direction
//    if((TIM2->CR1 & TIM_CounterMode_Down) == TIM_CounterMode_Down)  
//    {
//        return -(DeltaPulse<<2);
//    }
//    else  
//    {
//        return (DeltaPulse<<2);
//    }
    PrePulse = Pulse;

    return -(DeltaPulse<<2);
}
SINT32 GetSpeedR(void)
{
	 	
		static SINT32 PrePulse = 0;
    SINT32 Pulse = 0;
    SINT32 DeltaPulse = 0;

    // Get the pulse
    Pulse = TIM_GetCounter(TIM8);
	  //ENCODER_L_TIMER->CNT = COUNTER_RESET;			//计数器清零
    DeltaPulse = Pulse - PrePulse;
    
    // Overflow processing
    if(DeltaPulse >= 32768)
    {
        DeltaPulse -= 65535;   
    }
    else if(DeltaPulse <= -32768)
    {
        DeltaPulse += 65535;
    }

    PrePulse = Pulse;

    return -(DeltaPulse<<2);
}	
	
	
	
	
void GetSpeed(void)
{	
	  Speed_L_Get = TIM_GetCounter(ENCODER_L_TIMER);		//采集左轮速度
    ENCODER_L_TIMER->CNT = COUNTER_RESET;			//计数器清零
    if ((ENCODER_L_TIMER->CR1 & (1 << 4)))
    {
        Speed_L_Get = Speed_L_Get - 65537;
    }
    Speed_L_Get = -Speed_L_Get;							//根据电路更改符号
		left.vi_FeedBack = Speed_L_Get;
	
	
	
    Speed_R_Get = TIM_GetCounter(ENCODER_R_TIMER);		//采集右轮速度
		right.vi_FeedBack = Speed_R_Get;
    ENCODER_R_TIMER->CNT = COUNTER_RESET;			//计数器清零

    if ((ENCODER_R_TIMER->CR1 & (1 << 4)))
    {
        Speed_R_Get = Speed_R_Get - 65537;
    }
    Speed_R_Get = -Speed_R_Get;

    



//    if(Speed_L_Get > 400 || Speed_L_Get < -400)
//    {
//        Speed_L_Get = Last_Speed_L_Get;
//    }
//    if(Speed_R_Get > 400 || Speed_R_Get < -400)
//    {
//        Speed_R_Get = Last_Speed_R_Get;
//    }


    Last_Speed_L_Get = Speed_L_Get;
    Last_Speed_R_Get = Speed_R_Get;			//保留上次的速度值

    L_PID.vi_FeedBack = Speed_L_Get;
    R_PID.vi_FeedBack = Speed_R_Get;
		
		
		
    if(Speed_L_Get > 1 || Speed_L_Get < -1)
    {
        L_Sum_Step += Speed_L_Get;
    }

    if(Speed_R_Get > 1 || Speed_R_Get < -1)
    {

        R_Sum_Step += Speed_R_Get;
    }
    //Sum_Step = (R_Sum_Step + L_Sum_Step);
}

s32 SpeedL = 0;
//void GetSpeedL(void)
//{
//	static s32 PrePulse = 0;
//	s32 Pulse = 0;
//	
//	Pulse = TIM_GetCounter(TIM1);//采集左轮速度
//	SpeedL = Pulse - PrePulse;
//	
//	if(SpeedL >= 32768)
//	{
//		SpeedL -= 65535;
//	}
//	else if(SpeedL <= -32768)
//	{
//		SpeedL += 65535;
//	}
//	PrePulse = Pulse;
//   
//}
s32 SpeedR = 0;
//void GetSpeedR(void)
//{
//	static s32 PrePulse = 0;
//	s32 Pulse = 0;
//	
//	Pulse = TIM_GetCounter(TIM8);//采集右轮速度
//	SpeedR = Pulse - PrePulse;
//	
//	if(SpeedR >= 32768)
//	{
//		SpeedR -= 65535;
//	}
//	else if(SpeedR <= -32768)
//	{
//		SpeedR += 65535;
//	}
//	PrePulse = Pulse;
//   
//}


SINT32 GetEncoder1_Pulse(TIM_TypeDef *TIMx)
{
    static SINT32 PrePulse = 0;
    SINT32 Pulse = 0;
    SINT32 DeltaPulse = 0;

    // Get the pulse
    Pulse = TIM_GetCounter(TIMx);
    DeltaPulse = Pulse - PrePulse;
    
    // Overflow processing
    if(DeltaPulse >= PULSE_MAX)
    {
        DeltaPulse -= 65535;   
    }
    else if(DeltaPulse <= -PULSE_MAX)
    {
        DeltaPulse += 65535;
    }
		//Speed_L_Get_NEW = DeltaPulse;
    // Encoder direction
//    if((TIMx->CR1 & TIM_CounterMode_Down) == TIM_CounterMode_Down)  
//    {
//			  //motorflag = 1;
//        DeltaPulse = -DeltaPulse;
//    }
//    else  
//    {
//			//motorflag = 0;
//        DeltaPulse = DeltaPulse;
//    }

    PrePulse = Pulse;

    return(DeltaPulse);
}

SINT32 GetEncoder2_Pulse(TIM_TypeDef *TIMx)
{
    static SINT32 PrePulse = 0;
    SINT32 Pulse = 0;
    SINT32 DeltaPulse = 0;

    // Get the pulse
    Pulse = TIM_GetCounter(TIMx);
    DeltaPulse = Pulse - PrePulse;
    
    // Overflow processing
    if(DeltaPulse >= PULSE_MAX)
    {
        DeltaPulse -= 65535;   
    }
    else if(DeltaPulse <= -PULSE_MAX)
    {
        DeltaPulse += 65535;
    }

    // Encoder direction
//    if((TIMx->CR1 & TIM_CounterMode_Down) == TIM_CounterMode_Down)  
//    {
//			  //motorflag = 1;
//        DeltaPulse = -DeltaPulse;
//    }
//    else  
//    {
//			  //motorflag = 0;
//        DeltaPulse = DeltaPulse;
//    }

    PrePulse = Pulse;

    return(DeltaPulse);
}
Robot_Location rl;
SINT32 Robot_Loaction(float speed, float Angular_velocity)
{
   float t=5;//5ms
	if(ABS(Speed_R_Get-Speed_L_Get)>=3)
	{
		//rl.angle += Angular_velocity*5*360;
		rl.angle += Angular_velocity*5*180/PI;
	}
	//rl.angle += Angular_velocity;
	 if(rl.angle>=360)
	 {
		 rl.angle -= 360;
	 }
	 if(rl.angle<=-360)
	 {
		 rl.angle += 360;
	 }
	 //rl.x += speed * t;
	 //rl.y += speed * t;
   rl.x += speed * t * cos(rl.angle);
   rl.y += speed * t * sin(rl.angle);
	 
}
/*----------------------------------------  F U N C T I O N S  ----------------------------------------*/

//===========================================  End Of File  ===========================================//
