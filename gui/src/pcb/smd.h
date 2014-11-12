#include "pcbcomponent.h"

#ifndef SMD_H
#define SMD_H

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class Smd : public PcbComp
{
public:
  Smd(int layer, QColor color, qreal x, qreal y, qreal dx, qreal dy, qreal rot = 0);
  virtual QGraphicsItem* createGraphicsItem();

private:
  qreal _x, _y, _dx, _dy, _rot;
};




#endif // SMD_H
