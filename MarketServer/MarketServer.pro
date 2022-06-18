TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c

LIBS += -lpthread
LIBS += -lm
LIBS += -lmysqlclient

INCLUDEPATH += $$PWD/include
include(BLL_network/BLL_network.pri)
include(reactor/reactor.pri)
include(DAL_database/DAL_database.pri)
