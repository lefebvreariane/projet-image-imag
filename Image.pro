#-------------------------------------------------
#
# Project created by QtCreator 2011-01-05T15:36:49
#
#-------------------------------------------------

QT       += core gui

TARGET = Image
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    zonedessin.cpp \
    labelimage.cpp \
    controleur.cpp \
    fenetrepipette.cpp \
    histogramme.cpp

HEADERS  += mainwindow.h \
    zonedessin.h \
    labelimage.h \
    controleur.h \
    fenetrepipette.h \
    histogramme.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    save.png \
    open.png

RESOURCES += \
    images/icones.qrc \
    images/images.qrc
