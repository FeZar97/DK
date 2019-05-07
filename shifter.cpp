#include "shifter.h"

SHIFTER::SHIFTER(SDR_params *new_sdr_params, DSP_params *new_dsp_params): sdr_params{new_sdr_params}, dsp_params{new_dsp_params}
{}

SHIFTER::~SHIFTER()
{}

void SHIFTER::get_shift_step(Ipp32fc *rb_cell)
{
    // комплексная синусоида
    ippsTone_32fc(dsp_params->shift_params.complex_sin,
                  dsp_params->shift_params.rb.cell_size / 2,
                  1,
                  float(fabs(dsp_params->shift_params.r_frec)) / sdr_params->sample_rate,
                  &dsp_params->shift_params.sin_phase,
                  ippAlgHintAccurate);

    // случай отрицательного сдвига по частоте
    if(dsp_params->shift_params.r_frec < 0)
        ippsConj_32fc_I(dsp_params->shift_params.complex_sin,
                        dsp_params->shift_params.rb.cell_size / 2);

    // частотный сдвиг
    ippsMul_32fc(rb_cell,
                 dsp_params->shift_params.complex_sin,
                 dsp_params->shift_params.rb.get_current_cell(),
                 dsp_params->shift_params.rb.cell_size / 2);

    // запись в файл
    if(dsp_params->shift_params.write_to_file)
        emit write_to_file32f(reinterpret_cast<Ipp32f*>(dsp_params->shift_params.rb.get_current_cell()), dsp_params->shift_params.rb.cell_size);

    // БПФ
    if(dsp_params->fft_params.fft_mode == SHIFT_FFT)
        emit get_fft_step(dsp_params->shift_params.rb.get_current_cell());

    // демодуляция
    emit get_demod_step(dsp_params->shift_params.rb.get_current_cell());

    // инкремент итератора
    dsp_params->shift_params.rb.increase_cnt();
}
