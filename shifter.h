#ifndef SHIFTER_H
#define SHIFTER_H

#include <definitions.h>

class SHIFTER : public QObject
{
    Q_OBJECT
public:
    SDR_params *sdr_params;
    DSP_params *dsp_params;

    SHIFTER(SDR_params *new_sdr_params = nullptr, DSP_params *new_dsp_params = nullptr);
    ~SHIFTER();

public slots:
    void get_shift_step(Ipp32fc *rb_cell = nullptr);

signals:
    void get_fft_step(Ipp32fc *shift_cell); // БПФ

    void get_demod_step(Ipp32fc *rb_cell); // демодуляция

    void write_to_file32f(Ipp32f *cell, int cell_size); // запись в файл
};

#endif // SHIFTER_H
