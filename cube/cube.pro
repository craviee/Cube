#-------------------------------------------------
#
# Project created by QtCreator 2017-07-04T09:15:05
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += widgets
QT += serialport
TARGET = cube
TEMPLATE = app

INCLUDEPATH += /usr/include/opencv2
INCLUDEPATH += /usr/include/opencv

LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_videoio

SOURCES += main.cpp\
        mainwindow.cpp \
    squares.cpp \
    config.cpp

HEADERS  += mainwindow.h \
    config.h

FORMS    += mainwindow.ui

DISTFILES += \
    table_edge2.rht \
    table_edge1.rht \
    table_corner.rht \
    tables.md5 \
    tags \
    COPYING
