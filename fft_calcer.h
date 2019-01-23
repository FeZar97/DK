#ifndef FFT_CALCER_H
#define FFT_CALCER_H

#include <definitions.h>

class fft_calcer : public QObject
{
    Q_OBJECT
public:
    DSP_params *dsp_params;

    explicit fft_calcer(DSP_params *new_dsp_params = nullptr);
    ~fft_calcer();

public slots:
    void get_fft_step(Ipp32fc *rb_cell = nullptr, unsigned int cell_size = 0);

signals:
    void paint_fft();

};

#endif // FFT_CALCER_H
