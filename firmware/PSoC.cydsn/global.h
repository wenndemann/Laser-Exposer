#ifndef GLOBAL_H
#define GLOBAL_H

#define TRUE 1
#define FALSE 0

#include "speed_cntr.h"
#include "laser.h"

struct GLOBAL_FLAGS {
  unsigned char running : 1;
  unsigned char cmd : 1;
  unsigned char dummy : 6;
};

speedRampData srd_X, srd_Y;
laserData ld;

#endif
