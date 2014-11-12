#include "smd.h"


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
Smd::Smd(int layer, QColor color, qreal x, qreal y, qreal dx, qreal dy, qreal rot)
    : PcbComp(layer, color),
      _x(x), _y(y), _dx(dx), _dy(dy), _rot(rot)
{
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
QGraphicsItem *Smd::createGraphicsItem()
{
    QGraphicsRectItem *rect = new QGraphicsRectItem(_x-(_dx/2), _y-(_dy/2), _dx, _dy);

    QPen pen;
    pen.setColor(_color);
    pen.setWidth(0);
    rect->setPen(pen);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(_color);
    rect->setBrush(brush);

    rect->setTransformOriginPoint(rect->boundingRect().center());
    rect->setRotation(_rot);
    return rect;
}
