#-------------------------------------------------
#
# Project created by QtCreator 2017-10-15T20:42:42
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MovieDatabase
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    vyberherce.cpp \
    vyberrezisera.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    vyberherce.h \
    vyberrezisera.h

FORMS    += mainwindow.ui \
    dialog.ui \
    vyberherce.ui \
    vyberrezisera.ui
