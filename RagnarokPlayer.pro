#-------------------------------------------------
#
# Project created by QtCreator 2017-03-22T12:32:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia multimediawidgets network
QMAKE_CXXFLAGS += -std=c++0x
TARGET = RagnarokPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    anibutton.cpp \
    listwidget.cpp \
    nextstrategy.cpp \
    listoflist.cpp \
    medialist.cpp \
    mediaplayer.cpp \
    baidumusic.cpp \
    baiduwidget.cpp \
    cookiejar.cpp \
    networker.cpp

HEADERS  += mainwindow.h \
    anibutton.h \
    mediaplayer.h \
    listoflist.h \
    medialist.h \
    nextstrategy.h \
    listwidget.h \
    baidumusic.h \
    baiduwidget.h \
    cookiejar.h \
    networker.h

FORMS    += mainwindow.ui

RESOURCES += \
    image.qrc \
    xml.qrc
