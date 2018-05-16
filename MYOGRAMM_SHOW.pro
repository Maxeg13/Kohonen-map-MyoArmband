#-------------------------------------------------
#
# Project created by QtCreator 2017-06-20T08:50:42
#
#-------------------------------------------------
INCLUDEPATH += C:/eigen-eigen-332
INCLUDEPATH += C:/QWT/include
LIBS += -LC:/QWT/lib -lqwt

QT       += core gui
QT += network widgets
CONFIG   += qwt
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



#LIBS+="$$PWD/serial.o"
#LIBS+="$$PWD/stand_dev.o"
LIBS+= -L$$OUT_PWD/release -lmyo32
#LIBS     += $$OUT_PWD/release/datacollector.o
#LIBS     += $$OUT_PWD/release/serialqobj.o
LIBS     += $$OUT_PWD/release/stdafx.o
#LIBS     += $$OUT_PWD/release/kohonenwidget.o
LIBS     += $$OUT_PWD/release/pca.o
#LIBS     += $$OUT_PWD/release/eig.o
LIBS     += -L$$OUT_PWD/release -leig

TARGET = MYOGRAMM_SHOW
TEMPLATE = app
QMAKE_RPATHDIR *= "$$PWD"
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#DEL_FILE += "$$OUT_PWD/release/MYOGRAMM_SHOW.EXE"

SOURCES += main.cpp\
    mainwindow.cpp \
    layer_koh.cpp \
    drawing.cpp \
    stand_dev.cpp \
    layer.cpp \
    perceptron.cpp \
    serial.cpp \
    receiver.cpp \
    histogramm.cpp
#    stand_dev.cpp

HEADERS  += \
    headers.h \
    stand_dev.h \
    drawing.h \
    stand_dev.h \
    mainwindow.h \
    layer_koh.h \
    eig.h \
    pca.h \
    layer.h \
    drawing_perc.h \
    perceptron.h \
    drawing.h \
    drawing_perc.h \
    eig.h \
    headers.h \
    layer.h \
    layer_koh.h \
    mainwindow.h \
    pca.h \
    perceptron.h \
    serial.h \
    stand_dev.h \
    stdafx.h \
    targetver.h \
    receiver.h \
    histogramm.h
#QMAKE_CLEAN +="$$OUT_PWD/MYOGRAMM_SHOW.EXE"
#FORMS    += mainwindow.ui

#DISTFILES += \
#    myo/libmyo/myo32.dll
