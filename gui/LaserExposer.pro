#-------------------------------------------------
#
# Project created by QtCreator 2014-10-19T17:38:11
#
#-------------------------------------------------

QT       += core gui
QT       += xml
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LaserExposer
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

SOURCES += src/pcb/layer.cpp \
    src/pcb/pad.cpp \
    src/pcb/pcb.cpp \
    src/pcb/pcbcomponent.cpp \
    src/pcb/pcbelement.cpp \
    src/pcb/pcbgraphicsscene.cpp \
    src/pcb/pcbgraphicsview.cpp \
    src/pcb/qgraphicsarcitem.cpp \
    src/pcb/rectangle.cpp \
    src/pcb/smd.cpp \
    src/pcb/via.cpp \
    src/pcb/wire.cpp \
    src/exposerhardware.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS  += src/pcb/layer.h \
    src/pcb/pad.h \
    src/pcb/pcb.h \
    src/pcb/pcbcomponent.h \
    src/pcb/pcbelement.h \
    src/pcb/pcbgraphicsscene.h \
    src/pcb/pcbgraphicsview.h \
    src/pcb/qgraphicsarcitem.h \
    src/pcb/rectangle.h \
    src/pcb/smd.h \
    src/pcb/via.h \
    src/pcb/wire.h \
    src/exposerhardware.h \
    src/mainwindow.h \
    src/sendbuffer.h

FORMS    += src/mainwindow.ui

OTHER_FILES += \
    src/pcb.xml

RESOURCES += \
    res/icons.qrc
