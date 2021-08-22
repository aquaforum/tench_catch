# -------------------------------------------------
# Project created by QtCreator 2010-02-25T14:01:57
# -------------------------------------------------
QT += network \
    sql
QT -= gui
TARGET = ioserv
CONFIG -= app_bundle
TEMPLATE = app
HEADERS += reportwr.h
SOURCES += main.cpp \
    reportwr.cpp
MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build
RCC_DIR = build
OTHER_FILES += list.txt \
    list2.txt \
    listall.txt \
    list.txt
RESOURCES += resources.qrc
QMAKE_LIBDIR += ../../../lib
LIBS += -lrcada
INCLUDEPATH += ../../../lib/include
PRE_TARGETDEPS += ../../../lib/librcada.a


#QMAKE_POST_LINK = strip     -s    $(TARGET)   &&     cp     $(TARGET)     /home/ftp/vanessa/LIN/centrif/


