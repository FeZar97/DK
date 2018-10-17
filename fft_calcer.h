#ifndef FFT_CALCER_H
#define FFT_CALCER_H

#include <definitions.h>

class fft_calcer : public QObject
{
    Q_OBJECT
public:
    DSP_params *dsp_params;

    explicit fft_calcer(DSP_params *new_dsp_params = NULL);
    ~fft_calcer();

    // для бпф
    IppsFFTSpec_C_32fc  *pFftSpec;              // спецификация
    Ipp8u               *fft_buf;               // буфер БПФ

    Ipp32fc             fft_dst[DSP_FFT_SIZE];  // кусок сигнала
public slots:
    // воркер
    void get_fft_step(Ipp32fc *rb_cell = NULL);

signals:
    // картинка для телевизора
    void paint_fft();
    // занавес
    void end_fft();

};

#endif // FFT_CALCER_H
