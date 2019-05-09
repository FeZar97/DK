#-------------------------------------------------
#
# Project created by QtCreator 2018-10-14T17:48:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += multimedia

TARGET = DigitalTract
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
           widget.cpp \
           stylehelper.cpp \
           config_manager.cpp \
           rpu.cpp \
           rpu_kalibrator.cpp \
           rpu_tract.cpp \
           sdr.cpp \
           dsp.cpp \
           reader.cpp \
           fft_calcer.cpp \
           filter.cpp \
           shifter.cpp \
           wav_recorder.cpp \
           demodulator.cpp \
           sounder.cpp \
           FftGraph.cpp \
           SonoGraph.cpp \
           iqcompensator.cpp \
    askr.cpp

HEADERS  += definitions.h \
            widget.h \
            stylehelper.h \
            config_manager.h \
            rpu.h \
            rpu_kalibrator.h \
            rpu_tract.h \
            rtl-sdr.h \
            rtl-sdr_export.h \
            sdr.h \
            dsp.h \
            reader.h \
            fft_calcer.h \
            filter.h \
            shifter.h \
            wav_recorder.h \
            demodulator.h \
            sounder.h \
            FftGraph.h \
            SonoGraph.h \
            iqcompensator.h \
    askr.h

FORMS    += widget.ui \
            config_manager.ui

INCLUDEPATH += ../RTL/include
LIBS += ../RTL/lib/rtlsdr.lib
LIBS += ../RTL/lib/convenience_static.lib
LIBS += ../RPU/inpoutx64.lib

include(../Qt IPP/LIBS/ipp.pri)

RESOURCES += \
    images.qrc
