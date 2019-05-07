#ifndef FFT_CALCER_H
#define FFT_CALCER_H

#include <definitions.h>

class FFT_CALCER : public QObject
{
    Q_OBJECT
public:
    DSP_params *dsp_params;

    FFT_CALCER(DSP_params *new_dsp_params = nullptr);
    ~FFT_CALCER();

    void recalc_noise_level();

public slots:
    void get_fft_step(Ipp32fc *cell = nullptr);

signals:
    void paint_fft(float *fftRes);
};

#endif // FFT_CALCER_H
