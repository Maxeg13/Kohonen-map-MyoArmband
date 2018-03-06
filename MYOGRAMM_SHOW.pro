#-------------------------------------------------
#
# Project created by QtCreator 2017-06-20T08:50:42
#
#-------------------------------------------------

QT       += core gui
CONFIG   += qwt
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS+= -LC:/QWT/lib -lqwt
INCLUDEPATH+= C:/QWT/include
INCLUDEPATH+=C:\eigen-eigen-332

#LIBS+="$$PWD/serial.o"
#LIBS+="$$PWD/stand_dev.o"
LIBS+=-L$$OUT_PWD/release -lmyo32
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
    datacollector.cpp \
    kohonenwidget.cpp \
#    serialqobj.cpp \
    drawing.cpp \
    stand_dev.cpp
#    stand_dev.cpp

HEADERS  += \
    headers.h \
    stand_dev.h \
    drawing.h \
    stand_dev.h \
#    myo/cxx/detail/ThrowOnError.hpp \
#    myo/cxx/impl/Hub_impl.hpp \
#    myo/cxx/impl/Myo_impl.hpp \
#    myo/cxx/impl/Pose_impl.hpp \
#    myo/cxx/DeviceListener.hpp \
#    myo/cxx/Hub.hpp \
#    myo/cxx/Myo.hpp \
#    myo/cxx/Pose.hpp \
#    myo/cxx/Quaternion.hpp \
#    myo/cxx/Vector3.hpp \
#    myo/libmyo/detail/visibility.h \
    kohonenwidget.h \
    mainwindow.h \
    datacollector.h \
#    serialqobj.h \
    layer_koh.h \
#    eig.h \
#    pca.h
#QMAKE_CLEAN +="$$OUT_PWD/MYOGRAMM_SHOW.EXE"
#FORMS    += mainwindow.ui

DISTFILES += \
    myo/libmyo/myo32.dll
