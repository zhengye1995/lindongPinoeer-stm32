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

#ifndef _FUNCTION_H
#define _FUNCTION_H

/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/
#include "Global.h"

/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/

/*--------------------------------------  D E C L A R A T I O N  --------------------------------------*/
/* Internal Variable */

/* External Variable */

/* Internal Function */

/* External Function */
void   fun_usartSend(USART_TypeDef *com ,u8 data);
u8     fun_usartRec(USART_TypeDef *com);
void   fun_nvicSetNormal(u8 chanel, u8 mPrio, u8 subPrio);

#endif
//===========================================  End Of File  ===========================================//
