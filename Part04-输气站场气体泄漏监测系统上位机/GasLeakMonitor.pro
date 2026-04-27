#-------------------------------------------------
#
# Project created by QtCreator 2020-12-04T14:44:55
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += printsupport
QT       += sql
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GasLeakMonitor
TEMPLATE = app

include(src/qcustomplot/qcustomplot.pri)        #第三方绘图图
include(src/database/database.pri)              #数据库
include(src/serial/serialport.pri)              #串口
# include(src/mainwidget/mainwidget.pri)          #主界面
include(src/timethread/timethread.pri)          #时间显示
# include(src/CJSON/cjson.pri)


INCLUDEPATH += $$PWD/src/qcustomplot
INCLUDEPATH += $$PWD/src/database
INCLUDEPATH += $$PWD/src/serial
INCLUDEPATH += $$PWD/src/timethread
# INCLUDEPATH += $$PWD/src/mainwidget

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    cJSON.cpp \
    main.cpp \
    gasleakmonitor.cpp \

HEADERS += \
    cJSON.h \
    gasleakmonitor.h \
    mynode.h \
    testthread.h

FORMS += \
    gasleakmonitor.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    gasleak.qrc


