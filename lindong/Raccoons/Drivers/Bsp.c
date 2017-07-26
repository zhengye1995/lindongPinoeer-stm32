/********************************************************************************************************
**                                 Copyright (c)   2017 灵动科技
**                                                 All Rights Reserved
**  
**                                 Email:18813124313@163.com   Tel:18813124313
**-------------------------------------------------------------------------------------------------------
**  MCU        : STM32F103RE (STMicroelectronics)
**  Compiler   : Keil uVision 5.16a
**  Module Name: Board Support Package
**  Module Date: 2017-06-26
**  Module Auth: zhengye
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
#include "Bsp.h"
/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
      set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
BSP_DATA bspData;

/*----------------------------------------  F U N C T I O N S  ----------------------------------------*/
	/********************************************************************************************************
Function Name: BSP_init
Author       : zhengye
Date         : 2017-06-26
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void BSP_init(void)
{
	  Systick_Init();
    BSP_GPIOinit();
    BSP_usartInit();
    BSP_timerInit();
    BSP_nvicInit();
    Bsp_adcInit();
    BSP_dmaInit();
    BSP_spiInit();
    Bsp_extiInit();
}
/********************************************************************************************************
Function Name: BSP_GPIOinit
Author       : zhengye
Date         : 2017-06-26
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
static void BSP_GPIOinit(void)
{
		//定义数据结构
		GPIO_InitTypeDef GPIO_InitStructure;
		
		//时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
    //PA0 PA2 电机PWM控制 
	  GPIO_InitStructure.GPIO_Pin = MOTOR1_PWM_PIN|MOTOR2_PWM_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_GPIO, &GPIO_InitStructure);
		
	  //PA1 PA3 电机转向控制
    GPIO_InitStructure.GPIO_Pin  = MOTOR1_DIR_PIN|MOTOR2_DIR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_GPIO, &GPIO_InitStructure);
		
	  //PB13-14  电机编码器A、B相位 TIM1
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	  //PC6-7  电机编码器A、B相位 TIM8
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		//UART4从芯片上直接飞线引出，没有硬件滤波（唯一引出的串口，与上位机通信，切勿供电）
		//PC10 UART4 TX 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //PC11 UART4 RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/********************************************************************************************************
Function Name: BSP_usart
Author       : zhengye
Date         : 2017-06-26
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
static void BSP_usartInit(void)
{
  //定义数据结构
	USART_InitTypeDef USART_InitStructure;
  
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	USART_Init(UART4, &USART_InitStructure);
	//USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);				//串口中断
	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);//UART4 接收数据DMA
	USART_Cmd(UART4, ENABLE);
}
/********************************************************************************************************
Function Name: BSP_dma
Author       : zhengye
Date         : 2017-06-26
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
static void BSP_dmaInit(void)
{
  //定义数据结构
  DMA_InitTypeDef DMA_InitStructure;
  //初始化时钟
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

	
	/*********************************串口四DMA双缓冲数据*********************************/
  DMA_DeInit(DMA2_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(UART4->DR));	 
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)(UART4recdma);	  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	 
	DMA_InitStructure.DMA_BufferSize = 9;	 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	  	 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;	 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;	 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	 
	
  DMA_Init(DMA2_Channel3, &DMA_InitStructure);
  
  DMA_Cmd(DMA2_Channel3, ENABLE);
  
  DMA_ClearFlag(DMA2_FLAG_TC3);
}
/********************************************************************************************************
Function Name: BSP_timer
Author       : zhengye
Date         : 2017-06-26
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
static void BSP_timerInit(void)
{
  //数据类型定义
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//基本定时对象
  TIM_ICInitTypeDef TIM_ICInitStructure;//输入捕获对象
	TIM_OCInitTypeDef TIM_OCInitStructure; 
  
  //TIM3  用于10MS中断定时
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
  TIM_TimeBaseStructure.TIM_Period = 10000 - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
	
	 //TIM4  用于10mS中断定时 MPU9250
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
  TIM_TimeBaseStructure.TIM_Period = 10000 - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM4, ENABLE);
	
	//TIM1 TIM8 Encoder
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	/***********************   ENCODER2  *************************/
	TIM_DeInit(TIM1);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0;  // No prescaling
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12,
														 TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);	  //  编码器计数方式
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;	 //   滤波器
	TIM_ICInit(TIM1, &TIM_ICInitStructure);
	TIM_SetCounter(TIM1, 0);
	TIM_Cmd(TIM1, ENABLE);

	/**********************  ENCODER1  *************************/
	TIM_DeInit(TIM8);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0;  // No prescaling
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12,
														 TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	TIM_SetCounter(TIM8, 0);
	TIM_Cmd(TIM8, ENABLE);
	
	/********************** Motor PWM *************************/
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_DeInit(MOTOR_TIM);
    TIM_TimeBaseStructure.TIM_Prescaler = 4;                               // 系统  72MHz    TIM时钟= 72MHz/5=14400000
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = MOTOR_Period;                         // Frequency = 14400000 / 2000=7200
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(MOTOR_TIM, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

    TIM_OCInitStructure.TIM_Pulse = MOTOR_Period >> 1;                       // TIM2 dutycycle = 50%
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

    TIM_OC1Init(MOTOR_TIM, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(MOTOR_TIM, TIM_OCPreload_Enable);

    TIM_OC3Init(MOTOR_TIM, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(MOTOR_TIM, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(MOTOR_TIM,ENABLE);
    TIM_Cmd(MOTOR_TIM, ENABLE);
}

/********************************************************************************************************
Function Name: BSP_nvic
Author       : zhengye
Date         : 2017-06-26
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
static void BSP_nvicInit(void)
{
  // 中断方案选择
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  // 中断初始化
  fun_nvicSetNormal(TIM3_IRQn, 0, 1);
	fun_nvicSetNormal(TIM4_IRQn, 1, 2);
  //fun_nvicSetNormal(UART4_IRQn, 0, 1);
}

/********************************************************************************************************
Function Name: BSP_spi
Author       : zhengye
Date         : 2017-06-26
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
static void BSP_spiInit(void)
{
  //数据类型定义
  //SPI_InitTypeDef spiInitStructure;
/*
 *****************************************************************
  example of SPI
 *****************************************************************
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);//初始化时钟
  spiInitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//双线全双工
  spiInitStructure.SPI_Mode = SPI_Mode_Master;//主机模式
  spiInitStructure.SPI_DataSize = SPI_DataSize_8b;//数据格式
  spiInitStructure.SPI_CPOL = SPI_CPOL_High;//时钟极性
  spiInitStructure.SPI_CPHA = SPI_CPHA_2Edge;//触发沿
  spiInitStructure.SPI_NSS = SPI_NSS_Soft;//片选信号软件控制
  spiInitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//波特率分频
  spiInitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//大小头
  //spiInitStructure.SPI_CRCPolynomial = 7;//CRC校验位数
  SPI_Init(SPI5, &spiInitStructure);//初始化 
  SPI_Cmd(SPI5, ENABLE);//使能
 ******************************************************************
*/
}
/********************************************************************************************************
Function Name: BSP_exti
Author       : zhengye
Date         : 2017-06-26
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
static void Bsp_extiInit(void)
{
  //数据类型定义
  //EXTI_InitTypeDef exti;
/*
 *****************************************************************
  example of EXTI
 *****************************************************************
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//时钟初始化
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource12);//外部中断管脚选择
  exti.EXTI_LineCmd = ENABLE;
  exti.EXTI_Mode = EXTI_Mode_Interrupt;
  exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  exti.EXTI_Line = EXTI_Line12;
  EXTI_Init(&exti);
 ******************************************************************
*/
}
/********************************************************************************************************
Function Name: BSP_adc
Author       : zhengye
Date         : 2017-06-26
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
static void Bsp_adcInit(void)
{
  //数据类型定义
  //ADC_InitTypeDef       ADC_InitStructure;
  //ADC_CommonInitTypeDef ADC_CommonInitStructure;
/*
 *****************************************************************
  example of ADC
 *****************************************************************
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  // ADC Common Init *********************************************************
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  // ADC3 Init ***************************************************************
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 2;
  ADC_Init(ADC1, &ADC_InitStructure);

  // ADC3 regular channel7 configuration ************************************
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_3Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_3Cycles);
  
 // Enable DMA request after last transfer (Single-ADC mode) 
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

  // Enable ADC3 DMA 
  ADC_DMACmd(ADC1, ENABLE);

  // Enable ADC3 
  ADC_Cmd(ADC1, ENABLE);
  
  // Start ADC1 Software Conversion
  ADC_SoftwareStartConv(ADC1);
 ******************************************************************
*/

}
/********************************************************************************************************
Function Name: delay_init
Author       : zhengye
Date         : 2017-06-26
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void delay_init(void)
{
	if (SysTick_Config(SystemCoreClock / 1000000))
  { 
    /* Capture error */ 
    while (1);
  }
}
/********************************************************************************************************
Function Name: Delay_us
Author       : zhengye
Date         : 2017-06-26
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void Delay_us(u32 us)
{
	bspData.systime=0;
	while(bspData.systime<=us)
	{
		;
	}
} 
/********************************************************************************************************
Function Name: Delay_ms
Author       : zhengye
Date         : 2017-06-26
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void Delay_ms(u32 ms)
{
	bspData.systime=0;
	while(bspData.systime<=ms*1000)
	{
		;
	}
} 
/********************************************************************************************************
Function Name: Systick_Init
Author       : zhengye
Date         : 2017-06-26
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void Systick_Init(void)
{
  if (SysTick_Config(SystemCoreClock / 1000000))
  { 
    /* Capture error */ 
    while (1);
  }
}
/********************************************************************************************************
Function Name: Code_receive_speed
Author       : zhengye
Date         : 2017-06-26
Description  : 
Inputs       : None
Outputs      : None
Notes        : 
Revision     : 
********************************************************************************************************/
void Code_receive_speed(void)
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
