/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/
#include "IOoutput.h"

u16 OutPinTable[]= {0,OUT1_PIN,0,OUT3_PIN,OUT4_PIN,OUT5_PIN,OUT6_PIN,OUT7_PIN,OUT8_PIN};
/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/
/********************************************************************************************************
Function Name: OutputIOInit
Description  :
Inputs       :
Outputs      :
Notes        :
Revision     :
********************************************************************************************************/
void OutputIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = OUT1_PIN|OUT5_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OUT1_5_GPIO, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = OUT3_PIN|OUT4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OUT2_4_GPIO, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = OUT6_PIN|OUT7_PIN|OUT8_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OUT6_8_GPIO, &GPIO_InitStructure);
}

/********************************************************************************************************
Function Name: SetHigh
Description  :
Inputs       : UINT8 key -- 1,2,3.......8
Outputs      :
Notes        :
Revision     :
********************************************************************************************************/
void SetHigh(u8 key)
{
    if(key == 1||key == 5)
    {
        GPIO_SetBits(OUT1_5_GPIO,OutPinTable[key]);
    }
    else if(key == 2||key == 3||key == 4)
    {
        GPIO_SetBits(OUT2_4_GPIO,OutPinTable[key]);
    }
    else if(key == 6||key == 7||key == 8)
    {
        GPIO_SetBits(OUT6_8_GPIO,OutPinTable[key]);
    }
}

/********************************************************************************************************
Function Name: SetLow
Description  :
Inputs       : UINT8 key -- 1,2,3.......8
Outputs      :
Notes        :
Revision     :
********************************************************************************************************/
void SetLow(u8 key)
{
    if(key == 1||key == 5)
    {
        GPIO_ResetBits(OUT1_5_GPIO,OutPinTable[key]);
    }
    else if(key == 2||key == 3||key == 4)
    {
        GPIO_ResetBits(OUT2_4_GPIO,OutPinTable[key]);
    }
    else if(key == 6||key == 7||key == 8)
    {
        GPIO_ResetBits(OUT6_8_GPIO,OutPinTable[key]);
    }
}

//===========================================  End Of File  ===========================================//

