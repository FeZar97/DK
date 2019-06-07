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

#ifndef SOUNDER_H
#define SOUNDER_H

#include <definitions.h>

class SOUNDER : public QObject
{
    Q_OBJECT
public:

    SDR_params *sdr_params;
    DSP_params *dsp_params;

    SOUNDER(SDR_params *new_sdr_params = nullptr, DSP_params *new_dsp_params = nullptr);
    ~SOUNDER();

public slots:
    void get_sound_step(Ipp32f *cell = nullptr);

signals:
    void write_to_file16s(Ipp16s *cell, int cell_size);
};

#endif // SOUNDER_H
