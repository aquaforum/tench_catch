# -------------------------------------------------
# Project created by QtCreator 2010-02-25T14:01:57
# -------------------------------------------------
QT += network \
    sql
QT -= gui
TARGET = ioserv
CONFIG -= app_bundle
CONFIG += debug_and_release
TEMPLATE = app
SOURCES += main.cpp
MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build
RCC_DIR = build
OTHER_FILES += list.txt \
    list2.txt
RESOURCES += test.qrc
QMAKE_LIBDIR += ../../../lib
LIBS += -lrcada
INCLUDEPATH += ../../../lib/include

#QMAKE_POST_LINK = strip     -s    $(TARGET)   &&     cp     $(TARGET)     /home/ftp/vanessa/LIN/aparat/
