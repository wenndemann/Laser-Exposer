#include "pcbelement.h"

///////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void PcbElement::add(PcbComp *comp)
{
    _components.push_back(comp);
}

///////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
QVector<PcbComp *> *PcbElement::components()
{
    return &_components;
}
