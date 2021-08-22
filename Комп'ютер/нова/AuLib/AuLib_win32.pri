
RESOURCES += \
    $$PWD/AuLib.qrc \
    $$PWD/picture.qrc

FORMS += \
    $$PWD/AuWidget/datetime.ui 

INCLUDEPATH += $$PWD

QMAKE_LIBDIR += c:/Qt/work/AuLib

LIBS += -lAuLib
PRE_TARGETDEPS += c:/Qt/work/AuLib/libAuLib.a


MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build
RCC_DIR = build

QT += network sql uitools svg



