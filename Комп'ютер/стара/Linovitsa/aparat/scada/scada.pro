DEPENDPATH += . \
    mnemo
INCLUDEPATH += .
QT += sql \
    svg \
    network

# Input
HEADERS += mainform.h \
    mnemo.h \
    sparamdialog.h \
    about.h \
    alertviewdialog.h \
    apone.h \
    dlgapone.h \
    dlgapsetup.h \
    dlgtermctrl.h \
    dlgalarm.h
FORMS += mainform.ui \
    mnemo.ui \
    sparamdialog.ui \
    about.ui \
    history.ui \
    alertviewdialog.ui \
    apone.ui \
    dlgapone.ui \
    dlgapsetup.ui \
    dlgtermctrl.ui \
    dlgalarm.ui
SOURCES += main.cpp \
    mainform.cpp \
    mnemo.cpp \
    sparamdialog.cpp \
    alertviewdialog.cpp \
    apone.cpp \
    dlgapone.cpp \
    dlgapsetup.cpp \
    dlgtermctrl.cpp \
    dlgalarm.cpp
RESOURCES += mnemo/mnemo.qrc \
    ../../../lib/picture.qrc \
    ../../../pict/lib/valves.qrc \
    text.qrc
MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build
RCC_DIR = build
QMAKE_LIBDIR += ../../../lib
LIBS += -lrcada_client
INCLUDEPATH += ../../../lib/include
OTHER_FILES += text/alert.txt \
    text/trend \
    text/ap
win32:QTPLUGIN += qsvg qsqlmysql
win32:CONFIG += static
PRE_TARGETDEPS += ../../../lib/librcada_client.a

CONFIG -= app_bundle
CONFIG += debug_and_release uitools

QMAKE_POST_LINK = strip     -s    $(TARGET)   &&     cp     $(TARGET)     /home/ftp/vanessa/LIN/aparat/
