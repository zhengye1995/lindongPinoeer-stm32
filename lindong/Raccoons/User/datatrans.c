#include "datatrans.h"


#define USART4_BUF_NUM 64
unsigned char USART4_TX_BUF[USART4_BUF_NUM] = {0};
unsigned char USART4_RX_BUF[USART4_BUF_NUM] = {0};
unsigned char * u4_dat_buf = USART4_RX_BUF;


void uart4_receiveProcess(unsigned char dat)
{
    static unsigned char u4_sta = 0;
    static unsigned char u4_cnt = 0;
    static unsigned char u4_len = 0;
    
    switch(u4_sta)
    {
        case 0:                                                                 // 帧头 0xFA
            if(0xFA == dat)
            {
                u4_sta = 1;
                u4_dat_buf[u4_cnt ++] = 0xFA;
            }
            else
            {
                u4_sta = 0;
                u4_cnt = 0;
                u4_len = 0;
            }
            break;

        case 1:                                                                 // 帧头 0xAF
            if(0xAF == dat)
            {
                u4_sta = 2;
                u4_dat_buf[u4_cnt ++] = 0xAF;
            }
            else
            {
                u4_sta = 0;
                u4_cnt = 0;
                u4_len = 0;
            }
            break;

        case 2:                                                                 // 数据长度 含帧头帧尾
            if( (dat < 200) && (dat > 2))                                       // 这里可以改一下最大数据长度 ，也可以不做判断
            {
                u4_sta = 3;
                u4_len = dat;
                u4_dat_buf[u4_cnt++] = dat;
            }
            else
            {
                u4_sta = 0;
                u4_cnt = 0;
                u4_len = 0;
            }
            break;

        case 3:                                                                 // 功能字
            u4_sta = 4;
            u4_dat_buf[u4_cnt++] = dat;
            break;

        case 4:                                                                 // 数据
            if(u4_len-- > 2)
            {
                u4_dat_buf[u4_cnt++] = dat;
            }
            if(u4_len == 2)
            {
                u4_sta = 5;
            }
            break;

        case 5:                                                                 // sum校验和
            u4_dat_buf[u4_cnt++] = dat;
						DT_Analysis(u4_dat_buf, u4_cnt);
						u4_sta = 0;
						u4_len = 0;
						u4_cnt = 0;
            break;
        default:
            u4_sta = 0;
            u4_cnt = 0;
            u4_len = 0;
    }
}

void DT_Analysis(unsigned char * data_buf, unsigned char len)
{
    unsigned char sum = 0;
    unsigned char i;

    for(i = 2; i < len - 1; i++)
    {
        sum += *(data_buf + i);
    }
    if(sum != data_buf[len-1]) return ;

    if(*(data_buf + 3) == 0x70)     // 视觉坐标
    {
			local_data.soft_watchdog = 100;
			local_data.direction = (short)(((*(data_buf + 5) )<< 8) + (*(data_buf + 4))); 
			local_data.speed = (short)(((*(data_buf + 7) )<< 8) + (*(data_buf + 6))); 
    }
		if(*(data_buf + 3) == 0x71)
		{
			if( *(data_buf + 4) == 0x01)
			{
				local_data.flag_mag_cali = 1;
			}
		}
}

void DT_U4_Send(void)
{
	static int u4_tx_cnt = 0;	
	
	if(local_data.flag_send_mag == 1)
	{
		//if(USART_GetFlagStatus(UART4, USART_FLAG_IDLE))
		//if((USART_GetFlagStatus(UART4, USART_FLAG_TXE)!=0) && (USART_GetFlagStatus(UART4, USART_FLAG_RXNE)))
		{
			USART_SendData(UART4, USART4_TX_BUF[u4_tx_cnt]);
			u4_tx_cnt ++;
			USART_ClearFlag(UART4, USART_FLAG_IDLE);
		
		}
		
		if(u4_tx_cnt > 6)
		{
			u4_tx_cnt = 0;
			local_data.flag_send_mag = 0;
		}
	}
		
}



void DT_SetMagData(void)
{
	unsigned char sum = 0;
	unsigned char i;


	USART4_TX_BUF[0] = 0xFA;
	USART4_TX_BUF[1] = 0xAF;
	USART4_TX_BUF[2] = 0x03;
	USART4_TX_BUF[3] = 0x72;	
	USART4_TX_BUF[5] = (unsigned char)(((int)Angle[2] & 0x0000FF00) >> 8);
	USART4_TX_BUF[4] = (unsigned char)((int)Angle[2] & 0x000000FF);
	for(i = 2; i < 6; i++)
	{
			sum += USART4_TX_BUF[i];
	}	
	USART4_TX_BUF[6] = sum;
	local_data.flag_send_mag = 1;
}






