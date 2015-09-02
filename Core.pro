
QT      += core gui widgets
TEMPLATE = lib
CONFIG  += staticlib
DESTDIR  = $$PWD
TARGET   = Core
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)

include(RsaToolbox/rsatoolbox.pri)
HEADERS += Settings.h \
           PortsPage.h \
           CalKitsPage.h \
           SetupPage.h \
           MeasurePage.h \
           PortsModel.h \
           PortsView.h \
           AvailableCalKitsModel.h \
           AvailableCalKitsView.h \
           DoubleOffsetShortKit.h \
           FrequencyRange.h \
           ChosenCalKitsModel.h \
           FrequencyDelegate.h \
           FrequencyEditor.h \
           Calibration.h \
           PartialCal.h \
           TimedProgressBar.h \
           CalibrationModel.h \
           Ports.h
SOURCES += PortsPage.cpp \
           CalKitsPage.cpp \
           SetupPage.cpp \
           MeasurePage.cpp \
           PortsModel.cpp \
           PortsView.cpp \
           AvailableCalKitsModel.cpp \
           AvailableCalKitsView.cpp \
           DoubleOffsetShortKit.cpp \
           FrequencyRange.cpp \
           ChosenCalKitsModel.cpp \
           FrequencyDelegate.cpp \
           FrequencyEditor.cpp \
           Calibration.cpp \
           PartialCal.cpp \
           TimedProgressBar.cpp \
           CalibrationModel.cpp \
           Ports.cpp
FORMS   += PortsPage.ui \
           CalKitsPage.ui \
           SetupPage.ui \
           MeasurePage.ui \
           TimedProgressBar.ui
RESOURCES  += Resources.qrc

DEFINES     += SOURCE_DIR=\\\"$$PWD\\\"
CONFIG(debug, debug|release):DEFINES += DEBUG_MODE
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
