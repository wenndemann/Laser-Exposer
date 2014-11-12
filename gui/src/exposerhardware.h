#ifndef EXPOSERHARDWARE_H
#define EXPOSERHARDWARE_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>



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
  QSerialPort *serial;
  MotorParam _motorX, _motorY;
  QByteArray _cmdBuffer, _dataBuffer;
  bool _isMoving;

private slots:
  void gotData();
};

#endif // EXPOSERHARDWARE_H
