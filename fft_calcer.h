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

#ifndef FFT_CALCER_H
#define FFT_CALCER_H

#include <definitions.h>

class FFT_CALCER : public QObject
{
    Q_OBJECT
public:
    DSP_params *dsp_params;

    FFT_CALCER(DSP_params *new_dsp_params = nullptr);
    ~FFT_CALCER();

    void recalc_noise_level();

public slots:
    void get_fft_step_fc(Ipp32fc *cell = nullptr);
    void get_fft_step_f(Ipp32f *cell = nullptr, int cellSize = 0);

signals:
    void paint_fft(float *fftRes);
};

#endif // FFT_CALCER_H
