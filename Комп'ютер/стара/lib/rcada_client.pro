# -------------------------------------------------
# Project created by QtCreator 2010-04-13T22:02:46
# -------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets uitools

QT += network \
    sql

TEMPLATE = lib
CONFIG += staticlib
lessThan(QT_MAJOR_VERSION, 5): CONFIG += uitools

HEADERS += iodev.h \
    header.h \
    ionetclient/netiodev.h \
    ionetclient/IoNetClient.h \
    trendchar/trendchart.h \
    trend/trend.h \
    trend/datetimedialog.h \
    RotatedLabel/rotatedlabel.h \
    panelReg/panelReg.h \
    scale/scale.h \
    trend/trendconstruct.h \
    history/history.h \
    trend/historythread.h \
    panelReg/trendloadthead.h \
    keyboardwin/keyboardwin.h \
    TpanelReg/TpanelReg.h
    #ioudpclient/IoUdpClient.h
SOURCES += ionetclient/IoNetClient.cpp \
    ionetclient/netiodev.cpp \
    iodev.cpp \
    trendchar/trendchart.cpp \
    trend/trend.cpp \
    RotatedLabel/rotatedlabel.cpp \
    panelReg/panelReg.cpp \
    scale/scale.cpp \
    trend/trendconstruct.cpp \
    history/history.cpp \
    trend/historythread.cpp \
    panelReg/trendloadthead.cpp \
    keyboardwin/keyboardwin.cpp \
    TpanelReg/TpanelReg.cpp
    #ioudpclient/IoUdpClient.cpp
MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build
RCC_DIR = build
VERSION = 0.1.0
RESOURCES += trend/trend.qrc \
    picture.qrc
FORMS += trend/trend.ui \
    trend/datetime.ui \
    panelReg/panelReg.ui \
    trend/trendconstruct.ui \
    keyboardwin/keyboardwin.ui \
    TpanelReg/TpanelReg.ui
INCLUDEPATH += include

QMAKE_CXXFLAGS_RELEASE += -ffast-math -Wreorder

OTHER_FILES += \
    TpanelReg/slider.svg
