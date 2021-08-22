#-------------------------------------------------
#
# Project created by QtCreator 2016-09-18T14:05:50
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

#include(../../lib/lib.pri)
#QMAKE_LIBDIR += ../../lib

TARGET = hhreport2
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    hhreport.cpp

HEADERS += \
    hhreport.h
