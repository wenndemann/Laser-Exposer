#ifndef SPEED_CNTR_H
#define SPEED_CNTR_H
    
#include "project.h"

typedef struct
{
  uint8 run_state :3;
  int8 dir;
  uint8 laser;
  uint32 step_delay;
  uint32 decel_start;
  int32 decel_val;
  uint32 min_delay;
  int32 accel_count;
  uint32 step_count;
  int32 act_pos;
  uint32 rest;
  uint32 last_accel_delay;
  uint32 spr;

  void (*Motor_Dir_Write)(uint8 dir);
  void (*Motor_Step_Write)(uint8 val);
  void (*Timer_WritePeriod)(uint32 period);
  void (*Timer_Start)(void);
  void (*Timer_Stop)(void);
} speedRampData;

#define T1_FREQ 1382400

#define CW  1
#define CCW -1

// Speed ramp states
#define STOP  0
#define ACCEL 1
#define DECEL 2
#define RUN   3

void speed_cntr_Move_Abs (uint32 pos, uint32 accel, uint32 decel, uint32 speed, speedRampData *srd);
void speed_cntr_Move (int32 step, uint32 accel, uint32 decel, uint32 speed, speedRampData *srd);
void speed_cntr_Init_Timer ();
void speed_cntr_Update(speedRampData *srd);
unsigned long my_sqrt (unsigned long v);
unsigned int min (unsigned int x, unsigned int y);
void speed_cntr_Motor_Pulse (speedRampData *srd);
void setLaser(int read);
void resetLaser();

//! Global status flags
extern struct GLOBAL_FLAGS status;

#endif
