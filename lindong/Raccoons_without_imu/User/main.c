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
/*----------------------------------------  Ӳ������ ----------------------------------------*/
/********************************************************************************************************
��Դ����Ϊ12V��Դ
����4  ֱ�Ӵ�оƬ�Ϸ��߳�����Ψһ���ڣ�֮��С����������Ҫע��
������1��2��������������ߣ��з����壬����˳����Խ�����ߵ����жϣ�û��Ҫ����
�����Դ��A��B�ֱ�����ҵ���������Ӧ˳������������һ�������ж�
������Ҫmpu9050��ϵ�����IIC�ӿ�
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
    SystemInit();  //ϵͳʱ�ӳ�ʼ
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
