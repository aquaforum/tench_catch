MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build
RCC_DIR = build


RESOURCES += $$PWD/picture.qrc 
#    ../../../pict/lib/valves.qrc \

INCLUDEPATH += $$PWD/include

FORMS += $$PWD/trend/datetime.ui


QMAKE_LIBDIR += ../../../lib

#unix:lessThan(QT_MAJOR_VERSION, 5): QMAKE_LIBDIR += /home/vanessa/src/qt4/lib
#unix:greaterThan(QT_MAJOR_VERSION, 4):  QMAKE_LIBDIR += /home/vanessa/src/qt5/lib

#win32:lessThan(QT_MAJOR_VERSION, 5):QMAKE_LIBDIR += C:/Qt/work/lib
#win32::greaterThan(QT_MAJOR_VERSION, 4):QMAKE_LIBDIR +=../../../lib


LIBS += -lrcada_client

PRE_TARGETDEPS += ../../../lib/librcada_client.a

#unix:lessThan(QT_MAJOR_VERSION, 5): PRE_TARGETDEPS += /home/vanessa/src/qt4/lib/librcada_client.a
#unix:greaterThan(QT_MAJOR_VERSION, 4): PRE_TARGETDEPS += /home/vanessa/src/qt5/lib/librcada_client.a

