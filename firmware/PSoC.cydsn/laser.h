#ifndef LASER_H
#define LASER_H

#include "project.h"

struct laserData {
  uint8 power;
  uint32 speed;
};

void setLaserByUart();
void resetLaser(void);

#endif
