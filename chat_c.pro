#-------------------------------------------------
#
# Project created by QtCreator 2013-03-25T12:56:21
#
#-------------------------------------------------

TEMPLATE = app

DEPENDPATH += .
INCLUDEPATH += .

# This gives us access to the Qt socket classes (QTcpSocket)
QT += network


TARGET = chat_c


SOURCES += main.cpp \
    MainWindow.cxx

HEADERS += \
    MainWindow.h

FORMS += \
    MainWindow.ui
