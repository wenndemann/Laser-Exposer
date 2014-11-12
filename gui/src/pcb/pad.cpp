#include "pad.h"

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
Pad::Pad(int layer, QColor color, qreal x, qreal y, qreal drill, QString shape, qreal diameter)
  : PcbComp(layer, color),
    _x(x), _y(y), _drill(drill), _diameter(diameter)
{
  _shape = shape;
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
QGraphicsItem *Pad::createGraphicsItem()
{


  return 0;
}
