QT -= gui
QT += network \
    sql
OTHER_FILES += list.txt \
    list2.txt
HEADERS += reportwr.h
SOURCES += reportwr.cpp \
    main.cpp
RESOURCES += test.qrc
QMAKE_LIBDIR += ../../../lib
LIBS += -lrcada
INCLUDEPATH += ../../../lib/include
PRE_TARGETDEPS += ../../../lib/librcada.a

#QMAKE_POST_LINK = strip     -s    $(TARGET)   &&     cp     $(TARGET)     /home/ftp/vanessa/LIN/kf-1000/
