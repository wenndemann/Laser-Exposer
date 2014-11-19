#include "laser.h"

uint8 laser_bit = 0;
uint8 laser_data;
uint8 laser_request = FALSE;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void setLaserByUart() {
  if ((laser_bit % 8) == 0) {
    if (UART_SpiUartGetRxBufferSize() > 0)
      laser_data = UART_UartGetByte();
  }

  PWM_Laser_WriteCompare(laser_data & (1 << (laser_bit % 8)) ? 255 : 0);
  laser_bit++;
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void resetLaser(void) {
  PWM_Laser_WriteCompare(0);
  laser_bit = 0;
  laser_data = 0;
  laser_request = 0;
}

