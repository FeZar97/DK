#include "fft_calcer.h"

fft_calcer::fft_calcer(DSP_params *new_dsp_params)
{
    if(new_dsp_params != nullptr)
        dsp_params = new_dsp_params;
    else
        dsp_params = new DSP_params();
}

fft_calcer::~fft_calcer()
{
    if(dsp_params)
        delete dsp_params;
}

void fft_calcer::get_fft_step(Ipp32fc *rb_cell, unsigned int cell_size)
{
    Q_UNUSED(cell_size);

    unsigned int i, j;

    // обнуление результирующего массива
    ippsZero_32f(dsp_params->fft_params->fft_res, DSP_FFT_SIZE);

    /*
       ОСТОРОЖНО!!! ТАВТОЛОГИЯ!!!
       цикл по кол-ву усреднений
       на каждой итерации вычисляется БПФ, добавляется к результирующему массиву
       в конце результирующий массив усредняется по числу усреднений */
    for(i = 0; i < dsp_params->fft_params->fft_averages_number; i++){

        switch(dsp_params->fft_params->fft_current_window){
            case BARLETT:
                ippsWinBartlett_32fc(rb_cell + i * DSP_FFT_SIZE, dsp_params->fft_params->fft_dst, DSP_FFT_SIZE);
                break;
            case BLACKMANN:
                ippsWinBlackman_32fc(rb_cell + i * DSP_FFT_SIZE, dsp_params->fft_params->fft_dst, DSP_FFT_SIZE, dsp_params->fft_params->fft_win_alpha);
                break;
            case HAMMING:
                ippsWinHamming_32fc(rb_cell + i * DSP_FFT_SIZE, dsp_params->fft_params->fft_dst, DSP_FFT_SIZE);
                break;
            case HANN:
                ippsWinHann_32fc(rb_cell + i * DSP_FFT_SIZE, dsp_params->fft_params->fft_dst, DSP_FFT_SIZE);
                break;
            case KAISER:
                ippsWinKaiser_32fc(rb_cell + i * DSP_FFT_SIZE, dsp_params->fft_params->fft_dst, DSP_FFT_SIZE, dsp_params->fft_params->fft_win_alpha);
                break;
            case NONE:
                ippsCopy_32fc(rb_cell + i * DSP_FFT_SIZE, dsp_params->fft_params->fft_dst, DSP_FFT_SIZE);
                break;
        }

        // БПФ от нового куска сигнала
        ippsFFTFwd_CToC_32fc_I(dsp_params->fft_params->fft_dst,
                               dsp_params->fft_params->pFftSpec,
                               dsp_params->fft_params->fft_buf);

        // добавление к общему массиву значений
        for(j = 0; j < DSP_FFT_SIZE; j++)
            dsp_params->fft_params->fft_res[j] += dsp_params->fft_params->fft_dst[j].re * dsp_params->fft_params->fft_dst[j].re
                                                + dsp_params->fft_params->fft_dst[j].im * dsp_params->fft_params->fft_dst[j].im;
    }

    // усреднение после цикла
    ippsDivC_32f_I(dsp_params->fft_params->fft_averages_number, dsp_params->fft_params->fft_res, DSP_FFT_SIZE);

    // пороги снизу/сверху, перевод в дБ
    ippsThreshold_LT_32f_I(dsp_params->fft_params->fft_res, DSP_FFT_SIZE, 1e-20);
    ippsThreshold_GT_32f_I(dsp_params->fft_params->fft_res, DSP_FFT_SIZE, 1e20);
    ippsLog10_32f_A11(dsp_params->fft_params->fft_res, dsp_params->fft_params->fft_res, DSP_FFT_SIZE);
    ippsMulC_32f_I(10, dsp_params->fft_params->fft_res, DSP_FFT_SIZE);

// ШУМ
    // поиск минимума и сохранение в массив
    int min_idx = 0;
    for(i = 1; i < DSP_FFT_SIZE/2 - DSP_FFT_SIZE/5; i++)
        if(dsp_params->fft_params->fft_res[i] < dsp_params->fft_params->fft_res[min_idx])
            min_idx = i;

    for(i = DSP_FFT_SIZE/2 + DSP_FFT_SIZE/5; i < DSP_FFT_SIZE; i++)
        if(dsp_params->fft_params->fft_res[i] < dsp_params->fft_params->fft_res[min_idx])
            min_idx = i;

    // убираем из аккумулятора самый старый уровень
    dsp_params->fft_params->noise_accum -= dsp_params->fft_params->noise_buf[dsp_params->fft_params->noise_idx];
    // заменяем старый уровень на новый
    dsp_params->fft_params->noise_buf[dsp_params->fft_params->noise_idx] = dsp_params->fft_params->fft_res[min_idx];
    // добавляем к аккумулятору новый уровень
    dsp_params->fft_params->noise_accum += dsp_params->fft_params->noise_buf[dsp_params->fft_params->noise_idx];
    // считаем среднее скользящего окна
    dsp_params->fft_params->noise_level = dsp_params->fft_params->noise_accum / DSP_NOISE_SIZE;
    //dsp_params->fft_params->noise_level += 1.8;

    // итератор по кольцевому
    dsp_params->fft_params->noise_idx = (dsp_params->fft_params->noise_idx + 1) % DSP_NOISE_SIZE;
//--------------------------------------------------------------------------------------------------------------

// поиск максимального бина БПФ
    dsp_params->fft_params->max_level_idx = 0;
    for(i = 1; i < DSP_FFT_SIZE; i++)
        if(dsp_params->fft_params->fft_res[i] > dsp_params->fft_params->fft_res[dsp_params->fft_params->max_level_idx])
            dsp_params->fft_params->max_level_idx = int(i);
//--------------------------------------------------------------------------------------------------------------------

    emit paint_fft();
}
