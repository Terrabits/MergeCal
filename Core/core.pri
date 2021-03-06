
QT          += core gui widgets

include($$PWD/../RsaToolbox/rsatoolbox.pri)
INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD
RESOURCES   += $$PWD/Resources.qrc

win32 {
    # C++11 enum class warnings:
    QMAKE_CXXFLAGS += /wd4482

    LIBS += -L$$PWD
    CONFIG(debug, release|debug) {
        LIBS += -l$$PWD/Cored
    }
    else {
        LIBS += -l$$PWD/Core
    }
}
macx {
    QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7
    LIBS += -stdlib=libc++
    LIBS += -L$$PWD
    CONFIG(debug, release|debug) {
        LIBS += $$PWD/libCored.a
    }
    else {
        LIBS += $$PWD/libCore.a
    }
}

DEFINES     += SOURCE_DIR=\\\"$$PWD\\\"
CONFIG(debug, debug|release):DEFINES += DEBUG_MODE
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
