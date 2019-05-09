#include "askr.h"

ASKR::ASKR(RPU *_rpu, SDR *_sdr, DSP *_dsp)
{
    testReport = "";

    rpu = _rpu;
    sdr = _sdr;
    dsp = _dsp;
}

void ASKR::kalibratorTest()
{
    // массив средних значений уровня калибратора при различных преселекторах
    // при выбранной центральной частоте X максимальное значение должна иметь ячейка X
    float avg_measurments[RPU_NUMBER_OF_PRESELECTORS], max;
    int i, j, k, maxIdx = -1;
    QString test_res;

    // количество центральных бинов БПФ, в которых содержится сигнал калибратора
    const int kalibBinNb = 4;

    dsp->dsp_params->read_params.is_receiving = true;

    // для каждого из десяти преселекторов осуществляется заполнение одного КБ чтения
    for(i = 0; i < RPU_NUMBER_OF_PRESELECTORS; i++){
        test_res = "   Testing freq №" + QString::number(i) + " (" + QString::number(preselectors_central_freqs[i]) + "): ";
        max = -200.; // -200 dB - вряд ли будет меньше

        // выставляется центральная частота преселектора
        // калибратор должен автоматически начать генерацию колебания с этой частотой
        rpu->first_tract.set_central_freq(preselectors_central_freqs[i]);

        // обнуляем массив средних значений
        ippsZero_32f(avg_measurments, RPU_NUMBER_OF_PRESELECTORS);

        for(j = 0; j < RPU_NUMBER_OF_PRESELECTORS; j++){
            // ручной выбор j-го преселектора
            rpu->first_tract.send_tract_settings_to_RPU(j, preselectors_central_freqs[i]);

            // холостой прием в течении половины секунды
            for(int c = 0; c < 5; c++) dsp->reader->get_one_read_step();

            dsp->reader->get_one_read_step();// - прогонка одного цикла приема

            // считаем сумму kalibBinNb центральных бинов БПФ
            for(k = 0; k < kalibBinNb; k++){
                // kalibBinNb/2 бинов БПФ, справа от центральной частоты
                if(k < kalibBinNb/2) avg_measurments[j] += dsp->dsp_params->fft_params.fft_res[k];
                // kalibBinNb/2 бинов БПФ, слева от центральной частоты
                else avg_measurments[j] += dsp->dsp_params->fft_params.fft_res[DSP_FFT_SIZE - (kalibBinNb - k)];
            }

            // усредняем полученный результат
            avg_measurments[j] /= kalibBinNb;
            test_res += QString::number(double(avg_measurments[j]), 'g', 2) + ' ';
            if(avg_measurments[j] > max){
                max = avg_measurments[j];
                maxIdx = j;
            }
        }

        qDebug() << test_res;
        qDebug() << "max freq is " << maxIdx;
    }

    dsp->dsp_params->read_params.is_receiving = false;
}

void ASKR::tractTest()
{

}
