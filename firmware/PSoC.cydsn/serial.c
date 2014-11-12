#include "serial.h"

uint8 readUINT8()
{
  while(UART_SpiUartGetRxBufferSize() < 1)
    CyDelayUs(1);
  return UART_UartGetByte();
}

uint32 readUINT32()
{
  //UART_UartPutChar('r');
  //while(UART_SpiUartGetRxBufferSize() < 4);
  
  uint32 temp = 0;
  uint8 i;
  while(UART_SpiUartGetRxBufferSize() < 4)
    CyDelayUs(1);
  for(i = 0; i < 4; i++)
    temp += (UART_UartGetByte() << (i*8));
    
  return temp;
}