#-------------------------------------------------
#
# Project created by QtCreator 2017-02-06T18:03:54
#
#-------------------------------------------------

QT += core gui
QT += opengl
QT += gui

#lQMAKE_CC = gcc-6
#QMAKE_CXX = g++-6
QMAKE_CXXFLAGS+= -fopenmp -O2
QMAKE_LFLAGS +=  -fopenmp
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Gamer
TEMPLATE = app


SOURCES += \
    source/galaxy/rasterizer.cpp \
    source/galaxy/galaxy.cpp \
    source/galaxy/galaxycomponents.cpp \
    source/util/util.cpp \
    source/noise/noise.cpp \
    source/noise/perlin.cpp \
    source/noise/simplex.cpp \
    source/galaxy/renderingparams.cpp \
    source/galaxy/gamercamera.cpp \
    source/galaxy/galaxyinstance.cpp \
    source/galaxy/rasterpixel.cpp \
    source/galaxy/galaxyparams.cpp \
    source/galaxy/galaxycomponent.cpp \
    source/noise/simplexnoise.cpp \
    source/galaxy/componentparams.cpp \
    source/galaxy/spectrum.cpp \
    source/glwidget.cpp \
    source/main.cpp \
    source/mainwindow.cpp \
    source/util/gmessages.cpp \
    source/noise/iqnoise.cpp \
    source/util/buffer2d.cpp

HEADERS  += \
    source/galaxy/rasterizer.h \
    source/galaxy/galaxy.h \
    source/galaxy/galaxycomponents.h \
    source/util/util.h \
    source/noise/noise.h \
    source/noise/perlin.h \
    source/noise/simplex.h \
    source/galaxy/renderingparams.h \
    source/galaxy/gamercamera.h \
    source/galaxy/galaxyparams.h \
    source/galaxy/componentparams.h \
    source/galaxy/spectrum.h \
    source/galaxy/galaxyinstance.h \
    source/rasterpixel.h \
    source/galaxy/rasterpixel.h \
    source/galaxy/galaxycomponent.h \
    source/noise/simplexnoise.h \
    source/glwidget.h \
    source/mainwindow.h \
    source/util/gmessages.h \
    source/noise/iqnoise.h \
    source/util/buffer2d.h

FORMS    += mainwindow.ui

RESOURCES += \
    gamerresources.qrc
