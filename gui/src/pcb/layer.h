#include <QGraphicsItem>
#include <QString>

#ifndef LAYER_H
#define LAYER_H

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class Layer
{
public:
  Layer(int num, QString name, int _color, bool visible);

  void add(QGraphicsItem* e);
  QColor color() const;
  QVector<QGraphicsItem *> *elements() { return &_vElements; }
  void setColor(const QColor &value);
  bool visible() { return _bVisible; }

private:
  QColor eagle2Qt(int eagleColor);

  int _iNum;
  QString _sName;
  QColor _color;
  bool _bVisible;
  QVector<QGraphicsItem *> _vElements;
};

#endif // LAYER_H
