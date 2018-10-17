#include "fft_calcer.h"

fft_calcer::fft_calcer(DSP_params *new_dsp_params)
{
    if(new_dsp_params != NULL)
        dsp_params = new_dsp_params;
    else
        dsp_params = new DSP_params();

    // готовь сани c лета
    int sizeSpec = 0, sizeInit = 0, sizeBuffer = 0;
    ippsFFTGetSize_C_32fc(10, IPP_FFT_DIV_BY_SQRTN, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer);
    Ipp8u *FFTSpec = new Ipp8u[sizeSpec];
    Ipp8u *FFTSpecBuf = new Ipp8u[sizeInit];
    fft_buf = new Ipp8u[sizeBuffer];
    ippsFFTInit_C_32fc(&pFftSpec, 10, IPP_FFT_DIV_BY_SQRTN, ippAlgHintNone, FFTSpec, FFTSpecBuf);
}

fft_calcer::~fft_calcer()
{
    // еще один бесполезный деструктор
}

void fft_calcer::get_fft_step(Ipp32fc *rb_cell)
{
    unsigned int i, j;

    // генеральная уборка
    ippsZero_32f(dsp_params->fft_params->fft_res, DSP_FFT_SIZE);

    /*
       ОСТОРОЖНО!!! ТАВТОЛОГИЯ!!!
       цикл по кол-ву усреднений
       на каждой итерации вычисляется БПФ, добавляется к результирующему массиву
       в конце результирующий массив усредняется по числу усреднений */
    for(i = 0; i < dsp_params->fft_params->fft_averages_number; i++){

        switch(dsp_params->fft_params->fft_current_window){
            case BARLETT:
                ippsWinBartlett_32fc(rb_cell + i * DSP_FFT_SIZE, fft_dst, DSP_FFT_SIZE);
                break;
            case BLACKMANN:
                ippsWinBlackman_32fc(rb_cell + i * DSP_FFT_SIZE, fft_dst, DSP_FFT_SIZE, dsp_params->fft_params->fft_win_alpha);
                break;
            case HAMMING:
                ippsWinHamming_32fc(rb_cell + i * DSP_FFT_SIZE, fft_dst, DSP_FFT_SIZE);
                break;
            case HANN:
                ippsWinHann_32fc(rb_cell + i * DSP_FFT_SIZE, fft_dst, DSP_FFT_SIZE);
                break;
            case KAISER:
                ippsWinKaiser_32fc(rb_cell + i * DSP_FFT_SIZE, fft_dst, DSP_FFT_SIZE, dsp_params->fft_params->fft_win_alpha);
                break;
            case NONE:
                ippsCopy_32fc(rb_cell + i * DSP_FFT_SIZE, fft_dst, DSP_FFT_SIZE);
                break;
        }

        // БПФ от нового куска сигнала
        ippsFFTFwd_CToC_32fc_I(fft_dst, pFftSpec, fft_buf);

        // добавление к общему массиву значений
        for(j = 0; j < DSP_FFT_SIZE; j++)
            dsp_params->fft_params->fft_res[j] += fft_dst[j].re * fft_dst[j].re
                                                + fft_dst[j].im * fft_dst[j].im;
    }

    // усреднение после цикла
    ippsDivC_32f_I(dsp_params->fft_params->fft_averages_number, dsp_params->fft_params->fft_res, DSP_FFT_SIZE);

    // пороги снизу/сверху, перевод в дБ
    ippsThreshold_LT_32f_I(dsp_params->fft_params->fft_res, DSP_FFT_SIZE, 1e-20);
    ippsThreshold_GT_32f_I(dsp_params->fft_params->fft_res, DSP_FFT_SIZE, 1e20);
    ippsLog10_32f_A11(dsp_params->fft_params->fft_res, dsp_params->fft_params->fft_res, DSP_FFT_SIZE);
    ippsMulC_32f_I(10, dsp_params->fft_params->fft_res, DSP_FFT_SIZE);

    // учет нулевого бина
    //dsp_params->fft_params->fft_res[0] = dsp_params->fft_params->fft_res[1] = (dsp_params->fft_params->fft_res[2] > dsp_params->fft_params->fft_res[DSP_FFT_SIZE - 1] ? dsp_params->fft_params->fft_res[2] : dsp_params->fft_params->fft_res[DSP_FFT_SIZE - 1]);

    // на телевизор
    emit paint_fft();
}
