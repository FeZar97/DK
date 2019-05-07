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
