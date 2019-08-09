#-------------------------------------------------
#
# Project created by QtCreator 2019-07-23T15:53:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -ldatastor_utils -lboost_date_time -lboost_system -lboost_thread -lpthread -lboost_filesystem
INCLUDEPATH += /usr/local/include

TARGET = release_tool
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


SOURCES += \
        main.cpp \
        main_from.cpp \
    release_op.cpp \
    deploy_op.cpp \
    project.cpp \
    log_collect.cpp \
    switch_op.cpp \
    time_counter.cpp

HEADERS += \
        main_from.h \
    release_op.h \
    deploy_op.h \
    project.h \
    log_collect.h \
    switch_op.h \
    time_counter.h

FORMS += \
        main_from.ui \
    release_op.ui \
    deploy_op.ui \
    project.ui \
    log_collect.ui \
    switch_op.ui \
    time_counter.ui
