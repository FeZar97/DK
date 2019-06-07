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

#include "sdr.h"

SDR::SDR()
{
    sdr_params = new SDR_params();
}

SDR::~SDR()
{
    // если приемник подключен - отключаем его
    if(sdr_params->is_open)
        rtlsdr_close(sdr_params->sdr_ptr);

    delete sdr_params;
}

// возвращает кол-во доступных приемников
int SDR::get_sdr_count()
{
    return int(rtlsdr_get_device_count());
}

// смена частоты настройки
void SDR::set_central_freq(int new_central_freq)
{
    if( (new_central_freq > SDR_MIN_TUNER_FREQ) && (new_central_freq < SDR_MAX_TUNER_FREQ) )
    {
        sdr_params->central_freq = new_central_freq;
        rtlsdr_set_center_freq(sdr_params->sdr_ptr, uint(sdr_params->central_freq));
    }

    emit update_interface();
}

/*!
 *                   rtlsdr_set_tuner_gain_mode
 *
 * Set the gain mode (automatic/manual) for the device.
 * Manual gain mode must be enabled for the gain setter function to work.
 *
 * \param dev the device handle given by rtlsdr_open()
 * \param manual gain mode, 1 means manual gain mode shall be enabled.
 * \return 0 on success
 */
// смена КУ
void SDR::set_gain_idx(int new_gain_idx)
{
    sdr_params->gain_idx = new_gain_idx;

    if(!sdr_params->gain_idx){
        rtlsdr_set_tuner_gain_mode(sdr_params->sdr_ptr, 0);
    }else{
        rtlsdr_set_tuner_gain_mode(sdr_params->sdr_ptr, 1);
        rtlsdr_set_tuner_gain(sdr_params->sdr_ptr, gains[sdr_params->gain_idx - 1]);
    }
}

// усление сигнала на чипе rtl
void SDR::set_rtl_agc(bool new_rtl_agc)
{
    sdr_params->rtl_agc = new_rtl_agc;
    rtlsdr_set_agc_mode(sdr_params->sdr_ptr, new_rtl_agc);
}

// смена частоты дискретизации
void SDR::set_sample_rate(int new_sample_rate)
{
    if((new_sample_rate >= 230000 && new_sample_rate < 300001) || (new_sample_rate > 900000 && new_sample_rate < 3200000)){
        sdr_params->sample_rate = new_sample_rate;
        rtlsdr_set_sample_rate(sdr_params->sdr_ptr, uint(new_sample_rate));
    }
}

// установка всех настроек на приемнике
// возвращает 0 при успешном выполнении
//           -1 если не удалось выставить частоту дискретизации
//           -2 если не удалось выставить частоту настрйоки
//           -3 если не удалось выставить режим оцифровки
//           -4 если не удалось выставить коэффициент усиления
//           -5 если не удалось установить усиление чипа realtek
int SDR::set_params_to_device()
{
    if(rtlsdr_set_sample_rate(sdr_params->sdr_ptr, uint(sdr_params->sample_rate)))
        return -1;

    if(rtlsdr_set_center_freq(sdr_params->sdr_ptr, uint(sdr_params->central_freq)))
        return -2;

    if(rtlsdr_set_direct_sampling(sdr_params->sdr_ptr, sdr_params->direct_sampling_idx))
        return -3;

    if(!sdr_params->gain_idx){
        if(rtlsdr_set_tuner_gain_mode(sdr_params->sdr_ptr, 0))
            return -4;
    }else{
        if(rtlsdr_set_tuner_gain_mode(sdr_params->sdr_ptr, 1) ||
           rtlsdr_set_tuner_gain(sdr_params->sdr_ptr, gains[sdr_params->gain_idx - 1]))
            return -4;
    }

    if(rtlsdr_set_agc_mode(sdr_params->sdr_ptr, sdr_params->rtl_agc))
        return -5;

    return 0;
}

QString SDR::get_device_params()
{
    return  "Центральная частота: " + QString::number(rtlsdr_get_center_freq(sdr_params->sdr_ptr))
            + "\n   Усиление: " + QString::number(rtlsdr_get_tuner_gain(sdr_params->sdr_ptr))
            + "\n   Частота дискретизации: " + QString::number(rtlsdr_get_sample_rate(sdr_params->sdr_ptr))
            + "\n   Квадратуры: "  + QString::number(rtlsdr_get_direct_sampling(sdr_params->sdr_ptr));
}

// считывание инфы о всех подключенных приемниках
void SDR::read_sdr_info()
{
    int _sdrCnt = int(rtlsdr_get_device_count());

    if(_sdrCnt){
        sdr_params->sdrInfo = "Найденные SDR приемники:\n";

        sdr_params->sdrInfo += QString("Производитель").leftJustified(14) + QString("Модель").leftJustified(14) + QString("ID").leftJustified(5) + "\n";
        sdr_params->findedIdxs.clear();

        char manufact[256], product[256], serial[256];

        for(int sdrIdx = 0; sdrIdx < _sdrCnt; sdrIdx++){

            rtlsdr_get_device_usb_strings(uint(sdrIdx), manufact, product, serial);

            sdr_params->sdrInfo += QString(manufact).leftJustified(14) + QString(product).leftJustified(14) + QString::number(QString(serial).toInt()).leftJustified(5);
        }
    }else{
        sdr_params->sdrInfo = "Не удалось обнаружить подключенные приемники";
    }
}
