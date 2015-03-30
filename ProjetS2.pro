#-------------------------------------------------
#
# Project created by QtCreator 2015-02-26T10:30:31
#
#-------------------------------------------------

QT       += core gui
QT       += svg
QT       += multimedia
QT       += websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjetS2
TEMPLATE = app

SOURCES += main.cpp\
    mainview.cpp \
    mainscene.cpp \
    layoutManager.cpp \
    cardItem.cpp \
    effects.cpp \
    settingsManager.cpp

HEADERS  += \
    mainview.h \
    mainscene.h \
    layoutManager.h \
    cardItem.h \
    effects.h \
    settingsManager.h

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    resources.qrc

CONFIG += c++11
