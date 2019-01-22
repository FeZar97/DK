#ifndef MRFILTERING_H
#define MRFILTERING_H

#include <definitions.h>

class MRFiltering : public QObject
{
    Q_OBJECT

public:
    SDR_params *sdr_params;
    DSP_params *dsp_params;

    MRFiltering(DSP_params *new_dsp_params = nullptr, SDR_params *new_sdr_params = nullptr);

    ~MRFiltering();

public slots:
    void get_filtration_step(Ipp32fc *rb_cell = nullptr);

signals:
    void write_to_file(Ipp32fc *cell); // запись в файл

    void get_fft_step(Ipp32fc *rb_cell, unsigned int cell_size); // БПФ

    void get_shift_step(Ipp32fc *rb_cell, unsigned int cell_size); // частотный сдвиг

    void get_sound_step(Ipp32fc *rb_cell, unsigned int cell_size); // вывод звука
};

#endif // MRFILTERING_H
