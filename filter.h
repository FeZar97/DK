#ifndef FILTER_H
#define FILTER_H

#include <definitions.h>

class FILTER : public QObject
{
    Q_OBJECT

public:
    SDR_params *sdr_params;
    DSP_params *dsp_params;

    FILTER(SDR_params *new_sdr_params = nullptr, DSP_params *new_dsp_params = nullptr);
    ~FILTER();

public slots:
    void get_filtration_step(Ipp32fc *cell = nullptr);

signals:
    void get_fft_step(Ipp32fc *cell);   // БПФ

    void get_shift_step(Ipp32fc *cell); // частотный сдвиг

    void write_to_file_32f(Ipp32f *cell, int cell_size); // запись в файл
};

#endif // FILTER_H
