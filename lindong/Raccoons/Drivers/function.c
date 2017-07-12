/********************************************************************************************************
**                                 Copyright (c)          MEI14
**                                                 All Rights Reserved
**  
**                                 Email:729527658@qq.com   QQ:729527658
**-------------------------------------------------------------------------------------------------------
**  MCU        : STM32F407VG (STMicroelectronics)
**  Compiler   : Keil uVision 5.10
**  Module Name: common functions
**  Module Date: 2014-
**  Module Auth: 
**  Description: 
**  Version    : V1.0
**  Notes      : 
**-------------------------------------------------------------------------------------------------------
**  Version    : 
**  Modified By: 
**  Date       : 
**  Content    : 
**  Notes      : 
********************************************************************************************************/
/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/
#include "function.h"
/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/
/*----------------------------------------  F U N C T I O N S  ----------------------------------------*/
/********************************************************************************************************
Function Name: 
Author       : 
Date         : 2013-03-
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void fun_usartSend(USART_TypeDef *com ,u8 data)
{
  while(USART_GetFlagStatus(com, USART_FLAG_TC) == RESET)
  {}
  USART_SendData(com, (u8) data);
}

/********************************************************************************************************
Function Name: 
Author       : 
Date         : 2013-03-
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void fun_nvicSetNormal(u8 chanel, u8 mPrio, u8 subPrio)
{
  NVIC_InitTypeDef nvicInitStructure;
  nvicInitStructure.NVIC_IRQChannel = chanel;
  nvicInitStructure.NVIC_IRQChannelPreemptionPriority = mPrio;
  nvicInitStructure.NVIC_IRQChannelSubPriority = subPrio;
  nvicInitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvicInitStructure);
}


/********************************************************************************************************
Function Name: 
Author       : 
Date         : 2013-03-
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
u8 fun_usartRec(USART_TypeDef *com)
{
  while(USART_GetFlagStatus(com, USART_FLAG_RXNE) == RESET)
  {}
  return USART_ReceiveData(com);
}


//===========================================  End Of File  ===========================================//
