#-------------------------------------------------
#
# Project created by QtCreator 2015-07-09T21:11:50
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = game_2048
TEMPLATE = app


SOURCES += main.cpp\
        board_2048.cpp \
    main_form.cpp

HEADERS  += board_2048.h \
    main_form.h

FORMS    += board_2048.ui \
    main_form.ui
