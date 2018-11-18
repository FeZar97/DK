#ifndef SOUND_MAKER_H
#define SOUND_MAKER_H

#include <definitions.h>

class sound_maker : public QObject
{
    Q_OBJECT
public:
    SDR_params *sdr_params;
    DSP_params *dsp_params;

    explicit sound_maker(DSP_params *new_dsp_params = NULL, SDR_params *new_sdr_params = NULL);
    ~sound_maker();

signals:
    // сигнал на запись в третий файл сдвинутого сигнала
    void write_to_file(Ipp32fc *sound_rb_cell);

public slots:
    // воркер
    void get_sound_step(Ipp32fc *rb_cell = NULL, unsigned int cell_size = 0);
};

#endif // SOUND_MAKER_H
