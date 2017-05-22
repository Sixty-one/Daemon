#-------------------------------------------------
#
# Project created by QtCreator 2017-05-17T10:07:28
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = serverDaemon
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../liblog/ \
               ../libtool \
               ../libencrypt \
LIBS += -L../lib -lliblog -llibtool -llibencrypt


SOURCES += main.cpp \
    delegateManager.cpp \
    messageserver.cpp \
    taskprocess.cpp \
    serverapp.cpp

HEADERS += \
    delegateHandler.h \
    delegateManager.h \
    msgServer.h \
    serviceMessage.h \
    messageserver.h \
    taskprocess.h \
    msgtype.h \
    serverapp.h
