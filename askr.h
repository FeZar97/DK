/*
    This file is part of DigitalKalmar(Кальмар-SDR)

    DigitalKalmar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DigitalKalmar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DigitalKalmar.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef ASKR_H
#define ASKR_H

#include <definitions.h>
#include <rpu.h>
#include <sdr.h>
#include <dsp.h>

/*
    Автоматическая система контроля работоспособности (АСКР) обеспечивает диагностику технического состояния РПУ при старте программы и во время работы.
    Процедура тестирования проводится в 5 этапов:
        «Тест калибратора»,
        «Тест чувствительности»,
        «Тест аттенюатора преселектора»,
        «Тест аттенюатора ВЧ»,
        «Тест аттенюатора ПЧ».
    Измерения проводятся по каждому из каналов РПУ. */

class ASKR : public QObject
{
    Q_OBJECT

    QString testReport;
    QString fullReport;
    RPU     *rpu;
    SDR     *sdr;
    DSP     *dsp;

    float   preselektorsAvgArray[RPU_NUMBER_OF_PRESELECTORS][RPU_NUMBER_OF_PRESELECTORS];

    bool setGlobalSettings(RPU_CONFIG _config,
                           int _bpoCentralFreq, IF_BAND _bpoIfBand, int _bpoInAttIdx, int _bpoHfAttIdx, int _bpoIfAttIdx,
                           STATUS _kalibWorkStatus, int _kalibAttIdx, KALIB_TYPE _kalibSignalType, MOD_SIGNAL _kalibModSignal,
                           int _sdrCentralFreq, int _sdrDirectSampIdx, int _sdrTunerGainIdx, bool _sdrRtlAgc, int _sdrSampleRate,
                           FFT_MODE _dspFftMode, WINDOW _dspCurrentWin, float _dspDcOffset, int _dspFPS, bool _dspSoundMute);

public slots:
    void testRpu();

public:
    ASKR(RPU *_rpu, SDR *_sdr, DSP *_dsp);

    void testKalibrator();
    void testPreselektors();
    void testAttenuators();
    void testIfBand();

signals:
    void updateAskrProgressBar(int progress);
    void updateAskrResTextBrowser(QString additionalText);
    void updateSplashInfoLabel(QString _text);
    void globalUpdateInterface();
    void endAskr();
};

#endif // ASKR_H
