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

    // второй файл
    memcpy(dsp_params->wav_params->second_header.chunkId, "RIFF", 4); // 0x52494646 в big-endian представлении
    dsp_params->wav_params->second_header.chunkSize = dsp_params->wav_params->second_file_total_size - 8; // data_size + 36
    memcpy(dsp_params->wav_params->second_header.format, "WAVE", 4); // (0x57415645 в big-endian представлении)
    memcpy(dsp_params->wav_params->second_header.subchunk1Id, "fmt ", 4); // (0x666d7420 в big-endian представлении)
    dsp_params->wav_params->second_header.subchunk1Size = 16; // 16 для формата PCM
    dsp_params->wav_params->second_header.audioFormat = 1;
    dsp_params->wav_params->second_header.numChannels = 2;
    dsp_params->wav_params->second_header.sampleRate = dsp_params->flt_params->out_sample_rate;
    dsp_params->wav_params->second_header.charRate = dsp_params->wav_params->second_header.sampleRate * dsp_params->wav_params->second_header.numChannels * sizeof(char);
    dsp_params->wav_params->second_header.blockAlign = dsp_params->wav_params->second_header.numChannels * sizeof(char);
    dsp_params->wav_params->second_header.bitsPerSample = sizeof(char) * 8;
    memcpy(dsp_params->wav_params->second_header.subchunk2Id, "data", 4);
    dsp_params->wav_params->second_header.subchunk2Size = dsp_params->wav_params->second_file_total_size;

    // третий файл
    memcpy(dsp_params->wav_params->third_header.chunkId, "RIFF", 4); // 0x52494646 в big-endian представлении
    dsp_params->wav_params->third_header.chunkSize = dsp_params->wav_params->third_file_total_size - 8; // data_size + 36
    memcpy(dsp_params->wav_params->third_header.format, "WAVE", 4); // (0x57415645 в big-endian представлении)
    memcpy(dsp_params->wav_params->third_header.subchunk1Id, "fmt ", 4); // (0x666d7420 в big-endian представлении)
    dsp_params->wav_params->third_header.subchunk1Size = 16; // 16 для формата PCM
    dsp_params->wav_params->third_header.audioFormat = 1;
    dsp_params->wav_params->third_header.numChannels = 2;
    dsp_params->wav_params->third_header.sampleRate = sdr_params->sample_rate;
    dsp_params->wav_params->third_header.charRate = dsp_params->wav_params->third_header.sampleRate * dsp_params->wav_params->third_header.numChannels * sizeof(char);
    dsp_params->wav_params->third_header.blockAlign = dsp_params->wav_params->third_header.numChannels * sizeof(char);
    dsp_params->wav_params->third_header.bitsPerSample = sizeof(char) * 8;
    memcpy(dsp_params->wav_params->third_header.subchunk2Id, "data", 4);
    dsp_params->wav_params->third_header.subchunk2Size = dsp_params->wav_params->third_file_total_size;
}

