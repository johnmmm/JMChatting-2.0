QT       += core gui xml network multimedia

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = John_mmmChatting_client

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    client.cpp \
    thread.cpp

HEADERS += \
    mainwindow.h \
    client.h \
    thread.h

FORMS += \
    mainwindow.ui
