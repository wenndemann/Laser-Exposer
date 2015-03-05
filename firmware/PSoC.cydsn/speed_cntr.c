#include "global.h"
#include "speed_cntr.h"
#include "laser.h"

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void speed_cntr_Move_Abs(uint32 pos, uint32 accel, uint32 decel, uint32 speed,
    speedRampData *srd) {
  speed_cntr_Move(pos - srd->act_pos, accel, decel, speed, srd);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void speed_cntr_Move(int32 step, uint32 accel, uint32 decel,
    unsigned long speed, speedRampData *srd) {
  unsigned int max_s_lim;
  unsigned int accel_lim;

  float ALPHA = (2 * 3.14159 / srd->stepsPerTurn); // 2*pi/stepsPerTurn
  long A_T_x100 = ((long)(ALPHA * T1_FREQ * 100)); // (ALPHA / T1_FREQ)*100
  int T1_FREQ_148 = ((int)((T1_FREQ * 0.676) / 100));
  // divided by 100 and scaled by 0.676
  long A_SQ = (long)(ALPHA * 2 * 10000000000); // ALPHA*2*10000000000
  int A_x20000 = (int)(ALPHA * 20000); // ALPHA*20000

  // Set direction from sign on step value.
  if (step < 0) {
    srd->dir = CCW;
    srd->Motor_Dir_Write(0);
    step *= -1;
  }
  else {
    srd->dir = CW;
    srd->Motor_Dir_Write(1);
  }

  // If moving only 1 step.
  if (step == 1) {
    srd->accel_count = -1;
    srd->run_state = DECEL;
    srd->step_delay = 1000;
  }
  // Only move if number of steps to move is not zero.
  else if (step != 0) {
    srd->min_delay = A_T_x100 / speed;
    srd->step_delay = (T1_FREQ_148 * my_sqrt(A_SQ / accel)) / 100;
    max_s_lim = (long) speed * speed / (long)(((long) A_x20000 * accel) / 100);

    // If we hit max speed limit before 0,5 step it will round to 0.
    // But in practice we need to move atleast 1 step to get any speed at all.
    if (max_s_lim == 0)
      max_s_lim = 1;

    // Find out after how many steps we must start deceleration.
    accel_lim = ((long) step * decel) / (accel + decel);

    // We must accelrate at least 1 step before we can start deceleration.
    if (accel_lim == 0)
      accel_lim = 1;

    // Use the limit we hit first to calc decel.
    if (accel_lim <= max_s_lim)
      srd->decel_val = accel_lim - step;
    else {
      srd->decel_val = ((long) max_s_lim * accel) / decel;
      srd->decel_val *= -1;
    }
    // We must decelrate at least 1 step to stop.
    if (srd->decel_val == 0)
      srd->decel_val = -1;

    // Find step to start decleration.
    srd->decel_start = step + srd->decel_val;

    // If the maximum speed is so low that we dont need to go via accelration state.
    if (srd->step_delay <= srd->min_delay) {
      srd->step_delay = srd->min_delay;
      srd->run_state = RUN;
    }
    else
      srd->run_state = ACCEL;

    // Reset counter.
    srd->accel_count = 0;

    srd->Timer_WritePeriod(50000);
    srd->Timer_Start();
  }
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
CY_ISR(Timer_X_ISR) {
  speed_cntr_Update(&srd_X);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
CY_ISR(Timer_Y_ISR) {
  speed_cntr_Update(&srd_Y);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void speed_cntr_Update(speedRampData *srd) {
  unsigned int new_step_delay = 0;

  srd->Timer_WritePeriod(srd->step_delay);

  switch (srd->run_state) {
  case STOP:
    resetLaser();
    srd->step_count = 0;
    srd->rest = 0;
    srd->Timer_Stop();
    break;

  case ACCEL:
    if (srd->laser == TRUE)
      setLaserByUart();
    speed_cntr_Motor_Pulse(srd);
    srd->step_count++;
    srd->accel_count++;
    srd->act_pos += srd->dir;
    new_step_delay = srd->step_delay -
        (((2 * (long) srd->step_delay) + srd->rest) /
        (4 * srd->accel_count + 1));
    srd->rest = ((2 * (long) srd->step_delay) + srd->rest) %
        (4 * srd->accel_count + 1);
    // Chech if we should start decelration.
    if (srd->step_count >= srd->decel_start) {
      srd->accel_count = srd->decel_val;
      srd->run_state = DECEL;
    }
    // Chech if we hitted max speed.
    else if (new_step_delay <= srd->min_delay) {
      srd->last_accel_delay = new_step_delay;
      new_step_delay = srd->min_delay;
      srd->rest = 0;
      srd->run_state = RUN;
    }
    break;

  case RUN:
    if (srd->laser == TRUE)
      setLaserByUart();
    speed_cntr_Motor_Pulse(srd);
    srd->step_count++;
    srd->act_pos += srd->dir;
    new_step_delay = srd->min_delay;
    // Chech if we should start decelration.
    if (srd->step_count >= srd->decel_start) {
      srd->accel_count = srd->decel_val;
      // Start decelration with same delay as accel ended with.
      new_step_delay = srd->last_accel_delay;
      srd->run_state = DECEL;
    }
    break;

  case DECEL:
    if (srd->laser == TRUE)
      setLaserByUart();
    speed_cntr_Motor_Pulse(srd);
    srd->step_count++;
    srd->accel_count++;
    srd->act_pos += srd->dir;
    new_step_delay = srd->step_delay +
        (((2 * (long) srd->step_delay) + srd->rest) /
        (-4 * srd->accel_count + 1));
    srd->rest = ((2 * (long) srd->step_delay) + srd->rest) %
        (-4 * srd->accel_count + 1);
    if (srd->accel_count >= 0) {
      srd->run_state = STOP;
    }

    break;
  }
  srd->step_delay = new_step_delay;
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void speed_cntr_Init_Timer() {
  srd_X.act_pos = 0;
  srd_X.laser = FALSE;
  srd_X.run_state = STOP;
  srd_X.stepsPerTurn = 3200;
  srd_X.Motor_Dir_Write = &Motor_X_Dir_Write;
  srd_X.Motor_Step_Write = &Motor_X_Step_Write;
  srd_X.Timer_Start = &Timer_X_Start;
  srd_X.Timer_Stop = &Timer_X_Stop;
  srd_X.Timer_WritePeriod = &Timer_X_WritePeriod;
  Timer_X_Stop();
  isr_X_StartEx(Timer_X_ISR);
  // Point to MY_ISR to carry out the interrupt sub-routine

  srd_Y.act_pos = 0;
  srd_X.laser = FALSE;
  srd_Y.run_state = STOP;
  srd_Y.stepsPerTurn = 3200;
  srd_Y.Motor_Dir_Write = &Motor_Y_Dir_Write;
  srd_Y.Motor_Step_Write = &Motor_Y_Step_Write;
  srd_Y.Timer_Start = &Timer_Y_Start;
  srd_Y.Timer_Stop = &Timer_Y_Stop;
  srd_Y.Timer_WritePeriod = &Timer_Y_WritePeriod;
  Timer_Y_Stop();
  isr_Y_StartEx(Timer_Y_ISR);
  // Point to MY_ISR to carry out the interrupt sub-routine
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
unsigned long my_sqrt(unsigned long x) {
  register unsigned long xr; // result register
  register unsigned long q2; // scan-bit register
  register unsigned char f; // flag (one bit)

  xr = 0; // clear result
  q2 = 0x40000000L; // higest possible result bit
  do {
    if ((xr + q2) <= x) {
      x -= xr + q2;
      f = 1; // set flag
    }
    else {
      f = 0; // clear flag
    }
    xr >>= 1;
    if (f) {
      xr += q2; // test flag
    }
  }
  while (q2 >>= 2); // shift twice
  if (xr < x) {
    return xr + 1; // add for rounding
  }
  else {
    return xr;
  }
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
unsigned int min(unsigned int x, unsigned int y) {
  if (x < y) {
    return x;
  }
  else {
    return y;
  }
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void speed_cntr_Motor_Pulse(speedRampData *srd) {
  srd->Motor_Step_Write(1);
  CyDelayUs(2);
  srd->Motor_Step_Write(0);
}

