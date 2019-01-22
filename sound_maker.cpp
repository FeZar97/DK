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

    in_buf_fc = new Ipp32fc[dsp_params->read_params->read_rb_cell_size];

    out_file = new QFile("raw sound.pcm");
    //out_file->open(QIODevice::WriteOnly);

    down_buf_len = BUF_SIZE;
    phase = 0;

    //Настройка звука
    QAudioFormat format;

    format.setSampleRate(BUF_SIZE);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice() );

    if (info.isFormatSupported(format)){
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
}

void sound_maker::get_sound_step(Ipp32fc *rb_cell, unsigned int cell_size)
{
    if(sdr_params->sample_rate != 1440000)
        qDebug() << "sample rate != 1440000, get_sound_step is not active!";
    else{

        ippsCopy_32fc(rb_cell, in_buf_fc, cell_size / 2);

        // децимация
        ippsSampleDown_32fc(in_buf_fc, cell_size / 2, down_buf_fc, &down_buf_len, 15, &phase);

        ippsCplxToReal_32fc(down_buf_fc, down_buf_re, down_buf_im, BUF_SIZE);
        ippsAtan2_32f_A21(down_buf_re, down_buf_im, down_buf_re, BUF_SIZE);

        // 2 - 1
        down_buf_im[0] = last_sample - down_buf_re[0];
        last_sample = down_buf_re[BUF_SIZE - 1];
        ippsSub_32f(down_buf_re + 1, down_buf_re, down_buf_im, BUF_SIZE - 1);

        for(int i = 0; i < BUF_SIZE; i++) {
            for(; down_buf_im[i] >  M_PI ; down_buf_im[i] -= 2*M_PI );
            for(; down_buf_im[i] < -M_PI ; down_buf_im[i] += 2*M_PI );

            down_buf_im[i] = 32767 * down_buf_im[i]/M_PI;
        }

        ippsConvert_32f16s_Sfs(down_buf_im, out_buf, BUF_SIZE, ippRndNear, 0);

        audio_device->write(reinterpret_cast<char*>(out_buf), BUF_SIZE);
    }

}
