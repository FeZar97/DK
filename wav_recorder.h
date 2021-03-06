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

#ifndef WAV_RECORDER_H
#define WAV_RECORDER_H

#include <definitions.h>

class WAV_RECORDER : public QObject
{
    Q_OBJECT
public:
    WAV_params  *params;

    WAV_RECORDER();
    ~WAV_RECORDER();

    void prepair_memory(size_t cell_size);
    void make_wav_header(int sample_rate, int channel_number);
    bool open_file();
    void close_file(int sample_rate, int chan_num);

    QString size_to_str();

public slots:
    void get_write_step_32f(Ipp32f *cell, int cell_size);
    void get_write_step_16s(Ipp16s *cell, int cell_size);

signals:
    void update_size_label();
};

#endif // WAV_RECORDER_H
