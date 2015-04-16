#-------------------------------------------------
#
# Project created by QtCreator 2013-11-15T14:00:31
#
#-------------------------------------------------

QT       += core gui testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_smoothSqrt
TEMPLATE = app

include(RsaToolbox/base.pri)
include(RsaToolbox/Bus/Rsib/rsib.pri)
include(RsaToolbox/Bus/Visa/visa.pri)
include(RsaToolbox/QCustomPlot/qcustomplot.pri)

INCLUDEPATH += $$PWD
SOURCES += test_smoothSqrt.cpp
OTHER_FILES += ./Results/smoothSqrt Test.txt
DEFINES += SRCDIR=\\\"$$PWD/\\\"


