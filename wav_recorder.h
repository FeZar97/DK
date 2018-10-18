#ifndef WAV_RECORDER_H
#define WAV_RECORDER_H

#include <definitions.h>

class wav_recorder : public QObject
{
    Q_OBJECT
public:
    DSP_params *dsp_params;
    SDR_params *sdr_params;

    explicit wav_recorder(DSP_params *new_dsp_params = NULL,
                          SDR_params *new_sdr_params = NULL);
    ~wav_recorder();

    Ipp8u *out_buf;

    void make_wav_headers();

public slots:
    void write_to_file(Ipp32fc *rb_cell);

signals:
    void update_progr_bar(int new_val);

};

#endif // WAV_RECORDER_H
