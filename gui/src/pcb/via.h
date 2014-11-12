#include "pcbcomponent.h"

#ifndef VIA_H
#define VIA_H

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class Via : public PcbComp
{
public:
  Via(int layer, QColor color, qreal x, qreal y,
      QString extent, qreal drill, qreal diameter);
  virtual QGraphicsItem* createGraphicsItem();

  static void setMinViaOuter(qreal value) { _minOuter = PcbComp::mil2mm(value); }
  static void setMaxViaOuter(qreal value) { _maxOuter = PcbComp::mil2mm(value); }
  static void setMinViaInner(qreal value) { _minInner = PcbComp::mil2mm(value); }
  static void setMaxViaInner(qreal value) { _maxInner = PcbComp::mil2mm(value); }
  static void setViaOuter(qreal value) { _relOuter = value; }
  static void setViaInner(qreal value) { _relInner = value; }

private:
  qreal _x, _y;
  QString _extent;
  qreal _drill, _diameter;

  static qreal _minOuter, _relOuter, _maxOuter;
  static qreal _minInner, _relInner, _maxInner;
};

#endif // VIA_H
