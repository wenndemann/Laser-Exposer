#ifndef EXPOSERHARDWARE_H
#define EXPOSERHARDWARE_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>

#include <QDebug>

struct MotorParam
{
  quint32 targetPosition;
  quint32 acceleration;
  quint32 deceleration;
  quint32 speed;
};

class ExposerHardware : public QObject
{
  Q_OBJECT

public:
  enum Command { refX, refY, moveX, moveY, on, off, laser};
  enum Axis { X, Y };

  ExposerHardware();

  void enable(bool val);
  void referendeAxis(Axis axis);
  void sendMotorParam(Axis axis);
  void moveTo(Axis axis, quint32 pos);
  bool isMoving() { return _isMoving; }
  void beam(bool fwd, const QByteArray &ba);

private:
  void sendData(const QByteArray &ba);

  QSerialPort *serial;
  MotorParam _motorX, _motorY;
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
