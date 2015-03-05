#include "exposerhardware.h"
#include "sendbuffer.h"

#include <QDebug>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
ExposerHardware::ExposerHardware()
{
  serial = new QSerialPort;
//  serial->setPortName("com3");
//  serial->setBaudRate(QSerialPort::Baud115200);
//  serial->open(QIODevice::ReadWrite);
  connect(serial, SIGNAL(readyRead()), this, SLOT(gotData()));

}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ExposerHardware::enable(bool val)
{
  QByteArray ba;

  _isMoving = true;
  ba.append(val ? Command::on : Command::off);
  sendData(ba);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ExposerHardware::referenceAxis(ExposerHardware::Axis axis)
{
  QByteArray ba;

  _isMoving = true;
  ba.append(axis + Command::refX);
  sendData(ba);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ExposerHardware::sendParameter()
{
  SendBuffer sb;

  sb << static_cast<quint8>(Command::writeParam);

  sb << _param.laserPower;
  sb << _param.laserSpeed;

  sb << _param.xMinPos << _param.xMaxPos << _param.xRefPos;
  sb << _param.xSpeed << _param.xAcc << _param.xDec;
  sb << _param.xStepsPerTurn << _param.xDistPerTurn;

  sb << _param.yMinPos << _param.yMaxPos << _param.yRefPos;
  sb << _param.ySpeed << _param.yAcc << _param.yDec;
  sb << _param.yStepsPerTurn << _param.yDistPerTurn;
  sendData(sb);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ExposerHardware::readParameter()
{
  SendBuffer sb;
  sb << static_cast<quint8>(Command::readParam);
  sendData(sb);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ExposerHardware::moveTo(ExposerHardware::Axis axis, quint32 pos)
{
  SendBuffer ba;

  _isMoving = true;
  ba.append(axis + Command::moveX);
  ba << pos;
  sendData(ba);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ExposerHardware::beam(bool fwd, const QByteArray &ba)
{
  QByteArray cmd;

  _isMoving = true;
  cmd.append(Command::laser);
  for(unsigned int i = 0; i < 4; i++)
    cmd.append((quint8)(static_cast<quint32>(ba.size()) >> (i*8)));
  cmd.append(fwd ? 1 : 0);
  cmd.append(ba);
  sendData(cmd);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ExposerHardware::sendData(const QByteArray &ba)
{
  if (serial->open(QIODevice::ReadWrite))
    {
      qDebug() << "send data (" << ba.size() << ")";
      emit(portStateChanged(true));
      serial->write(ba);
      if(!serial->waitForBytesWritten(2000))
        qDebug() << "failed to send data";
    }
  else
    qDebug() << "failed to open device";
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
ExposerParameter ExposerHardware::param() const
{
  return _param;
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ExposerHardware::setParam(const ExposerParameter &param)
{
   qDebug("parameter changed");
   _param = param;
   sendParameter();

}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ExposerHardware::requestParam()
{
  readParameter();
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ExposerHardware::gotData()
{
  while(serial->bytesAvailable())
    {
      qDebug() << "read (" << serial->bytesAvailable() << ")" ;
      char ch;
      serial->read(&ch, 1);
      if(ch == 'l')
        {
          if(_dataBuffer.size() > 0)
            {
              //qDebug() << ch;
              serial->write(_dataBuffer.data(), 1);
              _dataBuffer.remove(0, 1);
            }
          else
            _dataBuffer.clear();
        }
      else if(ch == 'd')
        {
          _isMoving = false;
          qDebug() << "done";
          serial->close();
          emit(portStateChanged(false));
        }
      else if(ch == 'p')
        {
          SendBuffer sb;
          sb.append(serial->readAll());

//          for(int i = 0; i < 10; i++)
//             qDebug() << i << ": " << (quint8)sb.data()[i];

          sb >> _param.laserPower;
          sb >> _param.laserSpeed;

          sb >> _param.xMinPos >> _param.xMaxPos >> _param.xRefPos;

          qDebug() << "laserPower = " << _param.laserPower;
          qDebug() << "laserSpeed = " << _param.laserSpeed;
          qDebug() << "xMinPos = " << _param.xMinPos;
          qDebug() << "xMaxPos = " << _param.xMaxPos;
          qDebug() << "xRefPos = " << _param.xRefPos;
          serial->close();
          emit(portStateChanged(false));
        }

      else
        {
          qDebug() << ch << serial->readAll();
          serial->close();
          emit(portStateChanged(false));
        }
    }
}
