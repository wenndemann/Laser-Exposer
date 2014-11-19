#include <project.h>
#include <stdio.h>

#include "global.h"
#include "speed_cntr.h"
#include "serial.h"

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
#define XOFF 19
#define XON 17

#define CMD_REF_X 0
#define CMD_REF_Y 1
#define CMD_MOVE_X 2
#define CMD_MOVE_Y 3
#define CMD_ON 4
#define CMD_OFF 5
#define CMD_LASER 6
#define CMD_READ_SETTINGS 7
#define CMD_WRITE_SETTINGS 8

#define EN_ON 0
#define EN_OFF 1

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
int actPos = 0;
int tarPos = 0;
int xoff;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void done(void);
void ref_x(void);
void ref_y(void);
void enable(int val);
void getData();
void sendData();

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
int main() {
  UART_Start();
  PWM_Laser_Start();
  CyGlobalIntEnable; // Enable global interrupts

  xoff = FALSE;

  Motor_X_M0_Write(0);
  Motor_X_M1_Write(0);
  Motor_X_M2_Write(1);
  Motor_X_Reset_Write(1);
  Motor_X_Sleep_Write(1);
  Motor_X_Enable_Write(1);

  Motor_Y_M0_Write(1);
  Motor_Y_M1_Write(1);
  Motor_Y_M2_Write(0);
  Motor_Y_Reset_Write(1);
  Motor_Y_Sleep_Write(1);
  Motor_Y_Enable_Write(1);

  UART_UartPutString("init\n");
  speed_cntr_Init_Timer();

  for (; ;) {
    int cnt = UART_SpiUartGetRxBufferSize();
    if (cnt > 0) {
      uint8 ch = readUINT8();
      switch (ch) {
      case CMD_REF_X:
        ref_x();
        done();
        break;

      case CMD_REF_Y:
        ref_y();
        done();
        break;

      case CMD_MOVE_X:
        speed_cntr_Move_Abs(readUINT32(), srd_X.acc, srd_X.dec, srd_X.speed,
            &srd_X);
        while (srd_X.run_state != STOP)
          CyDelay(1);
        done();
        break;

      case CMD_MOVE_Y:
        speed_cntr_Move_Abs(readUINT32(), srd_Y.acc, srd_Y.dec, srd_Y.speed,
            &srd_Y);
        while (srd_Y.run_state != STOP)
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

      case CMD_LASER: {
          srd_Y.laser = TRUE;
          uint32 byte = readUINT32();
          int32 steps = byte * 8;
          if (readUINT8() == 0)
            steps *= -1;
          while (UART_SpiUartGetRxBufferSize() < byte)
            CyDelayUs(1);
          srd_Y.laser = FALSE;
          done();
        } break;

      case CMD_READ_SETTINGS: // read settings from PC
        getData();
        break;

      case CMD_WRITE_SETTINGS: // write settings to PC
        sendData();
        break;

      default:
        UART_UartPutString("default");
        done();
      }
    }
  }
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void done(void) {
  if (UART_SpiUartGetRxBufferSize() == 0)
    UART_UartPutChar('d');
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ref_x(void) {
  enable(EN_ON);

  speed_cntr_Move(-2000000, srd_X.acc, srd_X.dec, srd_X.speed, &srd_X);
  while (Ref_X_Read() == 0);
  CyDelay(10);
  srd_X.run_state = STOP;

  speed_cntr_Move(50000, srd_X.acc, srd_X.dec, srd_X.speed / 10, &srd_X);
  while (Ref_X_Read() == 1);
  srd_X.run_state = STOP;

  srd_X.act_pos = srd_X.refPos / srd_X.distPerTurn * srd_X.stepsPerTurn;
  speed_cntr_Move_Abs(0, srd_X.acc, srd_X.dec, srd_X.speed, &srd_X);
  while (srd_X.run_state != STOP)
    CyDelay(1);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ref_y(void) {
  enable(EN_ON);

  speed_cntr_Move(-40000, srd_Y.acc, srd_Y.dec, srd_Y.speed, &srd_Y);
  while (Ref_Y_Read() == 0);
  CyDelay(10);
  srd_Y.run_state = STOP;

  speed_cntr_Move(1000, srd_Y.acc, srd_Y.dec, srd_Y.speed / 10, &srd_Y);
  while (Ref_Y_Read() == 1);
  srd_Y.run_state = STOP;

  srd_Y.act_pos = srd_Y.refPos / srd_Y.distPerTurn * srd_Y.stepsPerTurn;
  speed_cntr_Move_Abs(0, srd_Y.acc, srd_Y.dec, srd_X.speed, &srd_Y);
  while (srd_Y.run_state != STOP)
    CyDelay(1);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void enable(int val) {
  Motor_X_Enable_Write(val);
  Motor_Y_Enable_Write(val);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void getData() {
  ld.power = readUINT8();
  ls.speed = readUINT32();

  srd_X.minPos = readINT32();
  srd_X.maxPos = readINT32();
  srd_X.refPos = readINT32();
  srd_X.speed = readUINT32();
  srd_X.acc = readUINT32();
  srd_X.dec = readUINT32();
  srd_X.stepsPerTurn = readUINT32();
  srd_X.distPerTurn = readUINT32();

  srd_Y.minPos = readINT32();
  srd_Y.maxPos = readINT32();
  srd_Y.refPos = readINT32();
  srd_Y.speed = readUINT32();
  srd_Y.acc = readUINT32();
  srd_Y.dec = readUINT32();
  srd_Y.stepsPerTurn = readUINT32();
  srd_Y.distPerTurn = readUINT32();
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void sendData() {
  writeUINT8(ls.power);
  writeUINT32(ls.speed);

  writeINT32(srd_X.minPos);
  writeINT32(srd_X.maxPos);
  writeINT32(srd_X.refPos);
  writeUINT32(srd_X.speed);
  writeUINT32(srd_X.acc);
  writeUINT32(srd_X.dec);
  writeUINT32(srd_X.stepsPerTurn);
  writeUINT32(srd_X.distPerTurn);

  writeINT32(srd_Y.minPos);
  writeINT32(srd_Y.maxPos);
  writeINT32(srd_Y.refPos);
  writeUINT32(srd_Y.speed);
  writeUINT32(srd_Y.acc);
  writeUINT32(srd_Y.dec);
  writeUINT32(srd_Y.stepsPerTurn);
  writeUINT32(srd_Y.distPerTurn);
}

/* [] END OF FILE */
