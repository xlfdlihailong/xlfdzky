#-------------------------------------------------
#
# Project created by QtCreator 2020-12-31T00:59:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xlfdzky
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qlibwin/qlib.cpp \
    qlibwin/plibwin/plib.cpp \
    qlibwin/plibwin/ptcp.cpp \
    qlibwin/plibwin/pudp.cpp \
    qlibwin/plibwin/pxml.cpp \
    qlibwin/plibwin/tinystr.cpp \
    qlibwin/plibwin/tinyxml.cpp \
    qlibwin/plibwin/tinyxmlerror.cpp \
    qlibwin/plibwin/tinyxmlparser.cpp \
    xtablewidget.cpp \
    xtreewidget.cpp

HEADERS  += mainwindow.h \
    qlibwin/qlib.h \
    qlibwin/plibwin/plib.h \
    qlibwin/plibwin/ptcp.h \
    qlibwin/plibwin/pudp.h \
    qlibwin/plibwin/pxml.h \
    qlibwin/plibwin/tinystr.h \
    qlibwin/plibwin/tinyxml.h \
    xtablewidget.h \
    xtreewidget.h \
    interface.h

FORMS    += mainwindow.ui \
    xtablewidget.ui \
    xtreewidget.ui

RESOURCES += \
    xlfd.qrc
RC_ICONS =app.ico
