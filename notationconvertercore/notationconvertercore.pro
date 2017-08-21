QT += core
QT -= gui

CONFIG += c++11

TARGET = NotationConverterCore
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
        notationconvertercore.cpp

HEADERS += \
        notationconvertercore.h
