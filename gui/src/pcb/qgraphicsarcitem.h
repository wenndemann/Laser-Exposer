#ifndef QGRAPHICSARCITEM_H
#define QGRAPHICSARCITEM_H

#include <QGraphicsLineItem>

class QGraphicsArcItem : public QGraphicsLineItem
{
public:
    QGraphicsArcItem (qreal x1, qreal y1, qreal x2, qreal y2, qreal curve, QGraphicsItem * parent = 0 );
  qreal curve() { return _curve; }
  void setCurve(qreal curve) { _curve = curve; }

protected:
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    qreal _curve;
};

#endif // QGRAPHICSARCITEM_H
