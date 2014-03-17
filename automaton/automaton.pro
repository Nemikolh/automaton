#-------------------------------------------------
#
# Project created by QtCreator 2014-03-14T11:55:36
#
#-------------------------------------------------

include(qt/ui.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += /usr/include/lua5.2
INCLUDEPATH += /home/nemikolh/git/luabind/

LIBS +=  -L"/home/nemikolh/git/luabind/src" -lluabind09 -llua5.2 -Wl,-rpath=.

TARGET = automaton
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -isystem /usr/local/boost_1_53_0/

HEADERS += core/CellularAutomaton.hpp

SOURCES += main.cpp \
        core/CellularAutomaton.cpp

RESOURCES += \
    resources.qrc
