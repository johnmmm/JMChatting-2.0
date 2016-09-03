QT       += core gui xml network multimedia

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = John_mmm_Chatting_s

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    server.cpp \
    thread.cpp

HEADERS += \
    mainwindow.h \
    server.h \
    thread.h

FORMS += \
    mainwindow.ui
