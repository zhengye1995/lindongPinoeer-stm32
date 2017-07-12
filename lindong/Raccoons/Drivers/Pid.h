
#ifndef __PID_H__
#define __PID_H__

#include "Global.h"



extern PID   L_PID;
extern PID   R_PID;
extern PID 	 POSE_PID;
extern PID 	 TURN_PID;
extern s32 V_PIDCalc(PID *pp);
extern s32 TURN_PIDCalc( PID *pp);
extern s32 POSE_PIDCalc( PID *pp);
extern void V_PIDInit(void);
extern void V_PIDreset (PID * pp);
extern void final_PIDInit(void);
extern PID left;
extern PID right;
#endif
