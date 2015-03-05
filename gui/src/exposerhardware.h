#ifndef EXPOSERHARDWARE_H
#define EXPOSERHARDWARE_H

#include <QObject>
#include <QSerialPort>
#include "QByteArray.h"

#include <QDebug>

struct ExposerParameter
{
  quint8 laserPower;
  quint32 laserSpeed;

  qint32 xMinPos, xMaxPos, xRefPos;
  quint32 xSpeed, xAcc, xDec, xStepsPerTurn, xDistPerTurn;

  qint32 yMinPos, yMaxPos, yRefPos;
  quint32 ySpeed, yAcc, yDec, yStepsPerTurn, yDistPerTurn;
};

class ExposerHardware : public QObject
{
  Q_OBJECT

public:
  enum Command { refX, refY,
                 moveX, moveY,
                 on, off,
                 laser,
                 writeParam, readParam};
  enum Axis { X, Y };

  ExposerHardware();

  void enable(bool val);
  void referenceAxis(Axis axis);
  void sendParameter();
  void readParameter();
  void moveTo(Axis axis, quint32 pos);
  bool isMoving() { return _isMoving; }
  void beam(bool fwd, const QByteArray &ba);

  ExposerParameter param() const;
  void setParam(const ExposerParameter &param);
  void requestParam();

private:
  void sendData(const QByteArray &ba);

  QSerialPort *serial;
  ExposerParameter _param;
  QByteArray _cmdBuffer, _dataBuffer;
  bool _isMoving;

public slots:

private slots:
  void gotData();
  void setBaud(QString baud) { serial->setBaudRate(baud.toInt()); qDebug() << "baud = " << baud; }
  void setInterface(QString interface) { serial->setPortName(interface); qDebug() << "interface = " << interface;}

signals:
  void portStateChanged(bool state);

};

#endif // EXPOSERHARDWARE_H
