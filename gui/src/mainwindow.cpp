#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QGraphicsItemGroup>
#include <QSerialPortInfo>
#include <QThread>
#include <QDebug>

#include <iostream>
using namespace std;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  scaleFactor(1.25)
{
  ui->setupUi(this);
  _scene = new PcbGraphicsScene(this);
  //  _scene->setSceneRect(0,0,13000,13000);
  ui->gvPcb->setScene(_scene);

  // serial interface
  foreach (const QSerialPortInfo &ports, QSerialPortInfo::availablePorts())
     ui->cbInterface->addItem(ports.portName());
  if(ui->cbInterface->count() > 0)
    ui->cbInterface->setCurrentIndex(ui->cbInterface->count()-1);
  connect(ui->cbInterface, SIGNAL(currentTextChanged(QString)), &_laserExposer, SLOT(setInterface(QString)));

  ui->cbBaud->setCurrentText("115200");
  connect(ui->cbBaud, SIGNAL(currentTextChanged(QString)), &_laserExposer, SLOT(setBaud(QString)));

  connect(&_laserExposer, SIGNAL(portStateChanged(bool)), this, SLOT(on_portStateChange(bool)));
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
MainWindow::~MainWindow()
{
  delete _scene;
  delete ui;
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::open()
{
  QString fileName = QFileDialog::getOpenFileName(this,
                                                  tr("Open File"), "/home/reinhard/Dropbox/dev/LaserExposer",
                                                  tr("Eagle Boardfiles (*.brd);;Bitmap (*.bmp)"));
  QString suffix = QFileInfo(fileName).suffix();
  if (suffix == "brd") openBrdFile(fileName);
  if (suffix == "bmp") openBmpFile(fileName);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::openBmpFile(QString fileName)
{
  _scene->clear();
  ui->listWidget->clear();

  QImage image(fileName);
  cout << "image width=" << image.width() << endl;
  QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
  item->setScale(1);
  _scene->addItem(item);

  cout << "item width=" << item->boundingRect().width() << endl;

  ui->gvPcb->resetMatrix();
  ui->gvPcb->scale(1,1);
  //ui->gvPcb->fitInView(_scene->itemsBoundingRect(),Qt::KeepAspectRatio);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::openBrdFile(QString fileName)
{
  QDomDocument doc("mydocument");
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly))
    return;
  if (!doc.setContent(&file)) {
      file.close();
      return;
    }
  file.close();

  _scene->clear();
  ui->listWidget->clear();

  parseXML(doc);

  ui->gvPcb->resetMatrix();
  ui->gvPcb->scale(39.37007874,-39.37007874);
  ui->gvPcb->fitInView(_scene->itemsBoundingRect(),Qt::KeepAspectRatio);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::parseXML(QDomDocument &d)
{
  QDomElement elemEagle = d.documentElement();
  QDomNode elemDrawings = elemEagle.firstChild();
  elemLayers = elemDrawings.namedItem("layers");
  QDomNode elemBoard = elemDrawings.namedItem("board");
  QDomNode elemPlain = elemBoard.namedItem("plain");
  QDomNode elemDesignrules= elemBoard.namedItem("designrules");
  QDomNode elemLibraries = elemBoard.namedItem("libraries");
  QDomNode elemElements = elemBoard.namedItem("elements");
  QDomNode elemSignals = elemBoard.namedItem("signals");

  parseLayers(elemLayers);
  parseDesignrules(elemDesignrules);

  _mElements["plain"] = parsePcbComponents(elemPlain);
  _scene->addItem(createGraphics("plain"));

  _mElements["signals"] = parsePcbComponents(elemSignals);
  _scene->addItem(createGraphics("signals"));

  parsePackages(elemLibraries);
  drawElements(elemElements);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
PcbElement *MainWindow::parsePcbComponents(QDomNode &n)
{
  PcbElement *e = new PcbElement;
  parsePcbComponents(n, e);
  return e;
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::parsePcbComponents(QDomNode &n, PcbElement *e)
{
  QDomNode dn = n.firstChild();
  QDomElement elem;

  while(!(dn.isNull()))
    {
      if(dn.isElement())
        {
          elem = dn.toElement();
          if(!elem.isNull())
            {
              if(elem.tagName() == "wire")
                {
                  int layer = elem.attribute("layer").toInt();
                  QColor color = _mLayer[layer]->color();
                  qreal curve = 0;
                  if(elem.hasAttribute("curve"))
                    curve = elem.attribute("curve").toDouble();
                  e->add(new Wire(layer, color,
                                  elem.attribute("x1").toDouble(),
                                  elem.attribute("y1").toDouble(),
                                  elem.attribute("x2").toDouble(),
                                  elem.attribute("y2").toDouble(),
                                  elem.attribute("width").toDouble(),
                                  curve));
                }
              if(elem.tagName() == "rectangle")
                {
                  int layer = elem.attribute("layer").toInt();
                  QColor color = _mLayer[layer]->color();
                  e->add(new Rectangle(layer, color,
                                       elem.attribute("x1").toDouble(),
                                       elem.attribute("y1").toDouble(),
                                       elem.attribute("x2").toDouble(),
                                       elem.attribute("y2").toDouble()));
                }
              if(elem.tagName() == "smd")
                {
                  int layer = elem.attribute("layer").toInt();
                  QColor color = _mLayer[layer]->color();
                  double rot = 0;
                  if (elem.hasAttribute("rot"))
                    rot = elem.attribute("rot").mid(1).toDouble();
                  e->add(new Smd(layer, color,
                                 elem.attribute("x").toDouble(),
                                 elem.attribute("y").toDouble(),
                                 elem.attribute("dx").toDouble(),
                                 elem.attribute("dy").toDouble(),
                                 rot));
                }
              if(elem.tagName() == "via")
                {
                  int layer = 18; //elem.attribute("layer").toInt();
                  QColor color = _mLayer[layer]->color();
                  double diameter = 0;
                  if (elem.hasAttribute("diameter"))
                    diameter = elem.attribute("diameter").toDouble();
                  e->add(new Via(layer, color,
                                 elem.attribute("x").toDouble(),
                                 elem.attribute("y").toDouble(),
                                 elem.attribute("extend"),
                                 elem.attribute("drill").toDouble(),
                                 diameter));
                }

            }
        }
      parsePcbComponents(dn, e);
      dn = dn.nextSibling();
    }
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::parseLayers(QDomNode &n)
{
  QDomNode dn = n.toElement().firstChild();
  QDomElement elem;
  while(!dn.isNull())
    {
      elem = dn.toElement(); // try to convert the node to an element.
      int num = elem.attribute("number").toInt();
      QString name = elem.attribute("name");
      bool visible = elem.attribute("visible") == "yes" ? true : false;
      _mLayer[num] = new Layer(num,
                               name,
                               elem.attribute("color").toInt(),
                               visible);

      // add layer to QListWidget
      QListWidgetItem* lwi = new QListWidgetItem(name);
      lwi->setFlags(lwi->flags() | Qt::ItemIsUserCheckable);
      lwi->setCheckState(visible ? Qt::Checked : Qt::Unchecked);
      ui->listWidget->addItem(lwi);
      _mLayerItemList[lwi] = _mLayer[num];

      dn = dn.nextSibling();
    }
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::parseDesignrules(QDomNode &n)
{
  QDomNode dn = n.firstChild();
  QDomElement elem;

  while(!(dn.isNull()))
    {
      if(dn.isElement())
        {
          elem = dn.toElement();
          if(!elem.isNull())
            {
              if(elem.tagName() == "param")
                {
                  QString name = elem.attribute("name");
                  qreal value = elem.attribute("value").toDouble();

                  std::string strValue = elem.attribute("value").toStdString();
                  size_t firstNoneNumeric = strValue.find_first_not_of("1234567890.");
                  if (firstNoneNumeric != std::string::npos)
                    value = atof(strValue.substr(0, firstNoneNumeric).c_str());



                  // Via
                  if (name == "rlMinViaOuter") Via::setMinViaOuter(value);
                  if (name == "rlMaxViaOuter") Via::setMaxViaOuter(value);
                  if (name == "rlMinViaInner") Via::setMinViaInner(value);
                  if (name == "rlMaxViaInner") Via::setMaxViaInner(value);
                  if (name == "rvViaInner") Via::setViaInner(value);
                  if (name == "rvViaInner") Via::setViaOuter(value);

                }
            }
        }
      dn = dn.nextSibling();
    }
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::parsePackages(QDomNode &n)
{
  QDomNode dn = n.toElement().firstChild();
  QDomElement elem;
  while(!dn.isNull())
    {
      elem = dn.toElement();
      if(!elem.isNull())
        {
          QDomNode dnPacks = elem.firstChild().firstChild();
          QDomElement elemPacks;
          while(!dnPacks.isNull())
            {
              elemPacks = dnPacks.toElement();
              if(!elemPacks.isNull())
                {
                  QString name = elemPacks.attribute("name");
                  _mElements[name] = parsePcbComponents(elemPacks);
                }
              dnPacks = dnPacks.nextSibling();
            }
        }
      dn = dn.nextSibling();
    }
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
QGraphicsItemGroup *MainWindow::createGraphics(QString package, qreal x, qreal y, qreal rot, bool mir)
{
  QGraphicsItemGroup *ig = new QGraphicsItemGroup;
  PcbElement* e = _mElements[package];
  QVector<PcbComp *> *compList = e->components();

  for(int i = 0; i < compList->size(); i++)
    {
      int layer = (*compList)[i]->layer();

      if(mir)
        {
          switch(layer)
            {
            case  1: layer = 16; break;
            case 16: layer =  1; break;
            case 21: layer = 22; break;
            case 22: layer = 21; break;
            case 23: layer = 24; break;
            case 24: layer = 23; break;
            case 25: layer = 26; break;
            case 26: layer = 25; break;
            case 27: layer = 28; break;
            case 28: layer = 27; break;
            case 29: layer = 30; break;
            case 30: layer = 29; break;
            case 31: layer = 32; break;
            case 32: layer = 31; break;
            case 33: layer = 34; break;
            case 34: layer = 33; break;
            case 35: layer = 36; break;
            case 36: layer = 35; break;
            case 37: layer = 38; break;
            case 38: layer = 37; break;
            case 39: layer = 40; break;
            case 40: layer = 39; break;
            case 41: layer = 42; break;
            case 42: layer = 41; break;
            case 51: layer = 52; break;
            case 52: layer = 51; break;
            case 200: layer = 215; break;
            case 215: layer = 200; break;
            }
        }

      //      (*compList)[i]->setLayer(layer);
      (*compList)[i]->setColor(_mLayer[layer]->color());

      QGraphicsItem *item = (*compList)[i]->createGraphicsItem();

      item->setVisible(_mLayer[layer]->visible());
      ig->addToGroup(item);
      _mLayer[layer]->add(item);
    }

  ig->setPos(x,y);
  if(mir) ig->setScale(-1);
  ig->setRotation(rot);

  return ig;
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::drawElements(QDomNode &n)
{
  QDomNode dn = n.toElement().firstChild();
  QDomElement elem;
  while(!dn.isNull())
    {
      elem = dn.toElement();
      if(!elem.isNull())
        {
          QString package = elem.attribute("package");
          qreal x = elem.attribute("x").toDouble();
          qreal y = elem.attribute("y").toDouble();
          qreal rot = 0;
          bool mir = false;

          if(elem.hasAttribute("rot"))
            {
              QString strRotation = elem.attribute("rot");
              if(strRotation[0] == 'M')
                {
                  mir = true;
                  rot = strRotation.mid(2).toDouble();
                }
              else
                rot = strRotation.mid(1).toDouble();
            }

          QGraphicsItemGroup* ig = createGraphics(package, x, y, rot, mir);
          _scene->addItem(ig);
        }
      dn = dn.nextSibling();
    }
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::on_listWidget_itemChanged(QListWidgetItem *item)
{
  QVector<QGraphicsItem*>* elements = _mLayerItemList[item]->elements();
  for(int i = 0; i < elements->size(); i++){
      (*elements)[i]->setVisible(item->checkState() ? true : false);
    }
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::on_btnSelAll_clicked()
{
  foreach (QListWidgetItem* item, ui->listWidget->findItems("*", Qt::MatchWildcard)) {
      item->setCheckState(Qt::Checked);
    }
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::on_btnSelNone_clicked()
{
  foreach (QListWidgetItem* item, ui->listWidget->findItems("*", Qt::MatchWildcard)) {
      item->setCheckState(Qt::Unchecked);
    }
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::on_actionOpen_triggered()
{
  open();
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::on_actionClose_triggered()
{
  exit(0);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::on_actionPlay_triggered()
{
  _scene->clearSelection();                                                  // Selections would also render to the file
  _scene->setSceneRect(_scene->itemsBoundingRect());                          // Re-shrink the scene to it's bounding contents
  QSize size(_scene->sceneRect().size().toSize());
  size.setHeight(size.height() / 1000.0 * 25.4 / 1 * 42.6666666);
  size.setWidth(size.width() / 1000.0 * 25.4 / 75 * 3200);
  size.setWidth(size.width() + (8 - (size.width()%8)));
  QImage img(size, QImage::Format_ARGB32);  // Create the image with the exact size of the shrunk scene
  img.fill(Qt::transparent);                                              // Start all pixels transparent

  QPainter painter(&img);
  painter.setRenderHint(QPainter::Antialiasing);
  _scene->render(&painter,QRectF(0,0,size.width(),size.height()), _scene->itemsBoundingRect(),Qt::IgnoreAspectRatio);
  cout << "height=" << img.height() << " width=" << img.width() << endl;

  QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(img));
  _scene->clear();
  _scene->addItem(item);
  QGraphicsRectItem *rect = new QGraphicsRectItem(0,0,img.width(), img.height());
  QPen pen(Qt::red);
  rect->setPen(pen);
  _scene->addItem(rect);


  _laserExposer.moveTo(ExposerHardware::Axis::X, 0);
  while(_laserExposer.isMoving()){
      QApplication::processEvents();
      QThread::msleep(100);
    }

  _laserExposer.moveTo(ExposerHardware::Axis::Y, 0);
  while(_laserExposer.isMoving()) {
      QApplication::processEvents();
      QThread::msleep(100);
    }

  QByteArray data;
  data.fill(0, img.width()/8);


  qDebug() << "size of tmp = " << data.size();

  for(int i =0; i < img.height(); i++)
    {
      data.clear();
      data.fill(0, img.width()/8);
      for(int y = 0; y < img.width(); y++){
          QColor c = img.pixel(y,i);
          if(c==Qt::white)
            data[y/8] = data[y/8] | (1 << (y%8));
        }

      _laserExposer.beam(true, data);
      while(_laserExposer.isMoving()) {
          QApplication::processEvents();
          QThread::msleep(1);
        }
      _laserExposer.moveTo(ExposerHardware::Axis::X, i*75);
      while(_laserExposer.isMoving()){
          QApplication::processEvents();
          QThread::msleep(1);
        }
      _scene->addLine(0,i,img.width(),i,QPen(Qt::red,0));

      i++;

      data.clear();
      data.fill(0, img.width()/8);
      for(int y = 0; y < img.width(); y++){
          QColor c = img.pixel((img.width()-y-1),i);
          if(c==Qt::white)
            data[y/8] = data[y/8] | (1 << (y%8));
        }
      _laserExposer.beam(false, data);
      while(_laserExposer.isMoving()) {
          QApplication::processEvents();
          QThread::msleep(1);
        }
      _laserExposer.moveTo(ExposerHardware::Axis::X, i*75);
      while(_laserExposer.isMoving()){
          QApplication::processEvents();
          QThread::msleep(1);
        }
      _scene->addLine(0,i,img.width(),i,QPen(Qt::red,0));
    }

  qDebug() << "pcb done";

}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::on_portStateChange(bool state)
{
  ui->statusBar->showMessage(state ? "port opened" : "port closed", 2000);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::on_btnRefX_clicked()
{
  _laserExposer.referendeAxis(ExposerHardware::Axis::X);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::on_btnRefY_clicked()
{
  _laserExposer.referendeAxis(ExposerHardware::Axis::Y);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::on_btnEnable_clicked()
{
  _laserExposer.enable(true);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::on_btnDisable_clicked()
{
  _laserExposer.enable(false);
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::on_btnMoveX_clicked()
{
  _laserExposer.moveTo(ExposerHardware::Axis::X, (quint32) (ui->sbX->value()*3200.0));
}

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
void MainWindow::on_btnMoveY_clicked()
{
  _laserExposer.moveTo(ExposerHardware::Axis::Y, (quint32) (ui->sbY->value()/75.0*3200.0));
}
