# #####################################################################
# Automatically generated by qmake (2.01a) ?? ????. 21 22:29:12 2010
# #####################################################################
TEMPLATE = app
TARGET = scada

#INCLUDEPATH += . \
#	build/

 target.path = /tmp # path on device
    INSTALLS += target

QT += sql \
    svg \
    network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets uitools printsupport
lessThan(QT_MAJOR_VERSION, 5): CONFIG += uitools

unix:include (../../../lib/lib.pri)
win32:include (y:/lib/libw.pri)

win32:QTPLUGIN += qsvg qsqlmysql
win32:CONFIG += static

# QMAKE_POST_LINK = strip     -s    $(TARGET)   &&
#QMAKE_POST_LINK = cp     $(TARGET)     /home/ftp/vanessa/KSP/ && strip     -s    /home/ftp/vanessa/KSP/$(TARGET)

# Input


RESOURCES += \
    text.qrc \
    aparat/mnemo/apmnemo.qrc \
    centrif/mnemo/cfmnemo.qrc \
    centrif/resources.qrc \
    gaspich/gaspich.qrc \
    rou/rou.qrc \
    mnemo/mnemo.qrc

FORMS += \
    about.ui \
    alertviewdialog.ui \
    amVater.ui \
    dlgbledingsetup.ui \
    dlgmassview.ui \
    dlgpumpctrl.ui \
    dlgvbctrl.ui \
    fabreport.ui \
    history.ui \
    mainform.ui \
    mnemo.ui \
    onefilter.ui \
    sparamdialog.ui \
    tparamdialog.ui \
    aparat/apmnemo.ui \
    aparat/apone.ui \
    aparat/dlgalarm.ui \
    aparat/dlgapone.ui \
    aparat/dlgapsetup.ui \
    aparat/dlgtermctrl.ui \
    BigPumpsMon/bigpumpsmon.ui \
    centrif/mnemo/cfmnemo.ui \
    centrif/cfparam1.ui \
    centrif/one_mnemo1.ui \
    centrif/report.ui \
    gaspich/dialogsetup.ui \
    gaspich/dialogsovalka.ui \
    gaspich/dlgdidisplay.ui \
    gaspich/dlgerrlogview.ui \
    gaspich/gpmnemo.ui \
    gaspich/gpreport.ui \
    gaspich/gpSetup.ui \
    gaspich/setupgpi4.ui \
    rou/dlglimit.ui \
    rou/roumnemo.ui

HEADERS += \
    about.h \
    alertviewdialog.h \
    amVater.h \
    dlgbledingsetup.h \
    dlgmassview.h \
    dlgpumpctrl.h \
    dlgvbctrl.h \
    fabreport.h \
    mainform.h \
    mnemo.h \
    sparamdialog.h \
    aparat/apmnemo.h \
    aparat/apone.h \
    aparat/dlgalarm.h \
    aparat/dlgapone.h \
    aparat/dlgapsetup.h \
    aparat/dlgtermctrl.h \
    aparat/valve_mask.h \
    BigPumpsMon/bigpumpsmon.h \
    BigPumpsMon/xudpclient.h \
    BigPumpsMon/xudpstore.h \
    centrif/cfmnemo.h \
    centrif/cfparam1.h \
    centrif/one_mnemo1.h \
    centrif/report.h \
    gaspich/dialogsetup.h \
    gaspich/dialogsovalka.h \
    gaspich/dlgdidisplay.h \
    gaspich/dlgerrlogview.h \
    gaspich/gpmnemo.h \
    gaspich/gpreport.h \
    gaspich/setupgpi4.h \
    rou/dlglimit.h \
    rou/roumnemo.h

SOURCES += \
    alertviewdialog.cpp \
    amVater.cpp \
    dlgbledingsetup.cpp \
    dlgmassview.cpp \
    dlgpumpctrl.cpp \
    dlgvbctrl.cpp \
    fabreport.cpp \
    main.cpp \
    mainform.cpp \
    mnemo.cpp \
    sparamdialog.cpp \
    aparat/apmnemo.cpp \
    aparat/apone.cpp \
    aparat/dlgalarm.cpp \
    aparat/dlgapone.cpp \
    aparat/dlgapsetup.cpp \
    aparat/dlgtermctrl.cpp \
    BigPumpsMon/bigpumpsmon.cpp \
    BigPumpsMon/xudpclient.cpp \
    BigPumpsMon/xudpstore.cpp \
    centrif/cfmnemo.cpp \
    centrif/cfparam1.cpp \
    centrif/one_mnemo1.cpp \
    centrif/report.cpp \
    gaspich/dialogsetup.cpp \
    gaspich/dialogsovalka.cpp \
    gaspich/dlgdidisplay.cpp \
    gaspich/dlgerrlogview.cpp \
    gaspich/gpmnemo.cpp \
    gaspich/gpreport.cpp \
    gaspich/setupgpi4.cpp \
    rou/dlglimit.cpp \
    rou/roumnemo.cpp

DISTFILES += \
    text/1.png \
    text/a1ap \
    text/a1sir1 \
    text/a1sir2 \
    text/a1trend \
    text/a1valves \
    text/a1whs \
    text/d1level \
    text/d1reg01 \
    text/d1reg02 \
    text/d1reg03 \
    text/d1reg04 \
    text/d1reg05 \
    text/d1reg06 \
    text/d1reg07 \
    text/d1reg08 \
    text/d1reg09 \
    text/d1temp \
    text/d1water \
    text/frdrv01 \
    text/frdrv02 \
    text/frdrv03 \
    text/frdrv04 \
    text/frdrv05 \
    text/frdrv06 \
    text/frdrv07 \
    text/frdrv08 \
    text/frdrv09 \
    text/frdrv10 \
    text/frdrv11 \
    text/frdrv12 \
    text/frdrv13 \
    text/frdrv14 \
    text/frdrv15 \
    text/frdrv21 \
    text/frdrv22 \
    text/frdrv23 \
    text/gperrormsg.txt \
    text/gplist.txt \
    text/list.txt \
    text/reg_d1.txt \
    text/reg_g1.txt \
    text/reg_rou.txt \
    text/reg_s1.txt \
    text/reg_v1.txt \
    text/roulist.txt \
    text/roureg3 \
    text/roureg4 \
    text/roureg5 \
    text/routrend \
    text/routrend1 \
    text/routrend2 \
    text/s1bleding \
    text/s1flowj \
    text/s1flowr \
    text/s1flowvm \
    text/s1level1sat \
    text/s1level2sat \
    text/s1leveldef \
    text/s1levelpr \
    text/s1phco2 \
    text/s1pump \
    text/s1reg01 \
    text/s1reg02 \
    text/s1reg03 \
    text/s1reg04 \
    text/s1reg05 \
    text/s1reg06 \
    text/s1reg07 \
    text/s1reg08 \
    text/s1reg09 \
    text/s1reg10 \
    text/s1reg11 \
    text/s1reg12 \
    text/s1reg13 \
    text/s1reg14 \
    text/s1reg15 \
    text/s1reg16 \
    text/s1reg17 \
    text/s1reg18 \
    text/s1term1 \
    text/s1term2 \
    text/s1termpr \
    text/s1trend00 \
    text/trclist.txt \
    text/trend1 \
    text/trend2 \
    text/trend3 \
    text/trend4 \
    text/trend5 \
    text/trend6 \
    text/trend7 \
    text/trend8 \
    text/trend9 \
    text/v1amvoda \
    text/v1level \
    text/v1presh \
    text/v1reg01 \
    text/v1reg02 \
    text/v1reg03 \
    text/v1reg04 \
    text/v1reg05 \
    text/v1reg06 \
    text/v1reg07 \
    text/v1reg08 \
    text/v1reg09 \
    text/v1reg10 \
    text/v1reg11 \
    text/v1reg12 \
    text/v1reg13 \
    text/v1reg14 \
    text/v1reg15 \
    text/v1reg16 \
    text/v1reg18 \
    text/v1temp \
    text/v1water \
    ../AndroidManifest.xml \
    ../gradle/wrapper/gradle-wrapper.jar \
    ../gradlew \
    ../res/values/libs.xml \
    ../build.gradle \
    ../gradle/wrapper/gradle-wrapper.properties \
    ../gradlew.bat \
    text/d1reg11 \
    text/d1reg13 \
    text/d1reg14

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../

