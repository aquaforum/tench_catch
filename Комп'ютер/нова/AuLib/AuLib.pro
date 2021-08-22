#-------------------------------------------------
#
# Project created by QtCreator 2019-04-01T13:32:22
#
#-------------------------------------------------

QT       += widgets network sql uitools

TARGET = AuLib
TEMPLATE = lib
CONFIG += staticlib

DEFINES += AULIB_LIBRARY

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build
RCC_DIR = build

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AuWidget/auhistory.cpp \
    AuWidget/aulamp.cpp \
    AuWidget/aupanelreg.cpp \
    AuWidget/auplcsetup.cpp \
    AuWidget/autagsetup.cpp \
    AuWidget/autrendplot.cpp \
    AuWidget/autrendview.cpp \
        aubase.cpp \
    aucollectdata.cpp \
    aulogging.cpp \
    aumelsec.cpp \
    auplc.cpp \
    auplcdrive.cpp \
    ausetup.cpp \
    ausqlexecuter.cpp \
    ausqlquery.cpp \
    autcpreceiver.cpp \
    autcptransmitter.cpp \
    auudp.cpp \
    aumodbus.cpp \
    autag.cpp \
    auudptransiver.cpp \
    scale/scale.cpp \
    trcdataloader.cpp \
    trendchar/trendchart.cpp

HEADERS += \
    AuWidget/AuTrendPlot.h \
    AuWidget/auhistory.h \
    AuWidget/aulamp.h \
    AuWidget/aupanelreg.h \
    AuWidget/auplcsetup.h \
    AuWidget/autagsetup.h \
    AuWidget/autrendview.h \
    AuWidget/datetimedialog.h \
        aubase.h \
    aucollectdata.h \
        aulib_global.h \
    aulibdefs.h \
    aulogging.h \
    aumelsec.h \
    auplc.h \
        auplcdrive.h \
    auprivatedev.h \
    ausetup.h \
    ausqlexecuter.h \
    ausqlquery.h \
    autcpreceiver.h \
    autcptransmitter.h \
    auudp.h \
    aumodbus.h \
    autag.h \
    aulib.h \
    auudptransiver.h \
    scale/scale.h \
    trcdataloader.h \
    trendchar/trendchart.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

QMAKE_CXXFLAGS_RELEASE += -ffast-math
QMAKE_CXXFLAGS_DEBUG += -ffast-math

FORMS += \
    AuWidget/aupanelreg.ui \
    AuWidget/autagsetup.ui \
    AuWidget/autrendview.ui \
    AuWidget/datetime.ui

RESOURCES += \
    AuLib.qrc

DISTFILES += \
    sql_опис
