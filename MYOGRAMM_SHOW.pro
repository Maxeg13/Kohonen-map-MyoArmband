#-------------------------------------------------
#
# Project created by QtCreator 2017-06-20T08:50:42
#
#-------------------------------------------------

QT       += core gui
CONFIG   += qwt
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#LIBS += -L"C:\Users\123\Documents\MYOGRAMM_SHOW" -lab
#LIBS +="$$PWD/stand_dev11.dll"
#INCLUDEPATH += "$$PWD"
#LIBS += "$$OUT_PWD/release/stand_dev.dll"
#LIBS+="C:\Users\123\Documents\MYOGRAMM_SHOW\serial.dll"
LIBS+="$$PWD/serial.o"
LIBS+="$$PWD/stand_dev.o"
INCLUDEPATH += "$$PWD"

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
DEL_FILE += "$$OUT_PWD/release/MYOGRAMM_SHOW.EXE"

SOURCES += main.cpp\
        mainwindow.cpp \
    stdafx.cpp \
    layer_koh.cpp \
    kohonenwidget.cpp \
#    stand_dev.cpp

HEADERS  += mainwindow.h \
    headers.h \
    serial.h \
    stdafx.h \
    targetver.h \
    stand_dev.h \
    drawing.h \
    layer_koh.h \
    kohonenwidget.h \
    stand_dev.h \
    serialqobj.h
#QMAKE_CLEAN +="$$OUT_PWD/MYOGRAMM_SHOW.EXE"
#FORMS    += mainwindow.ui
