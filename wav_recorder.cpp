#include "wav_recorder.h"

wav_recorder::wav_recorder()
{
}

wav_recorder::~wav_recorder()
{
}

void wav_recorder::write_to_file(Ipp32fc *rb_cell)
{
    qDebug() << "write" << endl;

    if(params.pos < params.total_size){

        ippsConvert_32f8u_Sfs((Ipp32f*)rb_cell, params.out_buf, params.input_cell_size, ippRndNear, 0);

        quint64 res = params.file.write((char*)(params.out_buf), params.input_cell_size);
        params.pos += res;

        if(params.pos == params.header.subchunk2Size)
            params.file.close();
    }
}

