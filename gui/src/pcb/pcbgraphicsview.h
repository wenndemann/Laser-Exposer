#ifndef PCBGRAPHICSVIEW_H
#define PCBGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

class PcbGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PcbGraphicsView(QWidget *parent = 0);

signals:

private:

protected:
    virtual void wheelEvent(QWheelEvent* event);

public slots:

};

#endif // PCBGRAPHICSVIEW_H
