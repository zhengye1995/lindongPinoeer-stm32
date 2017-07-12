#include "main.h"

/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/

int Test_count = 0;
u8  Test_InputIO_Data[16]={0};
u16 Test_Sensor_Data[5]  ={0};
int Test_ZLG7290_Data = 0;
int Test_Motor_PWM=1000;
int Test_Sover_PWM = 1000;
int speedEx = 0;
int turnFlag = 0;
char turnState = 0, lastState = 0;
int speedLeft = 50, speedRight = 50;
int speedBase = 500;
int jkFlag = 0 , lastFlag = 0;
s16 recvSpeed = 0;
s16 recvAngle = 0;
int deltaSpeed = 0;
extern u8 rece;
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
		int i,j;
	  //V_PIDInit();
	  final_PIDInit();   
    SystemInit();  //系统时钟初始化 
		TimerInit();
    //InputIOInit(); //数字输入初始化
 		//OutputIOInit();//数字输出初始化
	 	//SensorInit();  //模拟输入初始化
	
 		//ZLG7290Init(); //键盘初始化
	
 		MotorInit();	 //电机初始化
 		//ServoInit();   //舵机初始化
	
		EncoderInit(); //编码器初始化
	
		UsartInit();
		SetMotor1PWM(0);
		SetMotor2PWM(0);
		//init_mpu9250();
			//rrr = 0x55;
		//init_data_save();
		//data_read();
		
		
		system_init();
		//Motor_CtrlLR2(200,200);
		int flagg=0;
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
			Delay_ms(500);	
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
/********************************************************************************************************
Function Name: SystemStart
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
/********************************************************************************************************
Function Name: Test_InputIO
Description  : 数字输入测试
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void Test_InputIO(void)
{
	int i = 0;
	while(1)
	{
		for( i=0 ; i<16 ; i++ )
		{
			Test_InputIO_Data[i] = GetKeyState(i+1);
		}
	}
}
/********************************************************************************************************
Function Name: Test_OutputIO
Description  : 数字输出测试
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void Test_OutputIO(void)
{
	
	while(1)
	{
		
		SetHigh(1);SetHigh(2);SetHigh(3);SetHigh(4);SetLow(5);SetLow(6);SetLow(7);SetLow(8);
		
		Delay_ms(1000);

		SetLow(1);SetLow(2);SetLow(3);SetLow(4);SetHigh(5);SetHigh(6);SetHigh(7);SetHigh(8);
		
		Delay_ms(1000);	
	}
}
/********************************************************************************************************
Function Name: Test_Sensor
Description  : 模拟输入测试
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void Test_Sensor(void)
{
	int i = 0;
	while(1)
	{
		for( i=1 ; i<6 ; i++ )
		{
			Test_Sensor_Data[i] = GetADCValue(i);
			
			Delay_ms(3);
		}
	}
}
/********************************************************************************************************
Function Name: Test_Usart
Description  : 串口测试
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void Test_Usart(void)
{
	while(1)
	{
		SendData(0xf0);//串口0
		Send1Data(0xf1);//串口1
		
		Delay_ms(500);
	}
}
/********************************************************************************************************
Function Name: Test_ZLG7290
Description  : 键盘测试
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/

	
/********************************************************************************************************
Function Name: Test_Motor
Description  : 电机测试
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void Test_Motor(void)
{
	while(1)
	{
		SetMotor1PWM(Test_Motor_PWM);
		SetMotor2PWM(Test_Motor_PWM);
	}
}	
/********************************************************************************************************
Function Name: Test_Servo
Description  : 舵机测试
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void Test_Servo(void)
{
	while(1)
	{
		SetServo1PWM(Test_Sover_PWM);
		SetServo2PWM(Test_Sover_PWM);
	}
}	
/********************************************************************************************************
Function Name: Test_Encoder
Description  : 编码器测试
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void Test_Encoder(void)
{
	while(1)
	{
		GetSpeed();
		Delay_ms(500);
	}
}	
/********************************************************************************************************
Function Name: Test_V_PID
Description  : 电机闭环测试
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void Test_V_PID(void)
{
	
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

