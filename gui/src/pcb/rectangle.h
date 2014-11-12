#include "pcbcomponent.h"

#ifndef RECTANGLE_H
#define RECTANGLE_H

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class Rectangle : public PcbComp
{
public:
  Rectangle(int layer, QColor color, qreal x1, qreal y1, qreal x2, qreal y2);
  virtual QGraphicsItem* createGraphicsItem();

private:
  qreal _x1, _y1, _x2, _y2, _width;
};


#endif // RECTANGLE_H
