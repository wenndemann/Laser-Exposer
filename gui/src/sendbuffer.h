#ifndef SENDBUFFER_H
#define SENDBUFFER_H

#include <QByteArray>

class SendBuffer : public QByteArray
{
public:
  SendBuffer() { }

  template <typename T>
  SendBuffer &operator<< (const T& val)
  {
    for(unsigned int i = 0; i < sizeof(val); i++)
      this->append(static_cast<quint8>(val >> (i*8)));
    return *this;
  }

  template <typename T>
  SendBuffer &operator>> (T &val)
  {
    val = 0;
    for(unsigned int i = 0; i < sizeof(val); i++)
      val = val | (this->data()[i] << i*8);
    this->remove(0,sizeof(val));
    return *this;
  }
};

#endif // SENDBUFFER_H
