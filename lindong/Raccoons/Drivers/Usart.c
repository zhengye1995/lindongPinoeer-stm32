/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/
#include "Usart.h"
#include <stdarg.h> 

/*----------------------------------------  F U N C T I O N S  ----------------------------------------*/
/********************************************************************************************************
Function Name: UsartIOInit
Description  :
Inputs       : None
Outputs      : None
Notes        :
Revision     :
********************************************************************************************************/
void UsartIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);

//    GPIO_InitStructure.GPIO_Pin = UART1_TX_PIN;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(UART1_GPIO, &GPIO_InitStructure);

//    // Configure USART Rx as input floating
//    GPIO_InitStructure.GPIO_Pin = UART1_RX_PIN;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_Init(UART1_GPIO, &GPIO_InitStructure);

    // Configure USART Tx as alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = UART_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(UART_GPIO, &GPIO_InitStructure);

    // Configure USART Rx as input floating
    GPIO_InitStructure.GPIO_Pin = UART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART_GPIO, &GPIO_InitStructure);
}

/********************************************************************************************************
Function Name: USARTConfig
Description  :
Inputs       : None
Outputs      : None
Notes        :
Revision     :
********************************************************************************************************/
void UsartConfig(void)
{
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART4, &USART_InitStructure);
	  //USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);				//中断使能
    USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
   	USART_Cmd(UART4, ENABLE);

//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
//    USART_InitStructure.USART_BaudRate = 9600;
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;
//    USART_InitStructure.USART_Parity = USART_Parity_No;
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//    USART_ITConfig(UART1x,USART_IT_RXNE,ENABLE);
//    USART_Init(UART1x, &USART_InitStructure);
//    USART_Cmd(UART1x, ENABLE);
		
		
		
		//USART_ClearFlag(UART4, 0xFFFF);
		
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
static void BSP_dmaInit(void)
{
  //定义数据结构
  DMA_InitTypeDef DMA_InitStructure;
  //初始化时钟
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

	
	/*********************************串口三DMA陀螺双缓冲数据*********************************/
  DMA_DeInit(DMA2_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(UART4->DR));	 //????  
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)(UART4recdma);	 //????  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	 //????????? 
	DMA_InitStructure.DMA_BufferSize = 9;	 //DMA????:BufferSize 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	 //??????????  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	  	 //?????????	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 	//???????8? 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;	 //???????8? 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 //????????? 
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;	 //??DMA??????? 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	 //??DMA???????????? 
	

  DMA_Init(DMA2_Channel3, &DMA_InitStructure);
  
  DMA_Cmd(DMA2_Channel3, ENABLE);
  
  DMA_ClearFlag(DMA2_FLAG_TC3);
}
/********************************************************************************************************
Function Name: UsartInit
Description  :
Inputs       : None
Outputs      : None
Notes        :
Revision     :
********************************************************************************************************/
void UsartInit(void)
{
    UsartIOInit();
    UsartConfig();
	BSP_dmaInit();
}


/********************************************************************************************************
Function Name: USART1_IRQHandler
Description  :
Inputs       : None
Outputs      : None
Notes        :
Revision     :
********************************************************************************************************/
u8  rece;
void USART1_IRQHandler(void)
{
    if(RESET != USART_GetITStatus(UART1x,USART_IT_RXNE))
    {
        rece = USART_ReceiveData(UART1x);
				fun_usartSend(USART1, rece);
        USART_ClearITPendingBit(UART1x,USART_IT_RXNE);
    }
}
UINT8 Code_receive_speed(void)
{
		
	  if((UART4recdma[0]==0xFA)&&(UART4recdma[1]==0xAF)&&(UART4recdma[2]==0x06)&&(UART4recdma[3]==0x70))
		{
			unsigned char recnum = UART4recdma[4] + UART4recdma[5] + UART4recdma[6] + UART4recdma[7];
			if(recnum == UART4recdma[8])
			{
				UART4_RecvFin = 1;
			}
		}
}
//===========================================  End Of File  ===========================================//

