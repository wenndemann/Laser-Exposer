#include "pcbcomponent.h"

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
PcbComp::PcbComp(int layer, QColor color)
    :_iLayer(layer), _color(color)
{
}

qreal PcbComp::mil2mm(qreal mil)
{
  return mil * 0.0254;
}
