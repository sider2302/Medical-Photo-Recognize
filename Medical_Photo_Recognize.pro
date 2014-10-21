#-------------------------------------------------
#
# Project created by QtCreator 2014-10-15T13:41:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Medical_Photo_Recognize
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    maximiumminimumform.cpp \
    clustersviewwidget.cpp \
    maxmin.cpp

HEADERS  += mainwindow.h \
    maximiumminimumform.h \
    clustersviewwidget.h \
    recognizealgs.h

FORMS    += mainwindow.ui \
    maximiumminimumform.ui

RESOURCES += \
    resources.qrc
