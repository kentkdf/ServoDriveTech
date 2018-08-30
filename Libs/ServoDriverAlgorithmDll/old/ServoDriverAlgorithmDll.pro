# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = lib

CONFIG(debug, debug|release){
    TARGET = ServoDriverAlgorithmDlld
    DESTDIR =$${PWD}/../../debug/Bin
} else{
    TARGET = ServoDriverAlgorithmDll
    DESTDIR =$${PWD}/../../release/Bin
}


DEFINES += _WINDOWS _USRDLL SERVODRIVERALGORITHMDLL_EXPORTS
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
