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
void SysTick_Handler(void)
{
	bspData.systime++;
}

/********************************************************************************************************
Function Name: TIM3_IRQHandler
 
 
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
int num;
void TIM3_IRQHandler(void)//左右轮与实际相反// 中断频率1000Hz
{
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		num++;
		Speed_L_Get = GetEncoder2_Pulse();
		Speed_R_Get = GetEncoder1_Pulse();
		L_Sum_Step += ((float)Speed_L_Get);				
		R_Sum_Step += ((float)Speed_R_Get);		
//		Sum_Step = R_Sum_Step + L_Sum_Step;
		//if(num==5)//500ms
		//{
		//	num=0;  
//			if(MPU_RecvFin)
//			{
//				MPU_RecvFin = 0;
//				//Robot_Loaction((Speed_L_Get+Speed_R_Get)/2.0/Km, yaw);
//			}
		//}
	 } 
}
void TIM4_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		if(MPU_RecvFin)
			{
				MPU_RecvFin = 0;
				Cal_Robot_Location((Speed_L_Get+Speed_R_Get)/2.0/Km, yaw-yawG);
			}
	 } 
}

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/
