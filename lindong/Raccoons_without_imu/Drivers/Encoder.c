/********************************************************************************************************
**                                 Copyright (c)   lindong EE
**                                                 All Rights Reserved
**  
**                                 Email:18813124313@qq.com   weixin:18813124313
**-------------------------------------------------------------------------------------------------------
**  MCU        : STM32F103RE (STMicroelectronics)
**  Compiler   : Keil uVision 5.16a
**  Module Name: Encoder.c
**  Module Date: 2017-07-13
**  Module Auth: zhengye
**  Description: 
**  Version    : V1.0
**  Notes      : 
**-------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/
#include "Encoder.h"

/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/
s32 Speed_L_Get = 0;
s32 Speed_R_Get = 0;
double R_Sum_Step = 0;
double L_Sum_Step = 0;
double Sum_Step = 0;
Robot_Location rl;
/*----------------------------------------  F U N C T I O N S  ----------------------------------------*/
/********************************************************************************************************
Function Name: EncoderIOInit
Description  :
Inputs       : None
Outputs      : None
Notes        : Encoder IO初始化
Revision     :
********************************************************************************************************/
SINT32 GetEncoder1_Pulse()
{
    static SINT32 PrePulse = 0;
    SINT32 Pulse = 0;
    SINT32 DeltaPulse = 0;

    // Get the pulse
    Pulse = TIM_GetCounter(ENCODER_R_TIMER);
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
/********************************************************************************************************
Function Name: EncoderIOInit
Description  :
Inputs       : None
Outputs      : None
Notes        : Encoder IO初始化
Revision     :
********************************************************************************************************/
SINT32 GetEncoder2_Pulse()
{
    static SINT32 PrePulse = 0;
    SINT32 Pulse = 0;
    SINT32 DeltaPulse = 0;

    // Get the pulse
    Pulse = TIM_GetCounter(ENCODER_L_TIMER);
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
/********************************************************************************************************
Function Name: Robot_Loaction
Description  :
Inputs       : None
Outputs      : None
Notes        : 
Revision     :
********************************************************************************************************/
void Robot_Loaction(float speed, float Angular_velocity)
{
  float t=5;//5ms
	if(ABS(Speed_R_Get-Speed_L_Get)>=3)
	{
		rl.angle += Angular_velocity*5*180/PI;
	}
	 if(rl.angle>=360)
	 {
		 rl.angle -= 360;
	 }
	 if(rl.angle<=-360)
	 {
		 rl.angle += 360;
	 }
   rl.x += speed * t * cos(rl.angle);
   rl.y += speed * t * sin(rl.angle); 
}


//===========================================  End Of File  ===========================================//
