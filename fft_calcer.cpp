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

#include "fft_calcer.h"

FFT_CALCER::FFT_CALCER(DSP_params *new_dsp_params): dsp_params{new_dsp_params}
{}

FFT_CALCER::~FFT_CALCER()
{}

void FFT_CALCER::get_fft_step_fc(Ipp32fc *cell)
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
                ippsWinBartlett_32fc(cell + i * DSP_FFT_SIZE, dsp_params->fft_params.fft_dst_fc, DSP_FFT_SIZE);
                break;
            case BLACKMANN:
            // стандартный коэф-т = -0.16, оптимальный = -0.25
            // оптимальный коэф-т для длины len вычисляет следующим образом: -0.5 / (1 + cos(2 * M_PI / (len - 1)))
            // для 1024 = -0,25000235771451474102514418916463
                ippsWinBlackman_32fc(cell + i * DSP_FFT_SIZE, dsp_params->fft_params.fft_dst_fc, DSP_FFT_SIZE, dsp_params->fft_params.fft_win_alpha);
                break;
            case HAMMING:
                ippsWinHamming_32fc(cell + i * DSP_FFT_SIZE, dsp_params->fft_params.fft_dst_fc, DSP_FFT_SIZE);
                break;
            case HANN:
                ippsWinHann_32fc(cell + i * DSP_FFT_SIZE, dsp_params->fft_params.fft_dst_fc, DSP_FFT_SIZE);
                break;
            case NONE:
                ippsCopy_32fc(cell + i * DSP_FFT_SIZE, dsp_params->fft_params.fft_dst_fc, DSP_FFT_SIZE);
                break;
        }

        // БПФ от нового куска сигнала
        ippsFFTFwd_CToC_32fc_I(dsp_params->fft_params.fft_dst_fc,
                               dsp_params->fft_params.pFftSpec_fc,
                               dsp_params->fft_params.fft_buf_fc);

        // добавление к общему массиву значений
        for(j = 0; j < DSP_FFT_SIZE; j++)
            dsp_params->fft_params.fft_res[j] += dsp_params->fft_params.fft_dst_fc[j].re * dsp_params->fft_params.fft_dst_fc[j].re
                                               + dsp_params->fft_params.fft_dst_fc[j].im * dsp_params->fft_params.fft_dst_fc[j].im;
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

void FFT_CALCER::get_fft_step_f(Ipp32f *cell, int cellSize)
{
    int i, j, avgNb = cellSize / DSP_FFT_SIZE;

    // обнуление результирующего массива
    ippsZero_32f(dsp_params->fft_params.fft_res, DSP_FFT_SIZE);

    for(i = 0; i < avgNb; i++){
        switch(dsp_params->fft_params.fft_current_window){
            case BARLETT:
                ippsWinBartlett_32f(cell + i * DSP_FFT_SIZE, dsp_params->fft_params.fft_dst_f, DSP_FFT_SIZE);
                break;
            case BLACKMANN:
            // стандартный коэф-т = -0.16, оптимальный = -0.25
            // оптимальный коэф-т для длины len вычисляет следующим образом: -0.5 / (1 + cos(2 * M_PI / (len - 1)))
            // для 1024 = -0,25000235771451474102514418916463
                ippsWinBlackman_32f(cell + i * DSP_FFT_SIZE, dsp_params->fft_params.fft_dst_f, DSP_FFT_SIZE, dsp_params->fft_params.fft_win_alpha);
                break;
            case HAMMING:
                ippsWinHamming_32f(cell + i * DSP_FFT_SIZE, dsp_params->fft_params.fft_dst_f, DSP_FFT_SIZE);
                break;
            case HANN:
                ippsWinHann_32f(cell + i * DSP_FFT_SIZE, dsp_params->fft_params.fft_dst_f, DSP_FFT_SIZE);
                break;
            case NONE:
                ippsCopy_32f(cell + i * DSP_FFT_SIZE, dsp_params->fft_params.fft_dst_f, DSP_FFT_SIZE);
                break;
        }

        // БПФ от нового куска сигнала
        ippsFFTFwd_RToCCS_32f_I(dsp_params->fft_params.fft_dst_f,
                                dsp_params->fft_params.pFftSpec_f,
                                dsp_params->fft_params.fft_buf_f);

        // добавление к общему массиву значений
        dsp_params->fft_params.fft_res[512]   += dsp_params->fft_params.fft_dst_f[0]*dsp_params->fft_params.fft_dst_f[0];
        dsp_params->fft_params.fft_res[1023] += dsp_params->fft_params.fft_dst_f[1024]*dsp_params->fft_params.fft_dst_f[1024];
        ippsZero_32f(dsp_params->fft_params.fft_res + DSP_FFT_SIZE/2, DSP_FFT_SIZE/2);
        for(j = 1; j < DSP_FFT_SIZE/2; j++)
            dsp_params->fft_params.fft_res[512 + j] += dsp_params->fft_params.fft_dst_f[2 * j] * dsp_params->fft_params.fft_dst_f[2 * j + 1];
    }

    // усреднение после цикла
    ippsDivC_32f_I(avgNb, dsp_params->fft_params.fft_res, DSP_FFT_SIZE);

    // пороги снизу/сверху, перевод в дБ
    ippsThreshold_LT_32f_I(dsp_params->fft_params.fft_res, DSP_FFT_SIZE, 1e-20);
    ippsThreshold_GT_32f_I(dsp_params->fft_params.fft_res, DSP_FFT_SIZE, 1e20);
    ippsLog10_32f_A11(dsp_params->fft_params.fft_res, dsp_params->fft_params.fft_res, DSP_FFT_SIZE);
    ippsMulC_32f_I(10, dsp_params->fft_params.fft_res, DSP_FFT_SIZE);

    emit paint_fft(dsp_params->fft_params.fft_res);
}
