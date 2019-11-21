SOURCES += main.cpp 

SOURCES += QtHttpServer.cpp

          

HEADERS += utils.h

HEADERS += QtHttpServer.h

TARGET = main

CONFIG += debug

CONFIG += console

QT += network

QT += widgets

LIBS += libwsock32 libws2_32
