TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
QT += core

DEFINES += ZMQ_STATIC
LIBS += -L$$PWD/zeroMQ/lib -lzmq -lws2_32 -lIphlpapi -L$$PWD/zeroMQ/bin
INCLUDEPATH += $$PWD/zeroMQ/include

SOURCES += main.cpp
