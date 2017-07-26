#ifndef __MAIN_H__
#define __MAIN_H__

/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/
#include "Global.h"

/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/
#define K 1500
#define Km 150000 //´ý±ê¶¨
#define carL 38
/*-------------------------------------------  M A C R O S  -------------------------------------------*/

/*--------------------------------------  D E C L A R A T I O N  --------------------------------------*/
/* Internal Variable */

/* External Variable */
extern float pitch,roll,yaw; 		//Å·À­½Ç
extern int MPU_RecvFin;
extern float yawG;
/* Internal Function */

/* External Function */
extern s16 recvSpeed;
extern s16 recvAngle;
extern int UART4_RecvFin;
extern float angle_gyro;
extern unsigned char UART4recdma[9];
#endif

//===========================================  End Of File  ===========================================//

