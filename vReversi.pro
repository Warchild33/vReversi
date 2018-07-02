#-------------------------------------------------
#
# Project created by QtCreator 2015-11-11T18:34:24
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS+=-std=c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vReversi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gametree.cpp \
    gtreerenderer.cpp \
    gamewindow.cpp \
    board.cpp

HEADERS  += mainwindow.h \
    gametree.h \
    gtreerenderer.h \
    gamewindow.h \
    board.h

FORMS    += mainwindow.ui \
    gamewindow.ui
