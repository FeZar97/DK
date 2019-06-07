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

#ifndef SHIFTER_H
#define SHIFTER_H

#include <definitions.h>

class SHIFTER : public QObject
{
    Q_OBJECT
public:
    SDR_params *sdr_params;
    DSP_params *dsp_params;

    SHIFTER(SDR_params *new_sdr_params = nullptr, DSP_params *new_dsp_params = nullptr);
    ~SHIFTER();

public slots:
    void get_shift_step(Ipp32fc *rb_cell = nullptr);

signals:
    void get_fft_step(Ipp32fc *shift_cell); // БПФ

    void get_demod_step(Ipp32fc *rb_cell); // демодуляция

    void write_to_file32f(Ipp32f *cell, int cell_size); // запись в файл
};

#endif // SHIFTER_H
