#-------------------------------------------------
#
# Project created by QtCreator 2018-04-19T15:27:53
#
#-------------------------------------------------
include (../ctkplugins.pri)
QT       -= gui
QT+=widgets


TEMPLATE = lib

CONFIG(debug,debug|release){
  TARGET = CurveUsrVel
}else{
  TARGET = CurveUsrVel
}
DESTDIR=$$PLOT_PLUGINS_PATH/user

SOURCES += curveusrvel.cpp \
    curveusrvelactivator.cpp

HEADERS += curveusrvel.h \
    curveusrvelactivator.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    resource.qrc
