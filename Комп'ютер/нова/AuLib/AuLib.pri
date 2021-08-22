
# RESOURCES += $$PWD/picture.qrc

HOME=$$PWD/../../
 
RESOURCES += \
    $$PWD/AuLib.qrc \
    $$PWD/picture.qrc


FORMS += \
    $$PWD/AuWidget/datetime.ui 

INCLUDEPATH += $$PWD

QMAKE_LIBDIR += $$HOME/src/AuLibBuild

LIBS += -lAuLib
PRE_TARGETDEPS += $$HOME/src/AuLibBuild/libAuLib.a


MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build
RCC_DIR = build

QMAKE_CXXFLAGS_RELEASE += -ffast-math
QMAKE_CXXFLAGS_DEBUG += -ffast-math

QT += network sql uitools
