#ifndef __MAIN_H__
#define __MAIN_H__

/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/
#include "Global.h"

/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/

/*-------------------------------------------  M A C R O S  -------------------------------------------*/

/*--------------------------------------  D E C L A R A T I O N  --------------------------------------*/
/* Internal Variable */

/* External Variable */

/* Internal Function */
void SystemStart(void);

void Test_InputIO(void); //Êı×ÖÊäÈë²âÊÔ
void Test_OutputIO(void);//Êı×ÖÊä³ö²âÊÔ
void Test_Sensor(void);  //Ä£ÄâÊäÈë²âÊÔ
void Test_Usart(void);   //´®¿Ú²âÊÔ
void Test_ZLG7290(void); //¼üÅÌ²âÊÔ
void Test_Motor(void);   //µç»ú²âÊÔ
void Test_Servo(void);   //¶æ»ú²âÊÔ
void Test_Encoder(void); //±àÂëÆ÷²âÊÔ
void Test_V_PID(void);   //µç»ú±Õ»·²âÊÔ
void Motor_CtrlLR(s32 RefL,s32 RefR);
void Motor_CtrlLR2(s32 RefL,s32 RefR);

/* External Function */
extern s16 recvSpeed;
extern s16 recvAngle;
extern unsigned char UART4rec[9];
extern int UART4_RecvFin;
extern void SpeedSeed(void);
extern unsigned char UART4recdma[9];
#endif

//===========================================  End Of File  ===========================================//

