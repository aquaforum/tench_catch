CONFIG += debug_and_release
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
    history.h \
    alertviewdialog.h \
    kfone.h \
    kfupld.h \
    kfpanel.h \
    dlgkfmenu.h \
    dlgkfparam.h \
    dlgvodactrl.h \
    dlgptsctrl.h \
    report.h \
    dlggvodactrl.h
FORMS += mainform.ui \
    mnemo.ui \
    sparamdialog.ui \
    about.ui \
    history.ui \
    alertviewdialog.ui \
    kfone.ui \
    kfupld.ui \
    kfpanel.ui \
    dlgkfmenu.ui \
    dlgkfparam.ui \
    dlgvodactrl.ui \
    dlgptsctrl.ui \
    report.ui \
    ../../../lib/trend/datetime.ui \
    dlggvodactrl.ui
SOURCES += main.cpp \
    mainform.cpp \
    mnemo.cpp \
    sparamdialog.cpp \
    history.cpp \
    alertviewdialog.cpp \
    kfone.cpp \
    kfupld.cpp \
    kfpanel.cpp \
    dlgkfmenu.cpp \
    dlgkfparam.cpp \
    dlgvodactrl.cpp \
    dlgptsctrl.cpp \
    report.cpp \
    dlggvodactrl.cpp
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
INCLUDEPATH += ../../../../lib/include
OTHER_FILES += text/alert.txt \
    text/reg.txt
win32:QTPLUGIN += qsvg qsqlmysql
win32:CONFIG += static
PRE_TARGETDEPS += ../../../lib/librcada_client.a

#QMAKE_POST_LINK = strip     -s    $(TARGET)   &&     cp     $(TARGET)     /home/ftp/vanessa/LIN/kf-1000/

DISTFILES += \
    text/trans
