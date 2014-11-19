#include "serial.h"

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
uint8 readUINT8() {
  while (UART_SpiUartGetRxBufferSize() < 1)
    CyDelayUs(1);
  return UART_UartGetByte();
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
uint32 readINT32() {
  int32 temp = 0;
  uint8 i;
  while (UART_SpiUartGetRxBufferSize() < 4)
    CyDelayUs(1);
  for (i = 0; i < 4; i++)
    temp += (UART_UartGetByte() << (i*8));

  return temp;
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
uint32 readUINT32() {
  uint32 temp = 0;
  uint8 i;
  while (UART_SpiUartGetRxBufferSize() < 4)
    CyDelayUs(1);
  for (i = 0; i < 4; i++)
    temp += (UART_UartGetByte() << (i*8));

  return temp;
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void writeUINT8(uint8 val) {
  UART_UartPutByte(val);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void writeINT32(int32 val) {
  unsigned int i;
  for (i = 0; i < 4; i++)
    UART_UartPutByte((uint8)(val >> (i*8)));
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void writeUINT32(uint32 val) {
  unsigned int i;
  for (i = 0; i < 4; i++)
    UART_UartPutByte((uint8)(val >> (i*8)));
}
