#-------------------------------------------------
#
# Project created by QtCreator 2013-11-15T14:00:31
#
#-------------------------------------------------

QT       += core gui widgets testlib 

TARGET = test_smoothSqrt
TEMPLATE = app

include(RsaToolbox/rsatoolbox.pri)

INCLUDEPATH += $$PWD
SOURCES += test_smoothSqrt.cpp
OTHER_FILES += "./Results/smoothSqrt Test.txt"
DEFINES += SRCDIR=\\\"$$PWD/\\\"


