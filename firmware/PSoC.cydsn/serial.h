#ifndef SERIAL_H
#define SERIAL_H

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
#include "project.h"

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
uint8 readUINT8();
uint32 readINT32();
uint32 readUINT32();

void writeUINT8(uint8 val);
void writeINT32(int32 val);
void writeUINT32(uint32 val);

#endif
