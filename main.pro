SOURCES += main.cpp 

SOURCES += QtHttpServer.cpp

SOURCES += socketthread.cpp

          

HEADERS += utils.h

HEADERS += QtHttpServer.h

HEADERS += socketthread.h

TARGET = main

CONFIG += debug

CONFIG += console

QT += network

QT += widgets

LIBS += libwsock32 libws2_32
