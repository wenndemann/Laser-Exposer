#ifndef LASER_H
#define LASER_H

#include "project.h"

typedef struct {
  uint8 power;
  uint32 speed;
} laserData;

void setLaserByUart();
void resetLaser(void);

#endif
