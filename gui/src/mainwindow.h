#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDomDocument>
#include <QGraphicsScene>
#include <QMainWindow>

#include "pcb/pcb.h"
#include "pcb/pcbgraphicsscene.h"
#include "exposerhardware.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_listWidget_itemChanged(QListWidgetItem *item);

    void on_btnSelAll_clicked();

    void on_btnSelNone_clicked();

    void on_actionOpen_triggered();

    void on_actionClose_triggered();

    void on_btnRefX_clicked();

    void on_btnRefY_clicked();

    void on_btnEnable_clicked();

    void on_btnDisable_clicked();

    void on_btnMoveX_clicked();

    void on_btnMoveY_clicked();

    void on_actionPlay_triggered();

private:
    Ui::MainWindow *ui;
//    Pcb _pcb;
    void openBrdFile(QString fileName);
    void parseXML(QDomDocument& d);
    PcbElement* parsePcbComponents(QDomNode& n);
    void parsePcbComponents(QDomNode& n, PcbElement* e);
    void parseLayers(QDomNode& n);
    void parseDesignrules(QDomNode& n);
    void parsePackages(QDomNode& n);
    QGraphicsItemGroup* createGraphics(QString package, qreal x = 0, qreal y = 0, qreal rot = 0, bool mir = false);
    void drawElements(QDomNode& n);

    PcbGraphicsScene *_scene;
    QDomNode elemLayers;
    QMap<QListWidgetItem*, Layer*> _mLayerItemList;
    QMap<int, Layer*> _mLayer;
    QMap<QString, PcbElement*> _mElements;
    ExposerHardware _laserExposer;

    const double scaleFactor;
    void openBmpFile(QString fileName);
    void open();
};

#endif // MAINWINDOW_H
