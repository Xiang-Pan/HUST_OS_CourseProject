#-------------------------------------------------
#
# Project created by QtCreator 2018-04-22T22:13:21
#
#-------------------------------------------------

QT += core gui
QT      +=charts
QT += widgets

CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myLinuxMonitor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#debug: LIBS+= processes.o

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    filesystems.cpp \
    processes.cpp \
    progressbardelegate.cpp \
    tableview.cpp \
    tablemodel.cpp \
    netinfo.cpp

HEADERS += \
        mainwindow.h \
    filesystems.h \
    processes.h \
    progressbardelegate.h \
    tableview.h \
    tablemodel.h \
    netinfo.h

FORMS += \
        mainwindow.ui

DISTFILES +=

RESOURCES += \
    rsc.qrc
