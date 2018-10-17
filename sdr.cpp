#include "sdr.h"

SDR::SDR()
{
    sdr_params = new SDR_params();
}

// возвращает кол-во доступных приемников
int SDR::get_sdr_count()
{
    return rtlsdr_get_device_count();
}

// смена частоты настройки
void SDR::set_central_freq(unsigned int new_central_freq)
{
    if(new_central_freq > (SDR_MIN_TUNER_FREQ) && new_central_freq < (SDR_MAX_TUNER_FREQ)){
        sdr_params->central_freq = new_central_freq;
        rtlsdr_set_center_freq(sdr_params->sdr_ptr, sdr_params->central_freq);
        emit global_update_interface();
    }
}

// смена КУ
void SDR::set_gain_idx(unsigned int new_gain_idx)
{
    sdr_params->gain_idx = new_gain_idx;

    if(!sdr_params->gain_idx){
        rtlsdr_set_tuner_gain_mode(sdr_params->sdr_ptr, 0);
    }else{
        rtlsdr_set_tuner_gain_mode(sdr_params->sdr_ptr, 1);
        rtlsdr_set_tuner_gain(sdr_params->sdr_ptr, gains[sdr_params->gain_idx - 1]);
    }
}

// смена частоты дискретизации
void SDR::set_sample_rate(unsigned int new_sample_rate)
{
    if((new_sample_rate >= 230000 && new_sample_rate < 300001) || (new_sample_rate > 900000 && new_sample_rate < 2400001)){
        sdr_params->sample_rate = new_sample_rate;

        // пересчет параметров в DSP
        emit recalc_dsp_params();

        rtlsdr_set_sample_rate(sdr_params->sdr_ptr, new_sample_rate);
    }
}

// установка всех настроек на приемнике
bool SDR::set_params_to_device()
{
    int success = 0;

    success += rtlsdr_set_sample_rate(sdr_params->sdr_ptr, sdr_params->sample_rate);
    success += rtlsdr_set_center_freq(sdr_params->sdr_ptr, sdr_params->central_freq);
    success += rtlsdr_set_direct_sampling(sdr_params->sdr_ptr, sdr_params->direct_sampling_idx);

    if(!sdr_params->gain_idx){
        success += rtlsdr_set_tuner_gain_mode(sdr_params->sdr_ptr, 0);
    }else{
        success += rtlsdr_set_tuner_gain_mode(sdr_params->sdr_ptr, 1);
        success += rtlsdr_set_tuner_gain(sdr_params->sdr_ptr, gains[sdr_params->gain_idx - 1]);
    }

    return success;
}

QString SDR::get_device_params()
{
    return  "Центральная частота: " + QString::number(rtlsdr_get_center_freq(sdr_params->sdr_ptr))
            + "\n   Усиление: " + QString::number(rtlsdr_get_tuner_gain(sdr_params->sdr_ptr))
            + "\n   Частота дискретизации: " + QString::number(rtlsdr_get_sample_rate(sdr_params->sdr_ptr))
            + "\n   Квадратуры: "  + QString::number(rtlsdr_get_direct_sampling(sdr_params->sdr_ptr));
}
