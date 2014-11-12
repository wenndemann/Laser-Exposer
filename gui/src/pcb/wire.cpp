#include "qgraphicsarcitem.h"
#include "wire.h"

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
Wire::Wire(int layer, QColor color, qreal x1, qreal y1, qreal x2, qreal y2, qreal width, qreal curve)
    : PcbComp(layer, color),
      _x1(x1), _y1(y1),
      _x2(x2), _y2(y2),
      _width(width), _curve(curve)
{
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
QGraphicsItem *Wire::createGraphicsItem()
{
    QPen pen;
    pen.setColor(_color);
    pen.setWidthF(_width);
    pen.setCapStyle(Qt::RoundCap);

    QGraphicsArcItem *ai = new QGraphicsArcItem(_x1,_y1,_x2,_y2,_curve);
    ai->setPen(pen);

    return ai;
}
