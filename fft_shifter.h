#ifndef FFT_SHIFTER_H
#define FFT_SHIFTER_H

#include <definitions.h>

class fft_shifter : public QObject
{
    Q_OBJECT
public:
    SDR_params *sdr_params;
    DSP_params *dsp_params;

    explicit fft_shifter(DSP_params *new_dsp_params = NULL, SDR_params *new_sdr_params = NULL);
    ~fft_shifter();

public slots:
    // прием сигнала с поста чтения
    void get_shift_step(Ipp32fc *rb_cell = NULL);

signals:
    // сигнал для расчета спектра пересчитанного сигнала
    void get_fft_step(Ipp32fc *shift_cell);

    // сигнал на запись в трейти файл сдвинутого сигнала
    void write_to_file(Ipp32fc *shift_rb_cell);
};

#endif // FFT_SHIFTER_H
