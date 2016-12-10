#-------------------------------------------------
#
# Project created by QtCreator 2016-11-21T12:06:47
#
#-------------------------------------------------

QT       += widgets network



TARGET = ChattyBird
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp

HEADERS  += mainwindow.h \
    client.h \
    codes.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc
