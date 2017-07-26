/********************************************************************************************************
**                                 Copyright (c)   lindong EE
**                                                 All Rights Reserved
**  
**                                 Email:18813124313@qq.com   weixin:18813124313
**-------------------------------------------------------------------------------------------------------
**  MCU        : STM32F103RE (STMicroelectronics)
**  Compiler   : Keil uVision 5.16a
**  Module Name: Config.h
**  Module Date: 2017-07-13
**  Module Auth: zhengye
**  Description: 
**  Version    : V1.0
**  Notes      : 
**-------------------------------------------------------------------------------------------------------*/
#ifndef __CONFIG_H__
#define __CONFIG_H__

/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/

/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/

/*-------------------------------------------  M A C R O S  -------------------------------------------*/

/* DataType Define */
typedef unsigned char                  UINT8;       //            0 ~ 255
typedef   signed char                  SINT8;       //         -128 ~ 127
typedef unsigned short                 UINT16;      //            0 ~ 65535
typedef   signed short                 SINT16;      //       -32768 ~ 32767
typedef unsigned long                  UINT32;      //            0 ~ 4294967295
typedef   signed long                  SINT32;      //  -2147483648 ~ 2147483647

typedef unsigned char                  INT8U;       //            0 ~ 255
typedef   signed char                  INT8S;       //         -128 ~ 127
typedef unsigned short                 INT16U;      //            0 ~ 65535
typedef   signed short                 INT16S;      //       -32768 ~ 32767
typedef unsigned long                  INT32U;      //            0 ~ 4294967295
typedef   signed long                  INT32S;      //  -2147483648 ~ 2147483647

typedef unsigned char                  uint8;       //            0 ~ 255
typedef   signed char                  sint8;       //         -128 ~ 127
typedef unsigned short                 uint16;      //            0 ~ 65535
typedef   signed short                 sint16;      //       -32768 ~ 32767
typedef unsigned long                  uint32;      //            0 ~ 4294967295
typedef   signed long                  sint32;      //  -2147483648 ~ 2147483647

typedef unsigned char                  int8u;       //            0 ~ 255
typedef   signed char                  int8s;       //         -128 ~ 127
typedef unsigned short                 int16u;      //            0 ~ 65535
typedef   signed short                 int16s;      //       -32768 ~ 32767
typedef unsigned long                  int32u;      //            0 ~ 4294967295
typedef   signed long                  int32s;      //  -2147483648 ~ 2147483647

typedef float                          FP32;
typedef double                         FP64;

/* Const Define */
#ifndef ON
#define ON                             (1)
#endif

#ifndef OFF
#define OFF                            (0)
#endif

//#ifndef TRUE
//#define TRUE                           (1)
//#endif

#ifndef FAULSE
#define FAULSE                         (0)
#endif

/* MIN/MAX/ABS Macros */
#define MIN(a, b)                      (((a) < (b))?(a):(b))
#define MAX(a, b)                      (((a) > (b))?(a):(b))
#define ABS(x)                         (((x) > 0)?(x):(-(x)))

/* Constants */
#define PI                             3.14159265359

/* Function Define */
#ifndef ClrBit
#define ClrBit(reg, bit)               reg &= ~(1 << bit)
#endif

#ifndef SetBit
#define SetBit(reg, bit)               reg |= (1 << bit)
#endif

#endif
typedef struct			//����������������
{
    SINT32 vi_Ref;		//�ٶ�PID���ٶ��趨ֵ
    SINT32 vi_FeedBack;		//�ٶ�PID���ٶȷ���ֵ


    SINT32 vi_PreError;	  	//�ٶ�PID��ǰһ�Σ��ٶ����,,vi_Ref - vi_FeedBack
    SINT32 vi_PreDerror;	//�ٶ�PID��ǰһ�Σ��ٶ����֮�d_error-PreDerror;
		SINT32 PreI;    //֮ǰ��Iֵ
	
    SINT32 v_Kp;		//�ٶ�PID��Ka = Kp
    float v_Ki;		//�ٶ�PID��Kb = Kp * ( T / Ti )
    SINT32 v_Kd;		//�ٶ�PID��

    SINT32 vl_PreU;  //�������
    SINT32 motor_PreU;
}PID;
typedef struct
{
	unsigned char flag_mag_cali;
	unsigned char flag_send_mag;
	short speed;
	short direction;
	int soft_watchdog;
}sys_status_t;
//===========================================  End Of File  ===========================================//

