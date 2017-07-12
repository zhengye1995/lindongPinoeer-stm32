#include "main.h"

/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/
s16 recvSpeed = 0;
s16 recvAngle = 0;
int deltaSpeed = 0;
unsigned char UART4rec[9]={0};
int UART4_RecvFin = 0;
int speedLeftGet = 0;
int speedRightGet = 0;
u8 speedLsend = 0;
u8 speedRsend = 0;
unsigned char UART4recdma[9]={0};
/*----------------------------------------  F U N C T I O N S  ----------------------------------------*/
/********************************************************************************************************
Function Name: main
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
unsigned char rrr = 255;

int main(void)
{
	  final_PIDInit();   
    SystemInit();  //系统时钟初始化 
		TimerInit();
	
 		MotorInit();	 //电机初始化
	
		EncoderInit(); //编码器初始化
	
		UsartInit();
		SetMotor1PWM(0);
		SetMotor2PWM(0);
		
		Systick_Init();
		while(1)
		{
			speedLsend = Speed_L_Get;
			speedRsend = Speed_R_Get;
			
			//Robot_Loaction((Speed_L_Get+Speed_R_Get)/2.0, (Speed_R_Get-Speed_L_Get)/2.0);
			SpeedSeed();
	    Code_receive_speed();
			if(UART4_RecvFin)
			{
				UART4_RecvFin=0;
				recvSpeed = (UART4recdma[7]<<8) + UART4recdma[6];
				recvAngle = (UART4recdma[5]<<8) + UART4recdma[4];
			}
			  if(recvSpeed != 0)
				{
					Motor_CtrlLR(recvSpeed + recvAngle, recvSpeed - recvAngle);
				}
				else
				{ 
					if(recvAngle!=0)
					{
						Motor_CtrlLR(recvSpeed + recvAngle, recvSpeed - recvAngle);
					}
					//Motor_CtrlLR(0, 0);
					else
					{
						Motor_CtrlLR2(0, 0);
					  V_PIDreset(&left);
					  V_PIDreset(&right);
					}	
				}
			//GetSpeed();
			//speedLeftGet = GetSpeedL();
			//speedRightGet = GetSpeedR();
			//GetSpeed();
			Delay_ms(10);	
			//GetSpeedR();
			
			
		}
}
void Motor_CtrlLR(s32 RefL,s32 RefR)
{
	left.vi_Ref = RefL;
	left.vi_FeedBack = Speed_L_Get;//左电机
	//V_PIDCalc(&left);
	right.vi_Ref = RefR;
	right.vi_FeedBack = Speed_R_Get;//右电机
	//V_PIDCalc(&right);
	SetMotor1PWM(V_PIDCalc(&left));
	SetMotor2PWM(V_PIDCalc(&right));
}
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


void SpeedSeed(void)
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

