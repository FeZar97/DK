#include "sound_maker.h"

sound_maker::sound_maker(DSP_params *new_dsp_params,
                         SDR_params *new_sdr_params)
{
    if(new_dsp_params != nullptr)
        dsp_params = new_dsp_params;
    else
        dsp_params = new DSP_params();

    if(new_sdr_params != nullptr)
        sdr_params = new_sdr_params;
    else
        sdr_params = new SDR_params();

    raw_file = new QFile("raw file.pcm");
    raw_file->open(QIODevice::WriteOnly);

    out_file = new QFile("out sound.pcm");
    out_file->open(QIODevice::WriteOnly);

    phase = 0;

    //Настройка звука
    QAudioFormat format;

    format.setSampleRate(DSP_SOUND_SAMPLE_RATE);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());

    if(info.isFormatSupported(format)){
        qDebug() << "sounder has been created";
        audio_out = new QAudioOutput(format, this);
        audio_device = audio_out->start();
    }
}

sound_maker::~sound_maker()
{
    delete dsp_params;
    delete sdr_params;
    delete audio_device;
    delete audio_out;

    if(raw_file->isOpen())
        raw_file->close();

    if(out_file->isOpen())
        out_file->close();
}

void sound_maker::get_sound_step(Ipp32fc *rb_cell, unsigned int cell_size)
{
    if(sdr_params->sample_rate != 1440000)
        qDebug() << "sample rate != 1440000, sounder is not active!";
    else{
        raw_file->write(reinterpret_cast<char*>(rb_cell), 2 * sdr_params->sample_rate / DSP_READOUT_PER_SECONDS);
        // sample_count(float) = 2 * sample_rate / READOUT_PER_SEC = 2 * 1,44kk / 10 = 288k
        // 288k float сэмплов per 0,1 sec ~~ 144k fc сэмплов per 0,1 sec
        // 144k / 15 = 9600 fc сэмплов per 0,1 sec

        // sample_rate = 48kHz ~~ 4800 сэмплов per 0,1 sec

        // децимация (srcLen = cell_size / 2 = 144k, factor = 15, phase всегда 0)
        // pDstLen = (srcLen + factor - 1 - phase) / factor = (144k + 15 - 1 - 0) / 15 = 9600 fc сэмплов
        ippsSampleDown_32fc(rb_cell, cell_size / 2, down_buf_fc, &down_buf_len, 15, &phase);

        ippsCplxToReal_32fc(down_buf_fc, down_buf_re, down_buf_im, BUF_SIZE);
        ippsAtan2_32f_A21(down_buf_re, down_buf_im, down_buf_re, BUF_SIZE);

        // 2 - 1
        //down_buf_im[0] = last_sample - down_buf_re[0];
        //last_sample = down_buf_re[BUF_SIZE - 1];

        ippsSub_32f(down_buf_re + 1, down_buf_re, down_buf_im, BUF_SIZE - 1);

        for(int i = 0; i < BUF_SIZE; i++){
            for(; down_buf_im[i] >  M_PI ; down_buf_im[i] -= 2 * M_PI );
            for(; down_buf_im[i] < -M_PI ; down_buf_im[i] += 2 * M_PI );

            down_buf_im[i] = 32767 * down_buf_im[i] / M_PI;
        }

        ippsConvert_32f16s_Sfs(down_buf_im, out_buf, BUF_SIZE, ippRndNear, 0);

        audio_device->write(reinterpret_cast<char*>(out_buf), BUF_SIZE);
        out_file->write(reinterpret_cast<char*>(out_buf), BUF_SIZE);
    }

}
