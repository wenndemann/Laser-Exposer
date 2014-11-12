/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
 */
#include <project.h>
#include <stdio.h>

#include "global.h"
#include "speed_cntr.h"
#include "serial.h"

void ref_x(void);
void ref_y(void);
void enable(int val);

#define XOFF 19
#define XON 17
  
#define CMD_REF_X 0
#define CMD_REF_Y 1
#define CMD_MOVE_X 2
#define CMD_MOVE_Y 3
#define CMD_ON 4
#define CMD_OFF 5
#define CMD_LASER 6
  
#define EN_ON 0
#define EN_OFF 1

int actPos = 0;
int tarPos = 0;
  
int xoff;

void done(void)
{
  if (UART_SpiUartGetRxBufferSize() == 0)
    UART_UartPutChar('d');
}

int main ()
{

  UART_Start ();
  //UART_customIntrHandler = &bla;
  PWM_Laser_Start();
//  char val[64];

  CyGlobalIntEnable; // Enable global interrupts 
  UART_UartPutString("Start\n");

  xoff = FALSE;
  
  Motor_X_M0_Write (0);
  Motor_X_M1_Write (0);
  Motor_X_M2_Write (1);
  Motor_X_Reset_Write (1);
  Motor_X_Sleep_Write (1);
  Motor_X_Enable_Write(1);

  Motor_Y_M0_Write (1);
  Motor_Y_M1_Write (1);
  Motor_Y_M2_Write (0);
  Motor_Y_Reset_Write (1);
  Motor_Y_Sleep_Write (1);
  Motor_Y_Enable_Write(1);
  
  UART_UartPutString("init\n");
  speed_cntr_Init_Timer ();

  for(;;)
  {
    int cnt = UART_SpiUartGetRxBufferSize();
    if(cnt > 0)
    {
      //sprintf(val, "size = %d, last char = %c\n", cnt, (char)UART_UartGetChar());
      //UART_UartPutString(val);
      //if ((UART_SpiUartGetRxBufferSize() < 2) && (xoff == TRUE))
      //{
      //  UART_UartPutChar(XON);
      //  xoff = FALSE;
      //  isr_UART_StartEx(&bla);
      //}
      uint8 ch = readUINT8();
      switch(ch)
      {
        case CMD_REF_X:
          ref_x();
          done();
          break;
        case CMD_REF_Y:
          ref_y();
          done();
          break;
        case CMD_MOVE_X:
          speed_cntr_Move_Abs(readUINT32(), 50000, 50000, 3000, &srd_X);
          while(srd_X.run_state != STOP)
            CyDelay(1);    
          done();
          break;
        case CMD_MOVE_Y:
          speed_cntr_Move_Abs(readUINT32(), 50000, 50000, 2700, &srd_Y);
          while(srd_Y.run_state != STOP)
            CyDelay(1);    
          done();
          break;
        case CMD_ON:
          enable(EN_ON);
          done();
          break;
        case CMD_OFF:
          enable(EN_OFF);
          done();
          break;
        case CMD_LASER:
          {
          srd_Y.laser = TRUE;
          uint32 byte = readUINT32();
          int32 steps = byte * 8;
          if(readUINT8() == 0)
            steps *= -1;
          while(UART_SpiUartGetRxBufferSize() < byte)
            CyDelayUs(1);
          speed_cntr_Move(steps, 50000, 50000, 100, &srd_Y);
          while(srd_Y.run_state != STOP)
            CyDelay(1);    
          srd_Y.laser = FALSE;
          done();
          }
          break;
        default:
          UART_UartPutString("default");
          done();
      }
    }
  }
  
  
  
  /*char buf[64];
  int i = 0;
  for (;i < 50;i++)
    {
      UART_UartPutString("moving\n");
      speed_cntr_Move (-9600, 50000, 150000, 1500, &srd_X);
      while (status.running == TRUE)
      {
        sprintf(buf, "a=%ld sd=%du rs=%du\n", srd_X.accel_count, srd_X.step_delay, srd_X.run_state);
        UART_UartPutString(buf);
        //int a = srd_Y.step_count;
        //PWM_Laser_WriteCompare(buf[a/8] & (1 <<(a%8)) ? 255 : 0);
    	CyDelay (1);
      }
    
      i++;  
        
      speed_cntr_Move (9600, 50000, 150000, 1500, &srd_X);
      while (status.running == TRUE)
      {
        //int a = srd_Y.step_count;
        //PWM_Laser_WriteCompare(buf[a/8] & (1 <<(a%8)) ? 255 : 0);
    	  CyDelay (1);
      }
    
    }
    for(;;);*/
}

void ref_x(void)
{
  enable(EN_ON);

  speed_cntr_Move(-2000000, 1000, 1000, 3000, &srd_X);
  while(Ref_X_Read() == 0);
  CyDelay(10);
  srd_X.run_state = STOP;
  
  speed_cntr_Move(50000, 1000, 1000, 400, &srd_X);
  while(Ref_X_Read() == 1);
  srd_X.run_state = STOP;
  
  srd_X.act_pos = -15 / 1 * srd_X.spr;
  speed_cntr_Move_Abs(0, 1000, 1000, 3000, &srd_X);
  while(srd_X.run_state != STOP)
    CyDelay(1);
}

void ref_y(void)
{
  enable(EN_ON);

  speed_cntr_Move(-40000, 1000, 1000, 1500, &srd_Y);
  while(Ref_Y_Read() == 0);
  CyDelay(10);
  srd_Y.run_state = STOP;
  
  speed_cntr_Move(1000, 1000, 1000, 150, &srd_Y);
  while(Ref_Y_Read() == 1);
  srd_Y.run_state = STOP;
  
  srd_Y.act_pos = 50 / 75 * srd_Y.spr;
  srd_Y.act_pos *= -1;
  speed_cntr_Move_Abs(0, 1000, 1000, 1500, &srd_Y);
  while(srd_Y.run_state != STOP)
    CyDelay(1);   
}

void enable(int val)
{
  Motor_X_Enable_Write(val);
  Motor_Y_Enable_Write(val);
}

/* [] END OF FILE */
