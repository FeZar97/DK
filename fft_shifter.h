#ifndef FFT_SHIFTER_H
#define FFT_SHIFTER_H

#include <definitions.h>

class fft_shifter : public QObject
{
    Q_OBJECT
public:
    SDR_params *sdr_params;
    DSP_params *dsp_params;

    explicit fft_shifter(DSP_params *new_dsp_params = nullptr, SDR_params *new_sdr_params = nullptr);
    ~fft_shifter();

public slots:
    void get_shift_step(Ipp32fc *rb_cell = nullptr, unsigned int cell_size = 0);

signals:
    void get_fft_step(Ipp32fc *shift_cell, unsigned int cell_size); // БПФ

    void write_to_file(Ipp32fc *shift_rb_cell); // запись в файл

    void get_sound_step(Ipp32fc *rb_cell, unsigned int cell_size); // вывод звука
};

#endif // FFT_SHIFTER_H
