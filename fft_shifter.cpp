#include "fft_shifter.h"

fft_shifter::fft_shifter(DSP_params *new_dsp_params,
                         SDR_params *new_sdr_params)
{
    if(new_dsp_params != nullptr)
        dsp_params = new_dsp_params;
    else
        dsp_params = new DSP_params();

    if(new_sdr_params != nullptr)
        sdr_params = new_sdr_params;
    else
        sdr_params = new SDR_params();
}

fft_shifter::~fft_shifter()
{
}

// сдвиг
void fft_shifter::get_shift_step(Ipp32fc *rb_cell, unsigned int cell_size)
{
    // комплексная синусоида
    ippsTone_32fc(dsp_params->shift_params->complex_sin,
                  cell_size / 2,
                  1,
                  fabs(dsp_params->shift_params->shift_freq) / double(sdr_params->sample_rate),
                  &dsp_params->shift_params->CurrentPhase,
                  ippAlgHintAccurate);

    // учет отрицательного сдвига
    if(dsp_params->shift_params->shift_freq < 0)
        ippsConj_32fc_I(dsp_params->shift_params->complex_sin,
                        cell_size / 2);

    // домножение
    ippsMul_32fc(rb_cell,
                 dsp_params->shift_params->complex_sin,
                 dsp_params->shift_params->shift_rb[dsp_params->shift_params->shift_rb_cell_idx],
                 cell_size / 2);

    // запись
    if(dsp_params->read_params->use_third_file && dsp_params->read_params->use_files)
        emit write_to_file(dsp_params->shift_params->shift_rb[dsp_params->shift_params->shift_rb_cell_idx]);

    // бпф
    if(dsp_params->fft_params->fft_mode == SHIFT_FFT)
        emit get_fft_step(dsp_params->shift_params->shift_rb[dsp_params->shift_params->shift_rb_cell_idx],
                          dsp_params->shift_params->shift_rb_cell_size);

    // вывод звука
    emit get_sound_step(dsp_params->read_params->read_rb[dsp_params->read_params->read_rb_cell_idx],
                        dsp_params->read_params->read_rb_cell_size);

    // инкремент итератора
    dsp_params->shift_params->shift_rb_cell_idx = (dsp_params->shift_params->shift_rb_cell_idx + 1) % DSP_SHIFT_RB_SIZE;
}

