# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

include(../Libs.pri)

TEMPLATE = lib

CONFIG (debug, debug|release) {
    TARGET = ServoDriverAlgorithmDlld
    DESTDIR = $${APP_BUILD_PATH}/debug/bin
} else{
    TARGET = ServoDriverAlgorithmDll
    DESTDIR = $${APP_BUILD_PATH}/release/bin
}

DEFINES += _WINDOWS _USRDLL SERVODRIVERALGORITHMDLL_EXPORTS QT_DLL
INCLUDEPATH += ./FFT/include \
    . \
    ./..
LIBS += -L"."
PRECOMPILED_HEADER = stdafx.h
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(ServoDriverAlgorithmDll.pri)