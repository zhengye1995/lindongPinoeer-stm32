/********************************************************************************************************
**                                 Copyright (c)   lindong EE
**                                                 All Rights Reserved
**  
**                                 Email:18813124313@qq.com   weixin:18813124313
**-------------------------------------------------------------------------------------------------------
**  MCU        : STM32F103RE (STMicroelectronics)
**  Compiler   : Keil uVision 5.16a
**  Module Name: Pid.c
**  Module Date: 2017-07-13
**  Module Auth: zhengye
**  Description: 
**  Version    : V1.0
**  Notes      : 
**-------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/
#include "pid.h"

/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/
s32 VV_MAX = 1800;     //���PID �������
s32 VV_MIN = -1800;    //���PID ������
s32 VV_DEADLINE = 3;   //�ٶ�PID������������Χ
PID left; //����PID
PID right; //�ҵ��PID

/********************************************************************************************************
Function Name: V_PIDInit
Description  : PID��ʼ��
Inputs       : None
Outputs      : None
Notes        : PID��ʼ��
Revision     :
********************************************************************************************************/
void V_PIDInit (void)
{
		left.v_Kp = 3;
		left.v_Ki = 1;
		left.v_Kd = 0;
		right.v_Kp = 3;
		right.v_Ki = 1;
		right.v_Kd = 0;
		V_PIDreset(&left);
		V_PIDreset(&right);
}
	
/********************************************************************************************************
Function Name: V_PIDreset
Description  : PID
Inputs       : None
Outputs      : None
Notes        : PID��λ
Revision     :
********************************************************************************************************/
void V_PIDreset(PID * pp)
{
	pp->motor_PreU=0;
	pp->PreI=0;
	pp->vi_FeedBack=0;
	pp->vi_PreDerror=0;
	pp->vi_PreError=0;
	pp->vi_Ref=0;
	pp->vl_PreU=0;
}
/********************************************************************************************************
Function Name:V_PIDCalc
Description  :
Inputs       : None
Outputs      : None
Notes        : �ٶ�PID����
Revision     :
********************************************************************************************************/
s32 V_PIDCalc( PID *pp )
{
    SINT32 error = 0;
		SINT32 d_error = 0;
	  //SINT32 dd_error = 0;
	  SINT32 PreIntegral = 0;
	
    error = pp->vi_Ref - pp->vi_FeedBack;	// ƫ�����
    d_error = error - pp->vi_PreError;
    //dd_error = d_error - pp->vi_PreDerror;
    PreIntegral = pp->PreI+ error;
	
    pp->vi_PreError = error;		//�洢��ǰƫ��
    pp->vi_PreDerror = d_error;
	  pp->PreI = PreIntegral;

    if( ( error < VV_DEADLINE ) && ( error > -VV_DEADLINE ) )	//���õ�������
		{
				pp->motor_PreU = pp->vl_PreU;
		}
    else								//�ٶ�PID ����
    {
       pp->motor_PreU =  (pp -> v_Kp * error + (SINT32)((pp -> v_Ki *  PreIntegral)) + pp->v_Kd * d_error) >> 1;
    }

    if( pp->motor_PreU >= VV_MAX ) 		//�ٶ�PID ����ֹ����������
    {
        pp->motor_PreU = VV_MAX;
    }
    else if( pp->motor_PreU <= VV_MIN)	//�ٶ�PID ����ֹ����������
    {
        pp->motor_PreU = VV_MIN;
    }
		pp->vl_PreU = pp->motor_PreU;
    return (pp->motor_PreU);		//  ����Ԥ����ռ�ձ�
}
