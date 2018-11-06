#include "mrfiltering.h"
#include "filter.h"

MRFiltering::MRFiltering(DSP_params *new_dsp_params,
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

MRFiltering::~MRFiltering()
{
    // бесполезный деструктор
}

// передается указатель на массив принятого комплексного ЦП с поста чтения
void MRFiltering::get_filtration_step(Ipp32fc *rb_cell)
{
    Q_UNUSED(rb_cell);
    /*
    // если фильтрация используется, еще не конец фильтрации
    if(dsp_params->flt_params->is_using && !dsp_params->read_params->end_filtering){
        unsigned int i;

        // конвертация входного вектора 8u->32f
        ippsConvert_8u32f(rb_cell,
                          dsp_params->flt_params->temp_32fc,
                          dsp_params->read_params->read_rb_cell_size);

        // разбиение на re и im
        for(i = 0; i < dsp_params->read_params->read_rb_cell_size / 2; i++){
            dsp_params->flt_params->temp_32f_re[i] = dsp_params->flt_params->temp_32fc[i * 2];
            dsp_params->flt_params->temp_32f_im[i] = dsp_params->flt_params-> temp_32fc[i * 2 + 1];
        }

        // фильтрация
        unsigned int numIters = sdr_params->sample_rate / (dsp_params->flt_params->down_factor * dsp_params->read_params->readout_per_seconds);
        ippsFIRMR_32f(dsp_params->flt_params->temp_32f_re,
                      dsp_params->flt_params->flt_32f_re,
                      numIters,
                      dsp_params->flt_params->pSpec,
                      dsp_params->flt_params->delay_re,
                      dsp_params->flt_params->delay_re,
                      dsp_params->flt_params->buf);

        ippsFIRMR_32f(dsp_params->flt_params->temp_32f_im,
                      dsp_params->flt_params->flt_32f_im,
                      numIters,
                      dsp_params->flt_params->pSpec,
                      dsp_params->flt_params->delay_im,
                      dsp_params->flt_params->delay_im,
                      dsp_params->flt_params->buf);

        for(i = 0; i < dsp_params->flt_params->filtration_rb_cell_size / 2; i++){
            dsp_params->flt_params->filtration_rb[dsp_params->flt_params->filtration_rb_cell_idx][2 * i]     = dsp_params->flt_params->flt_32f_re[i];
            dsp_params->flt_params->filtration_rb[dsp_params->flt_params->filtration_rb_cell_idx][2 * i + 1] = dsp_params->flt_params->flt_32f_im[i];
        }

        // запись в файл
        if(dsp_params->read_params->use_second_file && dsp_params->read_params->use_files)
            emit write_to_file(dsp_params->flt_params->filtration_rb[dsp_params->flt_params->filtration_rb_cell_idx]);

        // БПФ
        if(dsp_params->fft_params->fft_mode == FLT_FFT){
            emit get_fft_step(dsp_params->flt_params->filtration_rb[dsp_params->flt_params->filtration_rb_cell_idx]);
        }

        // инкремент итератора по КБ
        dsp_params->flt_params->filtration_rb_cell_idx = (dsp_params->flt_params->filtration_rb_cell_idx + 1) % DSP_FLT_RB_SIZE;
    }
    */
}

