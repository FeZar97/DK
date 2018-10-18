#include "wav_recorder.h"

wav_recorder::wav_recorder(DSP_params *new_dsp_params,
                           SDR_params *new_sdr_params)
{
    if(new_dsp_params != NULL)
        dsp_params = new_dsp_params;
    else
        dsp_params = new DSP_params();

    if(new_sdr_params != NULL)
        sdr_params = new_sdr_params;
    else
        sdr_params = new SDR_params();

    make_wav_headers();

    out_buf = new Ipp8u[dsp_params->read_params->read_rb_cell_size * 2];
}

wav_recorder::~wav_recorder()
{
    if(dsp_params->wav_params->first_file.isOpen())
        dsp_params->wav_params->first_file.close();

    if(dsp_params->wav_params->second_file.isOpen())
        dsp_params->wav_params->second_file.close();

    if(dsp_params->wav_params->third_file.isOpen())
        dsp_params->wav_params->third_file.close();
}

void wav_recorder::make_wav_headers()
{
    // первый файл
    memcpy(dsp_params->wav_params->first_header.chunkId, "RIFF", 4); // 0x52494646 в big-endian представлении
    dsp_params->wav_params->first_header.chunkSize = dsp_params->wav_params->first_file_total_size - 8; // data_size + 36
    memcpy(dsp_params->wav_params->first_header.format, "WAVE", 4); // (0x57415645 в big-endian представлении)
    memcpy(dsp_params->wav_params->first_header.subchunk1Id, "fmt ", 4); // (0x666d7420 в big-endian представлении)
    dsp_params->wav_params->first_header.subchunk1Size = 16; // 16 для формата PCM
    dsp_params->wav_params->first_header.audioFormat = 1;
    dsp_params->wav_params->first_header.numChannels = 2;
    dsp_params->wav_params->first_header.sampleRate = sdr_params->sample_rate;
    dsp_params->wav_params->first_header.charRate = dsp_params->wav_params->first_header.sampleRate * dsp_params->wav_params->first_header.numChannels * sizeof(char);
    dsp_params->wav_params->first_header.blockAlign = dsp_params->wav_params->first_header.numChannels * sizeof(char);
    dsp_params->wav_params->first_header.bitsPerSample = sizeof(char) * 8;
    memcpy(dsp_params->wav_params->first_header.subchunk2Id, "data", 4);
    dsp_params->wav_params->first_header.subchunk2Size = dsp_params->wav_params->first_file_total_size;
}

void wav_recorder::write_to_file(Ipp32fc *rb_cell)
{
    if(dsp_params->wav_params->first_file_pos < dsp_params->wav_params->first_file_total_size){

        ippsConvert_32f8u_Sfs((Ipp32f*)rb_cell, out_buf, dsp_params->read_params->read_rb_cell_size, ippRndNear, 0);

        quint64 res = dsp_params->wav_params->first_file.write((char*)(out_buf), dsp_params->read_params->read_rb_cell_size);
        dsp_params->wav_params->first_file_pos += res;

        emit update_progr_bar((dsp_params->wav_params->first_file_pos / (float)dsp_params->wav_params->first_file_total_size) * 100 + 1);

        if(dsp_params->wav_params->first_file_pos == dsp_params->wav_params->first_header.subchunk2Size){
            dsp_params->wav_params->first_file.close();
            qDebug() << "closing" << endl;
        }
    }
}

