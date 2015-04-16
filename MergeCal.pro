#-------------------------------------------------
# 
# Project  : MergeCal
# App name : R&S Merge Cal
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
TARGET       = MergeCal
TEMPLATE     = app

include(RsaToolbox/rsatoolbox.pri)
#include(RsaToolbox/QuaZip/quazip.pri)
HEADERS     += Settings.h \
    PortsPage.h \
    CalKitsPage.h \
    SetupPage.h \
    MeasurePage.h \
    PortsModel.h \
    PortsView.h
INCLUDEPATH += $$PWD
SOURCES     += main.cpp \
    PortsPage.cpp \
    CalKitsPage.cpp \
    SetupPage.cpp \
    MeasurePage.cpp \
    PortsModel.cpp \
    PortsView.cpp
RESOURCES   += Resources.qrc
OTHER_FILES += DEBUG_LOGFILE.txt

# Executable Icon
win32: RC_FILE = VS2010Resources.rc

DEFINES     += SOURCE_DIR=\\\"$$PWD\\\"
CONFIG(debug, debug|release):DEFINES += DEBUG_MODE
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

FORMS += \
    PortsPage.ui \
    CalKitsPage.ui \
    SetupPage.ui \
    MeasurePage.ui

