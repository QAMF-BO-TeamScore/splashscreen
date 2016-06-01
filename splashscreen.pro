TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

linux {
    LIBS += -lcrypto -lssl -lboost_system -lcpprest -lpthread -lsfml-graphics -lsfml-window -lsfml-system
}

SOURCES += main.cpp \
    splashscreenmessage.cpp

HEADERS += \
    splashscreenmessage.h \
    colorconverter.h
