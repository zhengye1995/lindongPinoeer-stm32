/********************************************************************************************************
**                                 Copyright (c)   lindong EE
**                                                 All Rights Reserved
**  
**                                 Email:18813124313@qq.com   weixin:18813124313
**-------------------------------------------------------------------------------------------------------
**  MCU        : STM32F103RE (STMicroelectronics)
**  Compiler   : Keil uVision 5.16a
**  Module Name: PID.h
**  Module Date: 2017-07-13
**  Module Auth: zhengye
**  Description: 
**  Version    : V1.0
**  Notes      : 
**-------------------------------------------------------------------------------------------------------*/
#ifndef __PID_H__
#define __PID_H__
/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/
#include "Global.h"

/*--------------------------------------  D E C L A R A T I O N  --------------------------------------*/
/* Internal Variable */

/* External Variable */


/* Internal Function */


/* External Function */
extern void V_PIDInit(void);
extern void V_PIDreset (PID * pp);
extern s32 V_PIDCalc( PID *pp );
extern PID left;
extern PID right;
#endif
