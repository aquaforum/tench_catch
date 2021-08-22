!defined(HOMELIBPATH,var): HOMELIBPATH=/home/vanessa/src
unix: include ( $$HOMELIBPATH/AuLib/AuLib.pri)
win32: include (y:/AuLib/AuLib_win32.pri)

QT       += core gui printsupport
QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    auscada.qrc \
    centrif/mnemo/cfmnemo.qrc \
    tmaaparat/tmaaparat.qrc

FORMS += \
    about.ui \
    alertviewdialog.ui \
    amVater.ui \
    boiler2/mnemoboiler2.ui \
    dlgflokctl.ui \
    tmaaparat/apmainform.ui \
    tmaaparat/apone.ui \
    dlgbledingsetup.ui \
    dlgmassview.ui \
    dlgpumpctrl.ui \
    dlgvbctrl.ui \
    fabreport.ui \
    formreporttest.ui \
    history.ui \
    mainform.ui \
    mnemo.ui \
    onefilter.ui \
    sparamdialog.ui \
    tmaaparat/apcommon.ui \
    tmaaparat/apsetupcommon.ui \
    tmaaparat/apsetupone.ui \
    tmaaparat/apsgkl.ui \
    tmaaparat/apsingle.ui \
    tmaaparat/aptrendselect.ui \
    tmaaparat/apvatrendselect.ui \
    tmaaparat/apwincurkyl.ui \
    tmaaparat/apzagtrendselect.ui \
    tparamdialog.ui \
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
    boiler2/mnemoboiler2.h \
    dlgflokctl.h \
    fabmnemo.h \
    mainwidget.h \
    reporwrmgr.h \
    tmaaparat/apmainform.h \
    tmaaparat/apone.h \
    dlgbledingsetup.h \
    dlgmassview.h \
    dlgpumpctrl.h \
    dlgvbctrl.h \
    fabreport.h \
    fabreportwriter.h \
    formreporttest.h \
    gaspich/gpreportwr.h \
    gaspich/mnemo_gp.h \
    mainform.h \
    mnemo.h \
    BigPumpsMon/bigpumpsmon.h \
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
    rou/roumnemo.h \
    sparamdialog.h \
    tmaaparat/apcommon.h \
    tmaaparat/apsetupcommon.h \
    tmaaparat/apsetupone.h \
    tmaaparat/apsgkl.h \
    tmaaparat/apsingle.h \
    tmaaparat/aptrendselect.h \
    tmaaparat/apvatrendselect.h \
    tmaaparat/apwincurkyl.h \
    tmaaparat/apzagtrendselect.h \
    tmaaparat/bitmask.h \
    tmaaparat/errormanager.h \
    tmaaparat/kod_data.h \
    tmaaparat/valve_mask.h

SOURCES += \
    alertviewdialog.cpp \
    amVater.cpp \
    boiler2/mnemoboiler2.cpp \
    dlgflokctl.cpp \
    fabmnemo.cpp \
    mainwidget.cpp \
    reporwrmgr.cpp \
    tmaaparat/apmainform.cpp \
    tmaaparat/apone.cpp \
    dlgbledingsetup.cpp \
    dlgmassview.cpp \
    dlgpumpctrl.cpp \
    dlgvbctrl.cpp \
    fabreport.cpp \
    fabreportwriter.cpp \
    formreporttest.cpp \
    gaspich/gpreportwr.cpp \
    gaspich/mnemo_gp.cpp \
    main.cpp \
    mainform.cpp \
    mnemo.cpp \
    BigPumpsMon/bigpumpsmon.cpp \
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
    rou/roumnemo.cpp \
    sparamdialog.cpp \
    tmaaparat/apcommon.cpp \
    tmaaparat/apsetupcommon.cpp \
    tmaaparat/apsetupone.cpp \
    tmaaparat/apsgkl.cpp \
    tmaaparat/apsingle.cpp \
    tmaaparat/aptrendselect.cpp \
    tmaaparat/apvatrendselect.cpp \
    tmaaparat/apwincurkyl.cpp \
    tmaaparat/apzagtrendselect.cpp \
    tmaaparat/bitmask.cpp \
    tmaaparat/errormanager.cpp

DISTFILES += \
    plc/boiler2.txt \
    text/reg_ap.txt \
    text/reg_boiler2.txt \
    text/reg_d1.txt \
    text/reg_g1.txt \
    text/reg_ob.txt \
    text/reg_rou.txt \
    text/reg_s1.txt \
    text/reg_v1.txt \
    tmaaparat/text/sgkl.list \
    tmaaparat/text/vaalert.list \
    trends.list
