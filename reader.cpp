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

#include "reader.h"

READER::READER(SDR_params *new_sdr_params, DSP_params *new_dsp_params): sdr_params{new_sdr_params}, dsp_params{new_dsp_params}
{}

READER::~READER()
{}

void READER::start_reading()
{
    rtlsdr_reset_buffer(sdr_params->sdr_ptr);
    while(dsp_params->read_params.is_receiving){
        get_one_read_step();
    }
}

void READER::get_one_read_step()
{
    int i;

    // общение с железкой
    rtlsdr_read_sync(sdr_params->sdr_ptr,
                     dsp_params->read_params.read_cell,
                     dsp_params->read_params.rb.cell_size,
                     &i);

    // конвертация 8s -> 32f
    ippsConvert_8u32f(dsp_params->read_params.read_cell,
                      reinterpret_cast<Ipp32f*>(dsp_params->read_params.rb.get_current_cell()),
                      dsp_params->read_params.rb.cell_size);

    // подавление постоянной составляющей
    ippsSubC_32f_I(dsp_params->fft_params.dc_offset_gain,
                   reinterpret_cast<Ipp32f*>(dsp_params->read_params.rb.get_current_cell()),
                   dsp_params->read_params.rb.cell_size);

    // инверсия спектра
    if(dsp_params->fft_params.fft_inversion)
        ippsConj_32fc_I(dsp_params->read_params.rb.get_current_cell(),
                        dsp_params->read_params.rb.cell_size / 2);

// emit get_compenaste_step(dsp_params->read_params.rb.get_current_cell());

    // БПФ
    if(dsp_params->fft_params.fft_mode == READER_FFT && dsp_params->read_params.is_receiving)
        emit get_fft_step(dsp_params->read_params.rb.get_current_cell());

    // фильтрация
    if(dsp_params->flt_params.fltRFrec < 0.5)
        emit get_filtration_step(dsp_params->read_params.rb.get_current_cell());

    // вывод в файл
    if(dsp_params->read_params.write_to_file)
        emit write_to_file_32f(reinterpret_cast<Ipp32f*>(dsp_params->read_params.rb.get_current_cell()), dsp_params->read_params.rb.cell_size);

    // инкремент итератора
    dsp_params->read_params.rb.increase_cnt();
}
