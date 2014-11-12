#include "pcbcomponent.h"

#ifndef WIRE_H
#define WIRE_H

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class Wire : public PcbComp
{
public:
  Wire(int layer, QColor color, qreal x1, qreal y1, qreal x2, qreal y2, qreal width, qreal curve = 0);
  virtual QGraphicsItem* createGraphicsItem();

private:
  qreal _x1, _y1, _x2, _y2, _width, _curve;
};

#endif // WIRE_H
