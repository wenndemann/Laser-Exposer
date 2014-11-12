#include <QColor>
#include <QGraphicsItem>
#include <QPen>

#ifndef PCBCOMPONENT_H
#define PCBCOMPONENT_H

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class PcbComp
{
public:
  PcbComp(int layer, QColor color);
  virtual QGraphicsItem *createGraphicsItem() = 0;
  int layer() { return _iLayer; }
  QColor color() { return _color; }
  void setLayer(int layer) { _iLayer = layer; }
  void setColor(QColor color) { _color = color; }

  static qreal mil2mm(qreal mil);

protected:
  int _iLayer;
  QColor _color;

};

#endif // PCBCOMPONENT_H
