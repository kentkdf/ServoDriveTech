#-------------------------------------------------
#
# Project created by QtCreator 2017-12-01T15:45:35
#
#-------------------------------------------------

QT       += core gui designer network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

INCLUDEPATH += $${PWD}/ControlServo\
               $${PWD}/Flash\
               $${PWD}/JsonReader\
               $${PWD}/TreeManage\
               $${PWD}/Eprom\
               $${PWD}/TcpConnect\
               $${PWD}/../../Libs/Com/ServoDriverComDll/DllCom\
               $${PWD}/../../Libs/Com/ServoDriverComDll/NetCom/include\
               $${PWD}/../../Libs/GTUtils\
               $${PWD}/../../Libs/Option\

CONFIG(debug, debug|release){
    TARGET = EpromManager_d
    LIB_PATH = $${PWD}/../../build/debug/Bin
    OUT_ROOT = $${PWD}/../../build/debug
    LIBS += $${LIB_PATH}/ServoDriverComDlld.lib\
            $${LIB_PATH}/GTUtilsd.lib\
            $${LIB_PATH}/Optiond.lib
} else{
    TARGET = EpromManager
    LIB_PATH = $${PWD}/../../build/release/Bin
    OUT_ROOT = $${PWD}/../../build/release
    LIBS += $${LIB_PATH}/ServoDriverComDll.lib\
            $${LIB_PATH}/GTUtils.lib\
            $${LIB_PATH}/Option.lib
}

DESTDIR =$${OUT_ROOT}/Bin

SOURCES += main.cpp\
        eprommanager.cpp \
    ControlServo/controlservo.cpp \
    Flash/flashclass.cpp \
    JsonReader/jsonreader.cpp \
    TreeManage/treemanager.cpp \
    Eprom/eprom.cpp \
    Eprom/eprom_control.cpp \
    Eprom/eprom_power.cpp \
    globaldefine.cpp \
    TcpConnect/tcpconnect.cpp

HEADERS  += eprommanager.h \
    ControlServo/controlservo.h \
    Flash/flashclass.h \
    JsonReader/jsonreader.h \
    TreeManage/treemanager.h \
    globaldefine.h \
    Eprom/eprom.h \
    Eprom/eprom_control.h \
    Eprom/eprom_power.h \
    TcpConnect/tcpconnect.h

FORMS    += eprommanager.ui

RC_ICONS = Factory.ico

TRANSLATIONS    += ch_eeprom.ts en_eeprom.ts
