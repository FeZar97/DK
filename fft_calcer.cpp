#include "fft_calcer.h"

FFT_CALCER::FFT_CALCER(DSP_params *new_dsp_params): dsp_params{new_dsp_params}
{}

FFT_CALCER::~FFT_CALCER()
{}

void FFT_CALCER::get_fft_step(Ipp32fc *cell)
{
    int i, j;

    // обнуление результирующего массива
    ippsZero_32f(dsp_params->fft_params.fft_res, DSP_FFT_SIZE);

    /*
    ОСТОРОЖНО!!! ТАВТОЛОГИЯ!!!
    цикл по кол-ву усреднений
    на каждой итерации вычисляется БПФ, добавляется к результирующему массиву
    в конце результирующий массив усредняется по числу усреднений */
    for(i = 0; i < dsp_params->fft_params.fft_averages_number; i++){

        switch(dsp_params->fft_params.fft_current_window){
            case BARLETT:
                ippsWinBartlett_32fc(cell + i * DSP_FFT_SIZE, dsp_params->fft_params.fft_dst, DSP_FFT_SIZE);
                break;
            case BLACKMANN:
            // стандартный коэф-т = -0.16, оптимальный = -0.25
            // оптимальный коэф-т для длины len вычисляет следующим образом: -0.5 / (1 + cos(2 * M_PI / (len - 1)))
            // для 1024 = -0,25000235771451474102514418916463
                ippsWinBlackman_32fc(cell + i * DSP_FFT_SIZE, dsp_params->fft_params.fft_dst, DSP_FFT_SIZE, dsp_params->fft_params.fft_win_alpha);
                break;
            case HAMMING:
                ippsWinHamming_32fc(cell + i * DSP_FFT_SIZE, dsp_params->fft_params.fft_dst, DSP_FFT_SIZE);
                break;
            case HANN:
                ippsWinHann_32fc(cell + i * DSP_FFT_SIZE, dsp_params->fft_params.fft_dst, DSP_FFT_SIZE);
                break;
            case NONE:
                ippsCopy_32fc(cell + i * DSP_FFT_SIZE, dsp_params->fft_params.fft_dst, DSP_FFT_SIZE);
                break;
        }

        // БПФ от нового куска сигнала
        ippsFFTFwd_CToC_32fc_I(dsp_params->fft_params.fft_dst,
                               dsp_params->fft_params.pFftSpec,
                               dsp_params->fft_params.fft_buf);

        // добавление к общему массиву значений
        for(j = 0; j < DSP_FFT_SIZE; j++)
            dsp_params->fft_params.fft_res[j] += dsp_params->fft_params.fft_dst[j].re * dsp_params->fft_params.fft_dst[j].re
                                                + dsp_params->fft_params.fft_dst[j].im * dsp_params->fft_params.fft_dst[j].im;
    }

    // усреднение после цикла
    ippsDivC_32f_I(dsp_params->fft_params.fft_averages_number, dsp_params->fft_params.fft_res, DSP_FFT_SIZE);

    // пороги снизу/сверху, перевод в дБ
    ippsThreshold_LT_32f_I(dsp_params->fft_params.fft_res, DSP_FFT_SIZE, 1e-20);
    ippsThreshold_GT_32f_I(dsp_params->fft_params.fft_res, DSP_FFT_SIZE, 1e20);
    ippsLog10_32f_A11(dsp_params->fft_params.fft_res, dsp_params->fft_params.fft_res, DSP_FFT_SIZE);
    ippsMulC_32f_I(10, dsp_params->fft_params.fft_res, DSP_FFT_SIZE);

    emit paint_fft(dsp_params->fft_params.fft_res);
}
