#ifndef _DATATRANS_H__
#define _DATATRANS_H__

#include "Global.h"


void DT_Analysis(unsigned char * data_buf, unsigned char len);
void uart4_receiveProcess(unsigned char dat);
void uart4_receiveProcessNew(unsigned char dat);
void DT_U4_Send(void);
void DT_SetMagData(void);


#endif
