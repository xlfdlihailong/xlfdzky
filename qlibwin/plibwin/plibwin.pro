TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    plib.cpp \
    pxml.cpp \
    tinystr.cpp \
    tinyxml.cpp \
    tinyxmlerror.cpp \
    tinyxmlparser.cpp \
    mainplib.cpp \
    pudp.cpp \
    ptcp.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    plib.h \
    pxml.h \
    tinystr.h \
    tinyxml.h \
    pudp.h \
    ptcp.h \
    test/testptcp.h \
    test/testptime.h \
    test/testpudp.h \
    test/testserialization.h \
    test/testudp.h

TARGET=plibwin
