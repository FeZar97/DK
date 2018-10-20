#ifndef WAV_RECORDER_H
#define WAV_RECORDER_H

#include <definitions.h>

class wav_recorder : public QObject
{
    Q_OBJECT
public:
    WAV_params  *params;

    explicit wav_recorder();
    ~wav_recorder();

public slots:
    void write_to_file(Ipp32fc *rb_cell);

signals:
    void end_of_recording();
    void update_progr_bar(int new_val);

};

#endif // WAV_RECORDER_H
