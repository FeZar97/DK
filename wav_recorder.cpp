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

#include "wav_recorder.h"

WAV_RECORDER::WAV_RECORDER()
{
    params = new WAV_params();
}

WAV_RECORDER::~WAV_RECORDER()
{
    delete params;
}

void WAV_RECORDER::prepair_memory(size_t cell_size)
{
    if(params->out_buf){
        delete[] params->out_buf;
        params->out_buf = nullptr;
    }

    params->out_buf = new char[cell_size * (DSP_WAV_BITS_PER_SAMPLE / 8)];
}

void WAV_RECORDER::make_wav_header(int sample_rate, int channel_number)
{
    memcpy(params->header.chunkId, "RIFF", 4); // 0x52494646 в big-endian представлении
    params->header.chunkSize = params->file.pos() - 8; // data_size + 36
    memcpy(params->header.format, "WAVE", 4); // (0x57415645 в big-endian представлении)
    memcpy(params->header.subchunk1Id, "fmt ", 4); // (0x666d7420 в big-endian представлении)
    params->header.subchunk1Size = 16; // 16 для формата PCM
    params->header.audioFormat = 1;
    params->header.numChannels = channel_number;
    params->header.sampleRate = sample_rate;
    params->header.charRate = sample_rate * params->header.numChannels * (DSP_WAV_BITS_PER_SAMPLE / 8);
    params->header.blockAlign = channel_number * (DSP_WAV_BITS_PER_SAMPLE / 8);
    params->header.bitsPerSample = DSP_WAV_BITS_PER_SAMPLE;
    memcpy(params->header.subchunk2Id, "data", 4);
    params->header.subchunk2Size = params->file.pos();
}

bool WAV_RECORDER::open_file()
{
    params->file.setFileName(params->path + params->file_name);
    params->file.open(QIODevice::WriteOnly);
    if(params->file.isOpen()){
        params->file.seek(sizeof(WAVEHEADER));
        return true;
    }else{
        return false;
    }
}

void WAV_RECORDER::close_file(int sample_rate, int chan_num)
{
    if(params->file.isOpen()){
        make_wav_header(sample_rate, chan_num);
        params->file.seek(0);
        params->file.write(reinterpret_cast<char*>(&params->header), sizeof(WAVEHEADER));
        params->file.close();
    }
}

QString WAV_RECORDER::size_to_str()
{
    QString str;
    double kbytes = params->file.pos() / 1024;
    if(kbytes < 1024)
        return QString::number(kbytes, 'f', 2) + " кБ";
    else
        return QString::number(kbytes / 1024, 'f', 2) + " МБ";
}

void WAV_RECORDER::get_write_step_32f(Ipp32f *cell, int cell_size)
{
    if(DSP_WAV_BITS_PER_SAMPLE == 8){
        ippsConvert_32f8s_Sfs(cell, reinterpret_cast<Ipp8s*>(params->out_buf), cell_size, ippRndNear, 1);
    }
    if(DSP_WAV_BITS_PER_SAMPLE == 16){
        ippsConvert_32f16s_Sfs(cell, reinterpret_cast<Ipp16s*>(params->out_buf), cell_size, ippRndNear, 1);
    }
    params->file.write(reinterpret_cast<char*>(params->out_buf), cell_size * (DSP_WAV_BITS_PER_SAMPLE / 8));
    emit update_size_label();
}

void WAV_RECORDER::get_write_step_16s(Ipp16s *cell, int cell_size)
{
    params->file.write(reinterpret_cast<char*>(cell), uint(cell_size) * sizeof(Ipp16s));
    emit update_size_label();
}
