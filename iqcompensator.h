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

#ifndef IQCOMPENSATOR_H
#define IQCOMPENSATOR_H

#include <definitions.h>

class IQCompensator : public QObject
{
    Q_OBJECT

public:
    DSP_params *dsp_params;

    IQCompensator(DSP_params *new_dsp_params = nullptr);
    ~IQCompensator();

    Ipp32f tetta1;
    Ipp32f tetta2;
    Ipp32f tetta3;

    Ipp32f c1;
    Ipp32f c2;

    Ipp32f g;
    Ipp32f fi;

public slots:
    void get_compensate_step(Ipp32fc *cell);
};

#endif // IQCOMPENSATOR_H
