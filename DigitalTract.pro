#-------------------------------------------------
#
# Project created by QtCreator 2018-10-14T17:48:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DigitalTract
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    config_manager.cpp \
    dsp.cpp \
    fft_calcer.cpp \
    fft_shifter.cpp \
    mrfiltering.cpp \
    reader.cpp \
    rpu.cpp \
    rpu_kalibrator.cpp \
    rpu_tract.cpp \
    sdr.cpp \
    simplegraph.cpp \
    stylehelper.cpp \
    wav_recorder.cpp \
    sound_maker.cpp

HEADERS  += widget.h \
    config_manager.h \
    definitions.h \
    dsp.h \
    fft_calcer.h \
    fft_shifter.h \
    mrfiltering.h \
    reader.h \
    rpu.h \
    rpu_kalibrator.h \
    rpu_tract.h \
    rtl-sdr.h \
    rtl-sdr_export.h \
    sdr.h \
    simplegraph.h \
    stylehelper.h \
    wav_recorder.h \
    sound_maker.h

FORMS    += widget.ui \
    config_manager.ui

INCLUDEPATH += ../RTL/include
LIBS += ../RTL/lib/rtlsdr.lib
LIBS += ../RTL/lib/convenience_static.lib
LIBS += ../RPU/inpoutx64.lib

include(../Qt IPP/LIBS/ipp.pri)

RESOURCES += \
    images.qrc

DISTFILES +=
