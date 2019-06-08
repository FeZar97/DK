#  This file is part of DigitalKalmar(Кальмар-SDR)
#
#  DigitalKalmar is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  DigitalKalmar is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with DigitalKalmar.  If not, see <https://www.gnu.org/licenses/>.

#-------------------------------------------------
# Project created by FeZaR97
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 2.19.5.27

QMAKE_TARGET_COMPANY     = FeZaR97
QMAKE_TARGET_PRODUCT     = Kalmar-SDR
QMAKE_TARGET_DESCRIPTION = Kalmar-SDR
QMAKE_TARGET_COPYRIGHT   = FeZaR97

QT += multimedia
qtHaveModule(multimedia): QT += multimedia

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
    askr.cpp \
    splashscreen.cpp \
    ConstellationDiagram.cpp

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
    askr.h \
    splashscreen.h \
    ConstellationDiagram.h

FORMS    += widget.ui \
            config_manager.ui \
            splashscreen.ui \
    ConstellationDiagram.ui

INCLUDEPATH += ../RTL/include
LIBS += ../RTL/lib/rtlsdr.lib
LIBS += ../RTL/lib/convenience_static.lib
LIBS += ../RPU/inpoutx64.lib

include(../Qt IPP/LIBS/ipp.pri)

RESOURCES +=  images.qrc

win32: RC_ICONS = $$PWD/buttons/SquidLow.ico

DISTFILES += DigitalTract_resource.rc
