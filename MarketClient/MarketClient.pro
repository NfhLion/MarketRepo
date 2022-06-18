#-------------------------------------------------
#
# Project created by QtCreator 2020-04-16T08:50:34
#
#-------------------------------------------------

QT       += core gui network sql

CONFIG += C++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MarketClient
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += QT_DLL QWT_DLL
LIBS += -L"E:\QT5.9.6\5.9.6\mingw53_32\lib" -lqwtd
#LIBS += -L"E:\QT5.9.6\5.9.6\mingw53_32\lib" -lqwt
INCLUDEPATH += E:\QT5.9.6\5.9.6\mingw53_32\include\QWT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
		config.cpp \
    globalvariable.cpp

HEADERS += \
    config.h \
    globalvariable.h

INCLUDEPATH += $$PWD/include
include(UI_main/UI_main.pri)
include(UI_Widget/UI_Widget.pri)
include(UI_item/UI_item.pri)
include(BLL_logic/BLL_logic.pri)
include(BLL_network/BLL_network.pri)
include(DAL_database/DAL_database.pri)
include(ChatMain/ChatMain.pri)

RESOURCES += \
    image.qrc \
    ico.qrc

DISTFILES +=



