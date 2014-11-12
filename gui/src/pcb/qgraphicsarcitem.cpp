#include "qgraphicsarcitem.h"

#include <QtMath>
#include <QPainter>
#include <QVector2D>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
QGraphicsArcItem::QGraphicsArcItem(qreal x1, qreal y1, qreal x2, qreal y2, qreal curve, QGraphicsItem *parent)
    :  QGraphicsLineItem(x1, y1, x2, y2, parent), _curve(curve)
{
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void QGraphicsArcItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setPen(pen());

    if(_curve != 0)
    {
        QVector2D x(line().dx(), line().dy());
        x = x / 2;

        QVector2D y(line().dy(), -line().dx());
        y.normalize();
        y = -y * (x.length() / tan((_curve/180*M_PI)/2));

        QPointF m((QVector2D(line().p1()) + x + y).toPointF());

        qreal r = sqrt(pow(x.length(),2) + pow(y.length(),2));
        QRectF rect(m.x()-r, m.y()-r , r*2, r*2);
        QVector2D R(line().p1().x() - m.x(),line().p1().y() - m.y() );

        qreal startAngle = atan2(R.y(), R.x()) * 180 / M_PI * -16;
        qreal spanAngle = _curve * -16;

        painter->drawArc(rect, startAngle, spanAngle);
    }
    else
        painter->drawLine(line());

    //          if (option->state & QStyle::State_Selected)
    //              qt_graphicsItem_highlightSelected(this, painter, option);
}
