#include "pcbcomponent.h"

#ifndef PAD_H
#define PAD_H

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class Pad : public PcbComp
{
public:
  Pad(int layer, QColor color, qreal x, qreal y,
      qreal drill, QString shape, qreal diameter);
  virtual QGraphicsItem* createGraphicsItem();

private:
  qreal _x, _y, _drill;
  QString _shape;
  qreal _diameter;
};

#endif // PAD_H
