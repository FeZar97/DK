#include "mrfiltering.h"
#include "filter.h"

MRFiltering::MRFiltering(DSP_params *new_dsp_params,
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

MRFiltering::~MRFiltering()
{
}

// передается указатель на массив принятого комплексного ЦП с поста чтения
void MRFiltering::get_filtration_step(Ipp32fc *rb_cell)
{
    // фильтрация
    ippsFIRSR_32fc(rb_cell,
                   dsp_params->flt_params->filtration_rb[dsp_params->flt_params->filtration_rb_cell_idx],
                   dsp_params->read_params->read_rb_cell_size / 2,
                   dsp_params->flt_params->flt_spec,
                   dsp_params->flt_params->delay_line,
                   dsp_params->flt_params->delay_line,
                   dsp_params->flt_params->buf);

    // запись в файл
    if(dsp_params->read_params->use_second_file && dsp_params->read_params->use_files)
        emit write_to_file(dsp_params->flt_params->filtration_rb[dsp_params->flt_params->filtration_rb_cell_idx]);

    // БПФ
    if(dsp_params->fft_params->fft_mode == FLT_FFT)
        emit get_fft_step(dsp_params->flt_params->filtration_rb[dsp_params->flt_params->filtration_rb_cell_idx],
                          dsp_params->read_params->read_rb_cell_size);

    // частотный сдвиг
    emit get_shift_step(dsp_params->flt_params->filtration_rb[dsp_params->flt_params->filtration_rb_cell_idx],
                        dsp_params->read_params->read_rb_cell_size);

    // инкремент итератора по КБ
    dsp_params->flt_params->filtration_rb_cell_idx = (dsp_params->flt_params->filtration_rb_cell_idx + 1) % DSP_FLT_RB_SIZE;
}

