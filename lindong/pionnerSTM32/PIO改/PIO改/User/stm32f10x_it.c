/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

//struct IRState InfraredSensorState;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

/********************************************************************************************************
Function Name: SysTick_Handler
 
 
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
//void SysTick_Handler(void)
//{

//}

/********************************************************************************************************
Function Name: TIM2_IRQHandler
 
 
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void TIM2_IRQHandler(void) 
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
    { 
        TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update); 
    }	
}

/********************************************************************************************************
Function Name: TIM4_IRQHandler
 
 
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void TIM4_IRQHandler(void) 
{    
    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    { 
        TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update); 
    }  
}

void UART4_IRQHandler(void)
{
  if (USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
  {
		uint8_t dat;
		static unsigned char rCnt = 0;
				USART_ClearITPendingBit(UART4, USART_IT_RXNE);
				dat = USART_ReceiveData(UART4);
				UART4rec[rCnt] = dat;

		  if(rCnt == 0)     //??0xAA    
	    {
       		rCnt = (0xFA != dat)?0:rCnt+1;
	    }
	    else if(rCnt == 1) //??0x55  
	    {
				rCnt = (0xAF != dat)?0:rCnt+1;
	    }
	    else if(rCnt == 2) //??type
	    {
				rCnt = (0x06 != dat)?0:rCnt+1;
      }
	    else if(rCnt == 3) //??len
	    {
				rCnt = (0x70 != dat)?0:rCnt+1;
      }			
	    else if(rCnt >3 ) //?value
	    {
				if(rCnt == 8)
			  { 
					unsigned char recnum = UART4rec[4] + UART4rec[5] + UART4rec[6] + UART4rec[7];
					if(recnum == UART4rec[8])
					{
					  UART4_RecvFin = 1;  //
					}
					rCnt = 0;
			  }
			  else
			  {
				  rCnt++;
			  } 
			}				
				//uart4_receiveProcessNew(dat);
  }
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
