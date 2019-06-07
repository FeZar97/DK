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

#include "filter.h"

FILTER::FILTER(SDR_params *new_sdr_params, DSP_params *new_dsp_params) : sdr_params{new_sdr_params}, dsp_params{new_dsp_params}
{}

FILTER::~FILTER()
{}

void FILTER::get_filtration_step(Ipp32fc *cell)
{
    // замена спецификации при наличии новых коэф-тов
    if(dsp_params->flt_params.new_coeffs_ready){
        ippsFIRSRInit_32fc(dsp_params->flt_params.flt_taps32, DSP_FLT_LENGTH, ippAlgAuto, dsp_params->flt_params.flt_spec);
        dsp_params->flt_params.new_coeffs_ready = false;
    }

    // фильтрация
    ippsFIRSR_32fc(cell,
                   dsp_params->flt_params.rb.get_current_cell(),
                   dsp_params->flt_params.rb.cell_size / 2,
                   dsp_params->flt_params.flt_spec,
                   dsp_params->flt_params.delay_line,
                   dsp_params->flt_params.delay_line,
                   dsp_params->flt_params.buf);

    // запись в файл
    if(dsp_params->flt_params.write_to_file)
        emit write_to_file_32f(reinterpret_cast<Ipp32f*>(dsp_params->flt_params.rb.get_current_cell()), dsp_params->flt_params.rb.cell_size);

    // БПФ
    if(dsp_params->fft_params.fft_mode == FLT_FFT){
        emit get_fft_step(dsp_params->flt_params.rb.get_current_cell());
    }

    // частотный сдвиг
    emit get_shift_step(dsp_params->flt_params.rb.get_current_cell());

    // инкремент итератора по КБ
    dsp_params->flt_params.rb.increase_cnt();
}
