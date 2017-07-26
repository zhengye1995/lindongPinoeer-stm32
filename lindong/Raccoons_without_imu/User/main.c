/********************************************************************************************************
**                                 Copyright (c)   lindong EE
**                                                 All Rights Reserved
**  
**                                 Email:18813124313@qq.com   weixin:18813124313
**-------------------------------------------------------------------------------------------------------
**  MCU        : STM32F103RE (STMicroelectronics)
**  Compiler   : Keil uVision 5.16a
**  Module Name: main.c
**  Module Date: 2017-07-13
**  Module Auth: zhengye
**  Description: 
**  Version    : V1.0
**  Notes      : 
**-------------------------------------------------------------------------------------------------------*/
/*----------------------------------------  硬件连接 ----------------------------------------*/
/********************************************************************************************************
电源供电为12V电源
串口4  直接从芯片上飞线出来的唯一串口，之后小浣熊重新设计要注意
编码器1、2接两个电机码盘线，有防反插，具体顺序可以结合在线调试判断，没必要纠结
电机电源线A、B分别接左右电机，具体对应顺序可以像编码器一样调试判断
如有需要mpu9050等系列则接IIC接口
******************************************************************/

/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/
#include "main.h"

/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/
s16 recvSpeed = 0;
s16 recvAngle = 0;
int UART4_RecvFin = 0;
u8 speedLsend = 0;
u8 speedRsend = 0;
unsigned char UART4recdma[9]={0};

/*----------------------------------------  F U N C T I O N S  ----------------------------------------*/

/********************************************************************************************************
Function Name: 
Author       : zhengye
Date         : 2017-07-13
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/

int main(void)
{
	  V_PIDInit();   
    SystemInit();  //系统时钟初始
	  BSP_init(); 
		SetMotor1PWM(0);
		SetMotor2PWM(0);
		
		while(1)
		{
			speedLsend = Speed_L_Get;
			speedRsend = Speed_R_Get;
			SpeedSeed(speedLsend, speedRsend);
	    Code_receive_speed();
			if(UART4_RecvFin)
			{
				 UART4_RecvFin=0;
				 recvSpeed = (UART4recdma[7]<<8) + UART4recdma[6];
				 recvAngle = (UART4recdma[5]<<8) + UART4recdma[4];
			}
			if((recvSpeed!=0) || (recvAngle!=0))
			{
				 Motor_CtrlLR(recvSpeed + recvAngle, recvSpeed - recvAngle);
			}
			else
			{
					Motor_CtrlLR2(0, 0);
					V_PIDreset(&left);
					V_PIDreset(&right);
			}	
			Delay_ms(10);	
		}
}
//===========================================  End Of File  ===========================================//

