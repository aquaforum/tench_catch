greaterThan(QT_MAJOR_VERSION, 4): QT += widgets uitools

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
    alertviewdialog.h \
    about.h 

FORMS += mainform.ui \
    sparamdialog.ui \
    about.ui \
    alertviewdialog.ui \
    history.ui 

SOURCES += main.cpp \
    mainform.cpp \
    mnemo.cpp \
    alertviewdialog.cpp \
    sparamdialog.cpp 

RESOURCES += scada.qrc 
#    ../../../pict/lib/valves.qrc


OTHER_FILES += text/alert.txt \
    text/trend 


include (/home/vanessa/src/qt5/lib/lib.pri)

win32:QTPLUGIN += qsvg qsqlmysql
win32:CONFIG += static


CONFIG -= app_bundle
CONFIG += debug_and_release
LIBS += -lvlc

#QMAKE_POST_LINK = strip     -s    $(TARGET)   &&     cp     $(TARGET)     /home/ftp/vanessa/LIN/scada/
