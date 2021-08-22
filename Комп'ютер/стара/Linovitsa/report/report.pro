#-------------------------------------------------
#
# Project created by QtCreator 2014-09-30T13:55:14
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui
include(../../lib/lib.pri)
QMAKE_LIBDIR += ../../lib

TARGET = report
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    lireport.cpp


HEADERS += \
    lireport.h
