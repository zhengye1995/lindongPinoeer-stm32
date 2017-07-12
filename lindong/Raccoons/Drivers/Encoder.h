#ifndef __ENCODER_H__
#define __ENCODER_H__

/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/
#include "Global.h"

/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/


#define ENCODER1_A_PIN      GPIO_Pin_8
#define ENCODER1_B_PIN      GPIO_Pin_9
#define ENCODER1_GPIO       GPIOA

#define ENCODER_L_TIMER   	TIM8


#define ENCODER2_A_PIN      GPIO_Pin_6
#define ENCODER2_B_PIN      GPIO_Pin_7
#define ENCODER2_GPIO       GPIOC

#define ENCODER_R_TIMER    	TIM1

#define ENCODER_APP     	65535
#define COUNTER_RESET   	(0)
#define ICx_FILTER      	(6)
#define PULSE_MAX          32768

typedef struct Robot_Location			//定义数法核心数据
{
   float angle;
	 float x;
	 float y;
}Robot_Location;
/* External Function */
void EncoderIOInit(void);
void EncoderTIMInit(void);
void EncoderInit(void);
void GetSpeed(void);
SINT32 GetSpeedL(void);
SINT32 GetSpeedR(void);
//void GetSpeedL(void)；
//void GetSpeedR(void);

extern s32 Speed_L_Get;
extern s32 Speed_R_Get;
extern s32 Speed_L_Get_NEW;
extern s32 Speed_R_Get_NEW;
extern s32 Last_Speed_L_Get;
extern s32 Last_Speed_R_Get;
extern double R_Sum_Step;
extern double L_Sum_Step;
extern double Sum_Step;
extern int motorflag;
extern SINT32 GetEncoder1_Pulse(TIM_TypeDef *TIMx);
extern SINT32 GetEncoder2_Pulse(TIM_TypeDef *TIMx);
extern SINT32 Robot_Loaction(float speed, float Angular_velocity);
extern Robot_Location rl;
#endif

//===========================================  End Of File  ===========================================//

