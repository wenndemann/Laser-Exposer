#include "pcbcomponent.h"

#ifndef PCBELEMENT_H
#define PCBELEMENT_H

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
class PcbElement
{
public:
  void add(PcbComp *comp);
  QVector<PcbComp *> *components();

private:
  QVector<PcbComp *> _components;
};

#endif // PCBELEMENT_H
