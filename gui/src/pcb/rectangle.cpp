#include "rectangle.h"

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
Rectangle::Rectangle(int layer, QColor color, qreal x1, qreal y1, qreal x2, qreal y2)
    : PcbComp(layer, color),
      _x1(x1), _y1(y1),
      _x2(x2), _y2(y2)
{
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
QGraphicsItem *Rectangle::createGraphicsItem()
{
    QGraphicsRectItem *rect = new QGraphicsRectItem(QRectF(QPointF(_x1,_y1),QPointF(_x2,_y2)));

    QPen pen;
    pen.setColor(_color);
    pen.setWidth(0);
    rect->setPen(pen);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(_color);
    rect->setBrush(brush);

    return rect;
}
