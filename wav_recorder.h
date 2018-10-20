#ifndef WAV_RECORDER_H
#define WAV_RECORDER_H

#include <definitions.h>

class wav_recorder : public QObject
{
    Q_OBJECT
public:

    WAV_params  params;

    explicit wav_recorder();
    ~wav_recorder();

public slots:
    void write_to_file(Ipp32fc *rb_cell);

};

#endif // WAV_RECORDER_H
