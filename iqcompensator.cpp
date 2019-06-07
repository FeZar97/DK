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

#include "iqcompensator.h"

IQCompensator::IQCompensator(DSP_params *new_dsp_params): dsp_params{new_dsp_params}
{}

IQCompensator::~IQCompensator()
{}

void IQCompensator::get_compensate_step(Ipp32fc *cell)
{
    tetta1 = 0;
    tetta2 = 0;
    tetta3 = 0;

    for(int i = 0; i < dsp_params->read_params.rb.cell_size/2; i++){
        tetta1 += (cell[i].re > 0 ? 1 : -1) * cell[i].im;
        tetta2 += fabs(cell[i].re);
        tetta3 += fabs(cell[i].im);
    }

    tetta1 *= (-1.) / dsp_params->read_params.rb.cell_size/2;
    tetta2 /= dsp_params->read_params.rb.cell_size/2;
    tetta3 /= dsp_params->read_params.rb.cell_size/2;

    c1 = tetta1 / tetta2;
    c2 = sqrtf( (powf(tetta3, 2) - powf(tetta1, 2)) / (powf(tetta2,2)) );

    g = tetta3 / tetta2;
    fi = asinf(tetta1 / tetta3);

    //for(int i = 0; i < dsp_params->read_params.rb.cell_size/2; i++){
        //cell[i].im += c1 * cell[i].re;
        //cell[i].im = g * cosf(fi) * cell[i].im - g * sinf(fi) * cell[i].re;
        //cell[i].re *= g * cos(fi);
    //}

    dsp_params->fft_params.compensateBuf[dsp_params->fft_params.compensatePtr] = fi * 20000;
    dsp_params->fft_params.compensatePtr = (dsp_params->fft_params.compensatePtr + 1) % DSP_COMPENSATE_BUF_SIZE;

    float avgCompensate = dsp_params->fft_params.compensateBuf[0];
    for(int i = 1; i < DSP_COMPENSATE_BUF_SIZE; i++)
        if(fabs(dsp_params->fft_params.compensateBuf[i]) < fabs(avgCompensate))
            avgCompensate = dsp_params->fft_params.compensateBuf[i];

    if(dsp_params->fft_params.compensatePtr == 0 && fabs(avgCompensate) > 1.5){
        //qDebug() << "avg = " << avgCompensate << endl;
        dsp_params->fft_params.dc_offset_gain += avgCompensate / 1000;
    }

    //avgCompensate += dsp_params->fft_params.compensateBuf[i];
    //avgCompensate /= DSP_COMPENSATE_BUF_SIZE;

}
