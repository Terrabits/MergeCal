#-------------------------------------------------
# 
# Project  : MergeCalTest
# App name : My RSA App
# 
# 
# RsaToolbox
# Template : Rsa Vna Macro Without Ui
#            Version 2.0.0
# 
# (C) Rohde & Schwarz America
# 
#-------------------------------------------------


QT          += core gui widgets
TARGET       = VnaCalKitTest
TEMPLATE     = app

include(../RsaToolbox/rsatoolbox.pri)
#include(../RsaToolbox/QuaZip/quazip.pri)
HEADERS     += VnaCalKitSettings.h
#INCLUDEPATH += $$PWD
SOURCES     += VnaCalKitTest.cpp
#RESOURCES   += Resources.qrc
#OTHER_FILES += DEBUG_LOGFILE.txt

# Executable Icon
#win32: RC_FILE = VS2010Resources.rc

DEFINES     += SOURCE_DIR=\\\"$$PWD\\\"
CONFIG(debug, debug|release):DEFINES += DEBUG_MODE
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

