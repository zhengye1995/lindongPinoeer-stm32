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
Function Name: Motor_CtrlLR
Description  : PID电机控制
Inputs       : RefL RefR
Outputs      : None
Notes        :
Revision     :
********************************************************************************************************/
void Motor_CtrlLR(s32 RefL,s32 RefR)
{
	left.vi_Ref = RefL;
	left.vi_FeedBack = Speed_L_Get;//左电机
	right.vi_Ref = RefR;
	right.vi_FeedBack = Speed_R_Get;//右电机
	SetMotor1PWM(V_PIDCalc(&left));
	SetMotor2PWM(V_PIDCalc(&right));
}
/********************************************************************************************************
Function Name: Motor_CtrlLR2
Description  : 不带PID直接电机赋值
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
Function Name: SpeedSeed
Description  : 发送速度，包头0xAA，包尾0x55
Inputs       : speedLsend speedRsend
Outputs      : None
Notes        : 
Revision     :
********************************************************************************************************/
void SpeedSeed(s8 speedLsend, s8 speedRsend, s16 angle)
{
	u8 Lflag = 0;
	u8 Rflag = 0;
	if(speedLsend >= 0)
	{
		Lflag = 0;
	}
	else
	{
		Lflag = 1;
	}
	if(speedRsend >= 0)
	{
		Rflag = 0;
	}
	else
	{
		Rflag = 1;
	}
	fun_usartSend(UART4, 0xA5);
	fun_usartSend(UART4, Lflag);
	fun_usartSend(UART4, Rflag);
	fun_usartSend(UART4, speedLsend);
	fun_usartSend(UART4, speedRsend);
	fun_usartSend(UART4, angle);
	fun_usartSend(UART4, 0x5A);
}
/********************************************************************************************************
Function Name: SpeedSeed
Description  : 发送速度，包头0xAA，包尾0x55
Inputs       : speedLsend speedRsend
Outputs      : None
Notes        : 
Revision     :
********************************************************************************************************/
void TwistSeed(float x, float y, float yaw, float linerX, float angularZ)
{
	u8 xflag = 0;
	u8 yflag = 0;
	u8 yawflag = 0;
	u8 linerXflag = 0;
	u8 angularZflag = 0;
	x = (int)(x*100);
	y = (int)(y*100);
	yaw = (int)(yaw*100);
	linerX = (int)(linerX*100);
	angularZ = (int)(angularZ*100);
	if(x >= 0)
	{
		xflag = 0;
	}
	else
	{
		xflag = 1;
	}
	if(y >= 0)
	{
		yflag = 0;
	}
	else
	{
		yflag = 1;
	}
	if(yaw >= 0)
	{
		yawflag = 0;
	}
	else
	{
		yawflag = 1;
	}
	if(linerX >= 0)
	{
		linerXflag = 0;
	}
	else
	{
		linerXflag = 1;
	}
	if(angularZ >= 0)
	{
		angularZflag = 0;
	}
	else
	{
		angularZflag = 1;
	}
	fun_usartSend(UART4, 0xAA);
	fun_usartSend(UART4, xflag);
	fun_usartSend(UART4, yflag);
	fun_usartSend(UART4, yawflag);
	fun_usartSend(UART4, linerXflag);
	fun_usartSend(UART4, angularZflag);
  fun_usartSend(UART4, ((int)ABS(x) & 0xff00)>>8);
  fun_usartSend(UART4, ((int)ABS(x) & 0x00ff));
	fun_usartSend(UART4, ((int)ABS(y) & 0xff00)>>8);
  fun_usartSend(UART4, ((int)ABS(y) & 0x00ff));
	fun_usartSend(UART4, ((int)ABS(yaw) & 0xff00)>>8);
  fun_usartSend(UART4, ((int)ABS(yaw) & 0x00ff));
	fun_usartSend(UART4, ((int)ABS(linerX) & 0xff00)>>8);
  fun_usartSend(UART4, ((int)ABS(linerX) & 0x00ff));
	fun_usartSend(UART4, ((int)ABS(angularZ) & 0xff00)>>8);
  fun_usartSend(UART4, ((int)ABS(angularZ) & 0x00ff));
	fun_usartSend(UART4, 0x55);
}
//===========================================  End Of File  ===========================================//
