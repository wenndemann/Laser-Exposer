#include "layer.h"

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
Layer::Layer(int num, QString name, int color, bool visible)
    : _iNum(num), _sName(name), _color(eagle2Qt(color)), _bVisible(visible)
{
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void Layer::add(QGraphicsItem *e)
{
    _vElements.push_back(e);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
QColor Layer::color() const
{
    return _color;
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void Layer::setColor(const QColor &value)
{
    _color = value;
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
QColor Layer::eagle2Qt(int eagleColor)
{
    switch(eagleColor)
    {
    case  0: return Qt::black; break;
    case  1: return Qt::darkBlue; break;
    case  2: return Qt::darkGreen; break;
    case  3: return Qt::darkCyan; break;
    case  4: return Qt::darkRed; break;
    case  5: return Qt::darkMagenta; break;
    case  6: return Qt::darkYellow; break;
    case  7: return Qt::darkGray; break;
    case  8: return Qt::black; break;
    case  9: return Qt::blue; break;
    case 10: return Qt::green; break;
    case 11: return Qt::cyan; break;
    case 12: return Qt::red; break;
    case 13: return Qt::magenta; break;
    case 14: return Qt::yellow; break;
    case 15: return Qt::gray; break;
    }
    return Qt::black;
}
