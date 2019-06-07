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

#ifndef SDR_H
#define SDR_H

#include <definitions.h>

class SDR : public QObject
{
    Q_OBJECT

public:
    SDR();
    ~SDR();

    SDR_params *sdr_params;

    int get_sdr_count(); // возвращает кол-во доступных приемников

    void set_gain_idx(int new_gain_idx); // смена КУ тюнера
    void set_rtl_agc(bool new_rtl_agc); // усиление rtl чипа
    void set_sample_rate(int new_sample_rate); // смена частоты дискретизации

    int set_params_to_device(); // конфигурация приемника
    QString get_device_params(); // возвращает настройки, высталвенные на приемнике

    void read_sdr_info(); // считывание инфы о приемнике из EEPROM

public slots:
    void set_central_freq(int new_central_freq); // смена частоты настройки

signals:
    void recalc_dsp_params(); // пересчет параметров в DSP
    void update_interface();
};

#endif // SDR_H
