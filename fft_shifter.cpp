#include "fft_shifter.h"

fft_shifter::fft_shifter(DSP_params *new_dsp_params,
                         SDR_params *new_sdr_params)
{
    if(new_dsp_params != NULL)
        dsp_params = new_dsp_params;
    else
        dsp_params = new DSP_params();

    if(new_sdr_params != NULL)
        sdr_params = new_sdr_params;
    else
        sdr_params = new SDR_params();
}

fft_shifter::~fft_shifter()
{
    // еще один бесполезный деструктор
}

// сдвиг
void fft_shifter::get_shift_step(Ipp32fc *rb_cell)
{

    qDebug() << "get shift" << endl;

    // комплексная синусоида
    ippsTone_32fc(dsp_params->shift_params->complex_sin,
                  dsp_params->read_params->read_rb_cell_size / 2,
                  1,
                  dsp_params->shift_params->shift_freq / double(sdr_params->sample_rate),
                  &dsp_params->shift_params->CurrentPhase, ippAlgHintAccurate);

    // домножение
    ippsMul_32fc(rb_cell,
                 dsp_params->shift_params->complex_sin,
                 dsp_params->shift_params->shift_rb[dsp_params->shift_params->shift_rb_cell_idx],
                 dsp_params->read_params->read_rb_cell_size / 2);

    // запись
    if(dsp_params->wav_params->use_third_file)
        emit write_to_file(dsp_params->shift_params->shift_rb[dsp_params->shift_params->shift_rb_cell_idx]);

    // бпф
    if(dsp_params->fft_params->fft_mode == SHIFT_FFT)
        emit get_fft_step(dsp_params->shift_params->shift_rb[dsp_params->shift_params->shift_rb_cell_idx]);

    // инкремент итератора
    dsp_params->shift_params->shift_rb_cell_idx = (dsp_params->shift_params->shift_rb_cell_idx + 1) % DSP_SHIFT_RB_SIZE;
}

