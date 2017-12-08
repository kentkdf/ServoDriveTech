#-------------------------------------------------
#
# Project created by QtCreator 2017-12-08T11:42:48
#
#-------------------------------------------------
include(../App.pri)
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TEMPLATE = app

CONFIG(debug, debug|release){
  TARGET = AppUiTestd
} else{
  TARGET = AppUiTest
}

SOURCES += main.cpp\
        uimainwindow.cpp \
    ../../Common/src/Kernel/axistreemap.cpp \
    ../../Common/src/Kernel/devcomrwriter.cpp \
    ../../Common/src/Kernel/deviceconfig.cpp \
    ../../Common/src/Kernel/devtextrwriter.cpp \
    ../../Common/src/Kernel/gconfigreadwriter.cpp \
    ../../Common/src/Kernel/globalconfig.cpp \
    ../../Common/src/Kernel/ictrboard.cpp \
    ../../Common/src/Kernel/idevreadwriter.cpp \
    ../../Common/src/Kernel/idspmap.cpp \
    ../../Common/src/Kernel/ipwrboard.cpp \
    ../../Common/src/Kernel/iuicontroler.cpp \
    ../../Common/src/Kernel/linksocket.cpp \
    ../../Common/src/Kernel/sdassembly.cpp \
    ../../Common/src/Kernel/sevctrboard.cpp \
    ../../Common/src/Kernel/sevdevice.cpp \
    ../../Common/src/Kernel/sevdspmap.cpp \
    ../../Common/src/Kernel/sevpwrboard.cpp \
    ../../Common/src/Kernel/sevuicontroler.cpp \
    ../../Common/src/Kernel/PowerTreeManage/powertreemanage.cpp \
    ../../Common/src/UiFactory/uifactory.cpp

HEADERS  += uimainwindow.h \
    ../../Common/src/sdtglobaldef.h \
    ../../Common/src/Kernel/axistreemap.h \
    ../../Common/src/Kernel/devcomrwriter.h \
    ../../Common/src/Kernel/deviceconfig.h \
    ../../Common/src/Kernel/devtextrwriter.h \
    ../../Common/src/Kernel/gconfigreadwriter.h \
    ../../Common/src/Kernel/globalconfig.h \
    ../../Common/src/Kernel/ictrboard.h \
    ../../Common/src/Kernel/idevreadwriter.h \
    ../../Common/src/Kernel/idspmap.h \
    ../../Common/src/Kernel/ipwrboard.h \
    ../../Common/src/Kernel/iuicontroler.h \
    ../../Common/src/Kernel/linksocket.h \
    ../../Common/src/Kernel/sdassembly.h \
    ../../Common/src/Kernel/sevctrboard.h \
    ../../Common/src/Kernel/sevdevice.h \
    ../../Common/src/Kernel/sevdeviceprivate_p.h \
    ../../Common/src/Kernel/sevdspmap.h \
    ../../Common/src/Kernel/sevpwrboard.h \
    ../../Common/src/Kernel/sevuicontroler.h \
    ../../Common/src/Kernel/PowerTreeManage/powertreemanage.h \
    ../../Common/src/UiFactory/uifactory.h \
    ../../Common/src/UiFactory/registerfunction.h

FORMS    += uimainwindow.ui
