/********************************************************************************************************
**                                 Copyright (c)   lindong EE
**                                                 All Rights Reserved
**  
**                                 Email:18813124313@qq.com   weixin:18813124313
**-------------------------------------------------------------------------------------------------------
**  MCU        : STM32F103RE (STMicroelectronics)
**  Compiler   : Keil uVision 5.16a
**  Module Name: Motor.c
**  Module Date: 2017-07-13
**  Module Auth: zhengye
**  Description: 
**  Version    : V1.0
**  Notes      : 
**-------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/
#include "Motor.h"

/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/

/*----------------------------------------  F U N C T I O N S  ----------------------------------------*/
/********************************************************************************************************
Function Name: SetMotor1PWM
Description  :
Inputs       : xpwm
Outputs      : None
Notes        :
Revision     :
********************************************************************************************************/
void SetMotor1PWM(s16 xpwm)
{

    if(xpwm < 0)
    {
        MOTOR1_DIR_L;
        TIM_SetCompare1(MOTOR_TIM,0-xpwm);
    }
    else	
    {
        MOTOR1_DIR_H;
        TIM_SetCompare1(MOTOR_TIM,2000-xpwm);
    }
}

/********************************************************************************************************
Function Name: SetMotor2PWM
Description  :
Inputs       : xpwm
Outputs      : None
Notes        :
Revision     :
********************************************************************************************************/
void SetMotor2PWM(s16 xpwm)
{
	xpwm = -xpwm;
    if(xpwm < 0)
    {
        MOTOR2_DIR_L;
        TIM_SetCompare3(MOTOR_TIM,0-xpwm);
    }
    else
    {
        MOTOR2_DIR_H;
        TIM_SetCompare3(MOTOR_TIM,2000-xpwm);
    }
}
/********************************************************************************************************
Function Name: SetMotor2PWM
Description  : PID�������
Inputs       : RefL RefR
Outputs      : None
Notes        :
Revision     :
********************************************************************************************************/
void Motor_CtrlLR(s32 RefL,s32 RefR)
{
	left.vi_Ref = RefL;
	left.vi_FeedBack = Speed_L_Get;//����
	right.vi_Ref = RefR;
	right.vi_FeedBack = Speed_R_Get;//�ҵ��
	SetMotor1PWM(V_PIDCalc(&left));
	SetMotor2PWM(V_PIDCalc(&right));
}
/********************************************************************************************************
Function Name: SetMotor2PWM
Description  : ����PIDֱ�ӵ����ֵ
Inputs       : RefL RefR
Outputs      : None
Notes        :
Revision     :
********************************************************************************************************/
void Motor_CtrlLR2(s32 RefL,s32 RefR)
{
	if(RefL>500)
	{
		RefL=500;
	}
	if(RefL<-500)
	{
		RefL=-500;
	}
	if(RefR>500)
	{
		RefL=500;
	}
	if(RefR<-500)
	{
		RefL=-500;
	}
	SetMotor1PWM(RefL);
	SetMotor2PWM(RefR);
}

/********************************************************************************************************
Function Name: SetMotor2PWM
Description  : �����ٶȣ���ͷ0xAA����β0x55
Inputs       : speedLsend speedRsend
Outputs      : None
Notes        : 
Revision     :
********************************************************************************************************/
void SpeedSeed(u8 speedLsend, u8 speedRsend)
{
	USART_SendData(UART4, 0xAA);
	while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET)
	{
		 ;
	}
	USART_SendData(UART4, speedLsend);
	while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET)
	{
		 ;
	}
	USART_SendData(UART4, speedRsend);
	while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET)
	{
		 ;
	}
	USART_SendData(UART4, 0X55);
	while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET)
	{
		 ;
	}
}	
//===========================================  End Of File  ===========================================//
