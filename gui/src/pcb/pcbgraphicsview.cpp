#include "pcbgraphicsview.h"

PcbGraphicsView::PcbGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void PcbGraphicsView::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0)
        scale(1.25,1.25);
    else
        scale(0.8, 0.8);
}
