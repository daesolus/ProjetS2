#-------------------------------------------------
#
# Project created by QtCreator 2015-02-26T10:30:31
#
#-------------------------------------------------

QT       += core gui
QT       += svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjetS2
TEMPLATE = vcapp


SOURCES += main.cpp\
    mainview.cpp \
    mainscene.cpp \
    layoutManager.cpp \
    easywsclient.cpp \
    cardItem.cpp \
    effects.cpp \
    settingsManager.cpp

HEADERS  += \
    mainview.h \
    mainscene.h \
    layoutManager.h \
    easywsclient.hpp \
    cardItem.h \
    effects.h \
    settingsManager.h \
    CommunicationFPGA.h

FORMS    += mainwindow.ui

DISTFILES +=
INCLUDEPATH += libs
RESOURCES += \
    resources.qrc
CONFIG += c++11
CONFIG(debug, debug|release){
    LIBS	+= libs\CommunicationFPGA.lib
}
