#include "exposerhardware.h"

#include <QDebug>

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
ExposerHardware::ExposerHardware()
{
  serial = new QSerialPort;
  serial->setPortName("com3");
  serial->setBaudRate(QSerialPort::Baud115200);
  serial->open(QIODevice::ReadWrite);
  connect(serial, SIGNAL(readyRead()), this, SLOT(gotData()));
  qDebug() << "port is open";
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ExposerHardware::enable(bool val)
{
  _isMoving = true;
  QByteArray ba;
  ba.append(val ? Command::on : Command::off);
  serial->write(ba, ba.size());
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ExposerHardware::referendeAxis(ExposerHardware::Axis axis)
{
  _isMoving = true;
  QByteArray ba;
  ba.append(axis + Command::refX);
  serial->write(ba, ba.size());
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ExposerHardware::sendMotorParam(ExposerHardware::Axis axis)
{

}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ExposerHardware::moveTo(ExposerHardware::Axis axis, quint32 pos)
{
  _isMoving = true;
  QByteArray ba;

  ba.append(axis + Command::moveX);

  for(unsigned int i = 0; i < 4; i++)
    ba.append((quint8)(pos >> (i*8)));

  serial->write(ba, ba.size());
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ExposerHardware::beam(bool fwd, const QByteArray &ba)
{
  _isMoving = true;
  QByteArray cmd;

  cmd.append(Command::laser);
  for(unsigned int i = 0; i < 4; i++)
    cmd.append((quint8)(static_cast<quint32>(ba.size()) >> (i*8)));
  cmd.append(fwd ? 1 : 0);

  serial->write(cmd, cmd.size());
  serial->write(ba, ba.size());
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void ExposerHardware::gotData()
{
  while(serial->bytesAvailable())
    {
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
        }

//      else
//        {
//          qDebug() << ch << serial->readAll();
//        }
    }
}
