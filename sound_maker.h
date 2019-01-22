#ifndef SOUND_MAKER_H
#define SOUND_MAKER_H

#include <definitions.h>

class sound_maker : public QObject
{
    Q_OBJECT
public:
    static const int BUF_SIZE = 48000;

    SDR_params       *sdr_params;
    DSP_params       *dsp_params;

    int              phase;
    Ipp32fc          *in_buf_fc;

// out
    Ipp32fc          down_buf_fc[BUF_SIZE];
    int              down_buf_len;

    Ipp32f           last_sample;
    Ipp32f           down_buf_re[BUF_SIZE];
    Ipp32f           down_buf_im[BUF_SIZE];

    Ipp16s           out_buf[BUF_SIZE];
    QFile            *out_file;

    QAudioOutput     *audio_out;
    QIODevice        *audio_device;

    explicit sound_maker(DSP_params *new_dsp_params = nullptr, SDR_params *new_sdr_params = nullptr);
    ~sound_maker();

signals:
    // сигнал на запись в третий файл сдвинутого сигнала
    void write_to_file(Ipp32fc *sound_rb_cell);

public slots:
    // воркер
    void get_sound_step(Ipp32fc *rb_cell = nullptr, unsigned int cell_size = 0);
};

#endif // SOUND_MAKER_H
