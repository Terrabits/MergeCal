#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24T14:35:10
#
#-------------------------------------------------

QT       += core testlib
TEMPLATE = app
TARGET = Test
CONFIG   -= app_bundle
CONFIG   += console \
		    testcase

include(../core.pri)
SOURCES += \
    TestDoubleOffsetShortKit.cpp

DEFINES     += SOURCE_DIR=\\\"$$PWD\\\"
