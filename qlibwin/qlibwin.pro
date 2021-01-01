#-------------------------------------------------
#
# Project created by QtCreator 2020-09-04T12:38:29
#
#-------------------------------------------------

QT       += core network gui widgets

TARGET = qlib
CONFIG   += console
#CONFIG   -= app_bundle
CONFIG +=c++11
TEMPLATE = app


SOURCES += \
    qlib.cpp \
    mainqlib.cpp \
    plibwin/plib.cpp \
    plibwin/pxml.cpp \
    plibwin/tinystr.cpp \
    plibwin/tinyxml.cpp \
    plibwin/tinyxmlerror.cpp \
    plibwin/tinyxmlparser.cpp \
    plibwin/ptcp.cpp

HEADERS += \
    qlib.h \
    plibwin/plib.h \
    plibwin/pxml.h \
    plibwin/tinystr.h \
    plibwin/tinyxml.h \
    plibwin/ptcp.h
TARGET=qlibwin
