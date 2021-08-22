# -------------------------------------------------
# Project created by QtCreator 2010-02-25T14:01:57
# -------------------------------------------------
QT += network \
    sql
QT -= gui
TARGET = ioserv
CONFIG -= app_bundle
TEMPLATE = app

RESOURCES += resources.qrc

SOURCES += main.cpp \
    RxModbusS.cpp

HEADERS += \
    RxModbusS.h


include (../../../lib/lib.pri)
include (../../../lib/libs.pri)

DISTFILES += \
    list2.txt
