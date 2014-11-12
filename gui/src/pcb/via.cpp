#include "via.h"

#include <iostream>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
qreal Via::_minOuter = PcbComp::mil2mm(8);
qreal Via::_maxOuter = PcbComp::mil2mm(20);
qreal Via::_minInner = PcbComp::mil2mm(8);
qreal Via::_maxInner = PcbComp::mil2mm(20);
qreal Via::_relOuter = 0.25;
qreal Via::_relInner = 0.25;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
Via::Via(int layer, QColor color, qreal x, qreal y,
         QString extent, qreal drill, qreal diameter)
  : PcbComp(layer, color),
    _x(x), _y(y), _extent(extent), _drill(drill), _diameter(diameter)
{

}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
QGraphicsItem *Via::createGraphicsItem()
{
  qreal radius;

  if(_diameter > 0 )
    radius = _diameter / 2;
  else
    {
      radius = _drill * _relOuter;
      if(radius > _maxOuter)
        radius = _maxOuter;
      if(radius < _minOuter)
        radius = _minOuter;
      radius += _drill / 2;
    }

  QRectF rectOuter(_x - radius, _y - radius, 2 * radius, 2 * radius);
  QRectF rectInner(_x - (_drill / 2), _y - (_drill / 2), _drill, _drill);

  QPen pen;
  pen.setWidth(0);
  pen.setColor(_color);
  QBrush brush;
  brush.setColor(_color);
  brush.setStyle(Qt::SolidPattern);

  QGraphicsPathItem *pi = new QGraphicsPathItem;
  QPainterPath path;
  path.addEllipse(rectOuter);
  path.addEllipse(rectInner);
  path.setFillRule(Qt::OddEvenFill);

  pi->setPen(pen);
  pi->setBrush(brush);
  pi->setOpacity(1);
  pi->setPath(path);

  return pi;
}
