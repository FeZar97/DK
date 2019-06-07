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

#include "askr.h"

ASKR::ASKR(RPU *_rpu, SDR *_sdr, DSP *_dsp)
{
    testReport = "";

    rpu = _rpu;
    sdr = _sdr;
    dsp = _dsp;
}

bool ASKR::setGlobalSettings(RPU_CONFIG _config,
                             int _bpoCentralFreq, IF_BAND _bpoIfBand, int _bpoInAttIdx, int _bpoHfAttIdx, int _bpoIfAttIdx,
                             STATUS _kalibWorkStatus, int _kalibAttIdx, KALIB_TYPE _kalibSignalType, MOD_SIGNAL _kalibModSignal,
                             int _sdrCentralFreq, int _sdrDirectSampIdx, int _sdrTunerGainIdx, bool _sdrRtlAgc, int _sdrSampleRate,
                             FFT_MODE _dspFftMode, WINDOW _dspCurrentWin, float _dspDcOffset, int _dspFPS, bool _dspSoundMute)
{
    dsp->dsp_params->read_params.is_receiving = false;

/// настройки РПУ
    // перевод РПУ в четырехканальных режим
    rpu->set_config(_config);

    // настройки тракта РПУ
    rpu->first_tract.set_central_freq(_bpoCentralFreq);
    rpu->first_tract.set_if_band_idx(_bpoIfBand);
    rpu->first_tract.set_in_att_idx(_bpoInAttIdx);
    rpu->first_tract.set_hf_att_idx(_bpoHfAttIdx);
    rpu->first_tract.set_if_att_idx(_bpoIfAttIdx);

    // настройки калибратора
    rpu->kalibrator.set_work_status(_kalibWorkStatus);
    rpu->kalibrator.set_att_idx(_kalibAttIdx);
    rpu->kalibrator.set_exit_type(_kalibSignalType);
    rpu->kalibrator.set_signal_type(_kalibModSignal);

/// настройки SDR
    // если нет коннекта к SDR - выход
    if(!sdr->sdr_params->is_open) return false;

    sdr->sdr_params->central_freq = _sdrCentralFreq;
    sdr->sdr_params->direct_sampling_idx = _sdrDirectSampIdx;
    sdr->sdr_params->gain_idx = _sdrTunerGainIdx;
    sdr->sdr_params->rtl_agc = _sdrRtlAgc;
    sdr->sdr_params->sample_rate = _sdrSampleRate;

    rtlsdr_reset_buffer(sdr->sdr_params->sdr_ptr);

/// настройки ЦОС
    dsp->dsp_params->fft_params.fft_mode = _dspFftMode;
    dsp->dsp_params->fft_params.fft_current_window = _dspCurrentWin;
    dsp->dsp_params->fft_params.dc_offset_gain = _dspDcOffset;
    dsp->dsp_params->read_params.readout_per_seconds = _dspFPS;
    dsp->dsp_params->sound_params.is_mute = _dspSoundMute;

    // если при подготовке возникли ошибки - выход
    if(dsp->prepair_to_record()) return false;

    return true;
}

void ASKR::testRpu()
{
    testReport = "";
    fullReport = QDateTime::currentDateTime().toString() + "\r\n\r\n";

    rpu->first_tract.set_central_freq(14000000);
    testKalibrator();
    testPreselektors();
    testAttenuators();
    testIfBand();

    emit updateSplashInfoLabel("Проверка закончена");
    QFile file("askr report.txt");
    if(file.open(QIODevice::WriteOnly)){
        QTextStream stream(&file);
        stream << fullReport;
        file.close();
    }

    emit endAskr();
}

/* тестирование калибратора
   процесс тестирования заключается в настройке калибратора на частоту 15 МГц и измерении различных уровней аттенюации */
void ASKR::testKalibrator()
{
    if(!setGlobalSettings(FOUR_CHANNEL,
                          14000000, SECOND_BAND, ON, 2, 5,
                          ON, 10, INTERNAL, SINUS,
                          5500000, 2, 1, false, 240000,
                          READER_FFT, BLACKMANN, float(127.4), 10, true)){
        updateAskrResTextBrowser(testReport + "Не удалось сконфигурировать РПУ\n");
        return;
    }

    int i;
    float offKalibValue, onKalibValue;
    testReport += "Проверка калибратора: ";
    fullReport += "Проверка калибратора:\r\n";
    emit updateAskrResTextBrowser(testReport);
    emit updateSplashInfoLabel("Проверка калибратора");

    // начало приема
    dsp->dsp_params->read_params.is_receiving = true;
    emit globalUpdateInterface();

// выключение калибратора и измерение уровня сигнала на центральной частоте
    rpu->kalibrator.set_work_status(OFF);
    emit globalUpdateInterface();
    for(i = 0; i < 5; i++){
        dsp->reader->get_one_read_step();
        emit updateAskrProgressBar(i * 10);
    }
    offKalibValue = (dsp->dsp_params->fft_params.fft_res[0] + dsp->dsp_params->fft_params.fft_res[1023]) / 2;
    fullReport += "     Уровень сигнала на центральной частоте при выключенном калибраторе: " + QString::number(offKalibValue, 'f', 2) + " дБ;\r\n";

// включение калибратора и измерение уровня сигнала на центральной частоте
    rpu->kalibrator.set_work_status(ON);
    emit globalUpdateInterface();
    for(i = 0; i < 5; i++){
        dsp->reader->get_one_read_step();
        emit updateAskrProgressBar(50 + i * 10);
    }
    onKalibValue = (dsp->dsp_params->fft_params.fft_res[0] + dsp->dsp_params->fft_params.fft_res[1023]) / 2;
    fullReport += "     Уровень сигнала на центральной частоте при включенном калибраторе: " + QString::number(onKalibValue, 'f', 2) + " дБ;\r\n";

    // разность должна быть примерно 48 дБ
    testReport += (onKalibValue - offKalibValue > 10) ? "+;\n" : "-;\n";
    fullReport += "     Разность уровней: " + QString::number(onKalibValue - offKalibValue, 'f', 2) + ";\r\n\r\n";

    emit updateAskrResTextBrowser(testReport);
    emit updateAskrProgressBar(100);
    // конец приема
    dsp->dsp_params->read_params.is_receiving = false;
    emit globalUpdateInterface();
}

/* тестирование преселекторов
        0) выбор i-го преселектора, i = 0..9
        1) выбор j-ой центральной частоты преселетора, j = 0..9
        2) определение уровня сигнала калибратора с выбранным i-м преселектором и j-й центральной частотой
        3) если i != j, то преселектор не работает, иначе - работает

    производится перестройка на центральные частоты преселекторов и измеряется уровень калибровочного сигнала
     200  - 1000  : 600 кГц
     1000 - 1500  : 1250 кГц
     1500 - 2200  : 1850 кГц
     2200 - 3200  : 2700 кГц
     3200 - 4700  : 3800 кГц  (должно быть 3950)
     4700 - 6800  : 5250 кГц  (должно быть 5750)
     6800 - 9900  : 7350 кГц  (должно быть 8750)
     9900 - 14400 : 12150 кГц
    14400 - 20700 : 17550 кГц
    20700 - 32000 : 26350 кГц
*/
void ASKR::testPreselektors()
{
    if(!setGlobalSettings(FOUR_CHANNEL,
                          14000000, SECOND_BAND, ON, 2, 1,
                          ON, 14, INTERNAL, SINUS,
                          5500000, 2, 1, false, 240000,
                          READER_FFT, BLACKMANN, 127.4, 10, true)){
        updateAskrResTextBrowser(testReport + "Не удалось сконфигурировать РПУ\n");
        return;
    }

    QString templatePres = QString("\nПроверка преселекторов:\n")
                         + QString("   №   | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |\n");
    fullReport += "Проверка преселекторов:\r\n";

    float max;
    int i = 0, j, k, maxIdx = -1;
    const int kalibBinNb = 2; // количество центральных бинов БПФ, в которых содержится сигнал калибратора
    for(i = 0; i < RPU_NUMBER_OF_PRESELECTORS; i++) ippsZero_32f(preselektorsAvgArray[i], RPU_NUMBER_OF_PRESELECTORS);

    bool testRes[10]{false};

    QString res = "Статус |   |   |   |   |   |   |   |   |   |   |";

    emit updateAskrResTextBrowser(testReport + templatePres + res + "\n");
    emit updateAskrProgressBar(0);
    emit updateSplashInfoLabel("Проверка преселекторов");

    // начало приема
    dsp->dsp_params->read_params.is_receiving = true;
    emit globalUpdateInterface();

    // цикл по преселекторам
    for(i = 0; i < RPU_NUMBER_OF_PRESELECTORS; i++){

        emit updateAskrProgressBar(i * 10);

        fullReport += "     Преселектор " + QString::number(i) + ":\r\n";
        fullReport += "         Уровни сигналов: ";
        rpu->first_tract.send_tract_settings_to_RPU(i, preselectors_central_freqs[i]); // ручной выбор i-го преселектора

        max = -200.; // -200 dB - вряд ли будет меньше

        // цикл по центральным частотам преселекторов
        for(j = 0; j < RPU_NUMBER_OF_PRESELECTORS; j++){

            emit updateAskrProgressBar(i * 10 + j);

            // выставляется центральная частота j-го преселектора
            // калибратор должен автоматически начать генерацию колебания с этой частотой
            rpu->first_tract.send_tract_settings_to_RPU(i, preselectors_central_freqs[j]);
            emit globalUpdateInterface();

            // прием на частоте j в течении 0.4 секунд
            for(k = 0; k < 10; k++)
                dsp->reader->get_one_read_step();

            // считаем сумму kalibBinNb центральных бинов БПФ
            for(k = 0; k < kalibBinNb; k++)
                preselektorsAvgArray[i][j] += (k < kalibBinNb/2) ? dsp->dsp_params->fft_params.fft_res[k] : dsp->dsp_params->fft_params.fft_res[DSP_FFT_SIZE - (kalibBinNb - k)];

            // усредняем полученный результат
            preselektorsAvgArray[i][j] /= kalibBinNb;
            fullReport += QString::number(preselektorsAvgArray[i][j]);
            fullReport += (j == RPU_NUMBER_OF_PRESELECTORS - 1) ? ";\r\n" : ", ";
            if(preselektorsAvgArray[i][j] > max){
                max = preselektorsAvgArray[i][j];
                maxIdx = j;
            }
        }
        fullReport += "         Максимальный уровень на частоте " + QString::number(maxIdx) + ";\r\n";

        testRes[i] = (abs(maxIdx - i) <= 2);
        res = "Статус |";
        for(k = 0; k < RPU_NUMBER_OF_PRESELECTORS; k++){
            if(k <= i) res += testRes[k] ? " + |" : " - |";
            else res += "   |";
        }
        emit updateAskrResTextBrowser(testReport + templatePres + res + "\n");
    }
    fullReport += "\r\n";
    testReport += templatePres + res;
    emit updateAskrResTextBrowser(testReport);
    emit updateAskrProgressBar(100);
    // конец приема
    dsp->dsp_params->read_params.is_receiving = false;
    emit globalUpdateInterface();
}

/* тестирование аттенюаторов
   последовательное переключение аттенюаторов тракта и сравнение значений между собой
   1 проверка для входного аттенюатора
   8 проверок для ВЧ аттенюатора
   16 проверок для ПЧ аттенюатора
   всего 25 проверок, каждая по 0,4 сек, итог 25*0,4 = 10 сек
 */
void ASKR::testAttenuators()
{
    if(!setGlobalSettings(FOUR_CHANNEL,
                          14000000, SECOND_BAND, ON, 2, 1,
                          ON, 14, INTERNAL, SINUS,
                          5500000, 2, 1, false, 240000,
                          READER_FFT, BLACKMANN, float(127.4), 10, true)){
        updateAskrResTextBrowser(testReport + "Не удалось сконфигурировать РПУ\n");
        return;
    }

    int i, j, progressBarShift = 0;
    float prevFftValue, curFftValue;
    fullReport += "Проверка аттенюаторов:\r\n";
    QString report = QString("\n\nПроверка аттенюаторов:\n");
    emit updateAskrResTextBrowser(testReport + report);
    emit updateAskrProgressBar(progressBarShift);
    emit updateSplashInfoLabel("Проверка аттенюаторов");

    // начало приема
    dsp->dsp_params->read_params.is_receiving = true;
    emit globalUpdateInterface();

    bool isInAttCorrect = true;
// проверка входного аттенюатора ( два положения: выкл / 10 дБ), 1 проверка
    fullReport += "     Входной аттенюатор:\r\n";
    for(j = 0; j < 4; j++) dsp->reader->get_one_read_step();
    prevFftValue = (dsp->dsp_params->fft_params.fft_res[0] + dsp->dsp_params->fft_params.fft_res[1023]) / 2;
    fullReport += "         Уровень сигнала калибратора при включенном входном аттенюаторе: " + QString::number(prevFftValue, 'f', 2) + " дБ;\r\n";
    // выключение аттенюатора и накопление информации об уровне сигнала
    rpu->first_tract.set_in_att_idx(OFF);
    emit globalUpdateInterface();
    for(j = 0; j < 4; j++){
        dsp->reader->get_one_read_step();
        emit updateAskrProgressBar(++progressBarShift);
    }
    curFftValue = (dsp->dsp_params->fft_params.fft_res[0] + dsp->dsp_params->fft_params.fft_res[1023]) / 2;
    fullReport += "         Уровень сигнала калибратора при выключенном входном аттенюаторе: " + QString::number(curFftValue, 'f', 2) + " дБ;\r\n";
    fullReport += "         Разность урвоней: " + QString::number(curFftValue - prevFftValue, 'f', 2) + " дБ;\r\n";
    isInAttCorrect = curFftValue - prevFftValue > 7;
    report += QString("     Входной аттенюатор: ") + QString((isInAttCorrect) ? "+;\n" : "-;\n");
    emit updateAskrResTextBrowser(testReport + report);
    prevFftValue = curFftValue;

    // возвращение входного аттенюатора в исходное положение и выключение ВЧ аттенюатора
    rpu->first_tract.set_in_att_idx(ON);
    rpu->first_tract.set_hf_att_idx(0);
    for(j = 0; j < 2; j++) dsp->reader->get_one_read_step();

// проверка ВЧ аттенюатора (шаг аттенюации == 6 дБ)
    fullReport += "     Аттенюатор ВЧ:\r\n";
    bool isHfAttCorrect = true;
    // 8 итераций
    for(i = RPU_MIN_HF_ATT_IDX; i <= RPU_MAX_HF_ATT_IDX; i++){
        rpu->first_tract.set_hf_att_idx(i);
        emit globalUpdateInterface();
        for(j = 0; j < 4; j++){
            dsp->reader->get_one_read_step();
            emit updateAskrProgressBar(++progressBarShift);
        }
        curFftValue = (dsp->dsp_params->fft_params.fft_res[0] + dsp->dsp_params->fft_params.fft_res[1023]) / 2;
        fullReport += "         Уровень сигнала калибратора при ВЧ аттенюации "
                   + QString::number(i * RPU_HF_ATT_STEP) + " дБ: "
                   + QString::number(curFftValue, 'f', 2) + " дБ;\r\n";

        if(i > 1 && (prevFftValue - curFftValue) < 3)
            isHfAttCorrect = false;
        prevFftValue = curFftValue;
    }
    report += QString("          Аттенюатор ВЧ: ") + QString((isHfAttCorrect) ? "+;\n" : "-;\n");
    emit updateAskrResTextBrowser(testReport + report);

    // возвращение аттенюатора ВЧ на значение 12 дБ и выключение аттенюатора ПЧ
    rpu->first_tract.set_hf_att_idx(2);
    rpu->first_tract.set_if_att_idx(0);
    for(j = 0; j < 2; j++) dsp->reader->get_one_read_step();

    bool isIfAttCorrect = true;
// проверка ПЧ аттенюатора (шаг аттенюации == 2 дБ)
    fullReport += "     Аттенюатор ПЧ:\r\n";
    // 16 итераций
    for(i = RPU_MIN_IF_ATT_IDX; i <= RPU_MAX_IF_ATT_IDX; i++){
        rpu->first_tract.set_if_att_idx(i);
        emit globalUpdateInterface();
        for(j = 0; j < 4; j++){
            dsp->reader->get_one_read_step();
            emit updateAskrProgressBar(++progressBarShift);
        }
        curFftValue = (dsp->dsp_params->fft_params.fft_res[0] + dsp->dsp_params->fft_params.fft_res[1023]) / 2;

        fullReport += "         Уровень сигнала калибратора при ПЧ аттенюации "
                   + QString::number(i * RPU_IF_ATT_STEP) + " дБ: "
                   + QString::number(curFftValue, 'f', 2) + " дБ;\r\n";

        if(i > 1 && (prevFftValue - curFftValue) < 1)
            isIfAttCorrect = false;

        prevFftValue = curFftValue;
    }
    report += QString("          Аттенюатор ПЧ: ") + QString((isIfAttCorrect) ? "+;\n" : "-;\n");
    emit updateAskrResTextBrowser(testReport + report);

    // возвращение на значение 6 дБ (1 индекс)
    rpu->first_tract.set_if_att_idx(1);
    fullReport += "\r\n";

    testReport += report;
    emit updateAskrResTextBrowser(testReport);
    emit updateAskrProgressBar(100);
    // конец приема
    dsp->dsp_params->read_params.is_receiving = false;
    emit globalUpdateInterface();
}

/* тестирование работоспособности фильтров ПЧ
   последовательно перебираются фильтры ПЧ и сравниваются уровни шумов в соответсвующих диапазонах частот
 */
void ASKR::testIfBand()
{
    if(!setGlobalSettings(FOUR_CHANNEL,
                          14000000, FIRST_BAND, ON, 2, 1,
                          OFF, 14, INTERNAL, SINUS,
                          5500000, 2, 1, false, 240000,
                          READER_FFT, BLACKMANN, float(127.4), 10, true)){
        updateAskrResTextBrowser("Не удалось сконфигурировать РПУ\n");
        return;
    }

    int i, j, progressBarShift = 0, testBins[] = {13, 28, 298};
    float prevFftValue, curFftValue;
    fullReport += "Проверка фильтров ПЧ:\r\n";
    QString report = QString("\nПроверка фильтров ПЧ: ");
    emit updateAskrResTextBrowser(testReport + report);
    emit updateSplashInfoLabel("Проверка фильтров ПЧ");

    // начало приема
    dsp->dsp_params->read_params.is_receiving = true;
    emit globalUpdateInterface();

    for(j = 0; j < 5; j++){
        dsp->reader->get_one_read_step();
        emit updateAskrProgressBar(progressBarShift+=5);
    }

    bool isFiltersCorrect = true;
    for(i = SECOND_BAND; i <= FOURTH_BAND; i++){
        prevFftValue = dsp->dsp_params->fft_params.fft_res[testBins[i - 1]];
        rpu->first_tract.set_if_band_idx(i);
        emit globalUpdateInterface();
        for(j = 0; j < 5; j++){
            dsp->reader->get_one_read_step();
            emit updateAskrProgressBar(progressBarShift+=5);
        }
        curFftValue = dsp->dsp_params->fft_params.fft_res[testBins[i - 1]];
        fullReport += "     Изменение уровня сигнала при включении фильтра №" + QString::number(i) + ": "
                   + QString::number(curFftValue - prevFftValue, 'f', 2) + " дБ;\r\n";
        if(curFftValue - prevFftValue < 3)
            isFiltersCorrect = false;
    }
    report += isFiltersCorrect ? "+;\n" : "-;\n";
    testReport += report;
    emit updateAskrResTextBrowser(testReport);
    emit updateAskrProgressBar(100);
    // конец приема
    dsp->dsp_params->read_params.is_receiving = false;
    emit globalUpdateInterface();
}
