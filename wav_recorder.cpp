#include "wav_recorder.h"

wav_recorder::wav_recorder()
{
    params = new WAV_params();
}

wav_recorder::~wav_recorder()
{
}

void wav_recorder::write_to_file(Ipp32fc *rb_cell)
{
    if(params->pos < params->total_size){

        ippsConvert_32f8s_Sfs((Ipp32f*)rb_cell, params->out_buf, params->input_cell_size, ippRndNear, 0);

        quint64 res = params->file.write((char*)(params->out_buf), params->input_cell_size);
        params->pos += res;

        emit update_progr_bar(params->pos  * 100 / float(params->total_size) + 1);

        if(params->pos >= params->header.subchunk2Size){

            params->file.close();
            emit end_of_recording();
        }
    }
}

