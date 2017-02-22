#-------------------------------------------------
#
# Project created by QtCreator 2017-02-06T18:03:54
#
#-------------------------------------------------

QT += core gui
QT += opengl
QT += gui

#lQMAKE_CC = gcc-6
#QMAKE_CXX = g++-6b
#CONFIG -= c++11
QMAKE_CXXFLAGS+= -fopenmp
DEFINES += NO_OPENGL
QMAKE_LFLAGS +=  -fopenmp
#QMAKE_LFLAGS +=  -lopengl32
#QMAKE_CXXFLAGS += -m32
QMAKE_CXXFLAGS += -Ofast #-ffast-math  -march=native -mtune=native
LIBS += -LD:C:\Qt\Qt5.6.2\Tools\mingw492_32\i686-w64-mingw32\lib\ -lopengl32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Gamer
TEMPLATE = app
INCLUDEPATH += .
#CONFIG+= static
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
    source/util/buffer2d.cpp \
    dialogrendererhelp.cpp \
    source/util/fitsio.cpp \
    dialogabout.cpp \
    source/util/fitsparam.cpp \
    source/galaxy/renderqueue.cpp \
    source/consolerenderer.cpp \
    source/galaxy/rasterthread.cpp \

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
    source/util/buffer2d.h \
    dialogrendererhelp.h \
    source/util/fitsio.h \
    dialogabout.h \
    source/util/fitsparam.h \
    source/util/random.h \
    source/galaxy/renderqueue.h \
    source/consolerenderer.h \
    source/galaxy/rasterthread.h \
    source/galaxy/rasterizeromp.h

FORMS    += mainwindow.ui \
    dialogrendererhelp.ui \
    dialogabout.ui

RESOURCES += \
    gamerresources.qrc

DISTFILES +=
