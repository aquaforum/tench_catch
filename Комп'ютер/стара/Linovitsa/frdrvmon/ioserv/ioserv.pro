# -------------------------------------------------
# Project created by QtCreator 2010-02-25T14:01:57
# -------------------------------------------------
QT += network \
    sql
QT -= gui
TARGET = ioserv
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp

# MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build
RCC_DIR = build
OTHER_FILES += list.txt \
    alert.txt
RESOURCES += test.qrc
QMAKE_LIBDIR += ../../../lib
LIBS += -lrcada
INCLUDEPATH += ../../../lib/include
PRE_TARGETDEPS += ../../../lib/librcada.a

DISTFILES += \
    listfr.txt
