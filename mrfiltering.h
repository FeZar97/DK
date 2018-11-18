#ifndef MRFILTERING_H
#define MRFILTERING_H

#include <definitions.h>

class MRFiltering : public QObject
{
    Q_OBJECT

public:
    SDR_params *sdr_params;
    DSP_params *dsp_params;

    MRFiltering(DSP_params *new_dsp_params = NULL, SDR_params *new_sdr_params = NULL);

    ~MRFiltering();

public slots:
    // слот фильтрации
    void get_filtration_step(Ipp32fc *rb_cell = NULL);

signals:
    // запись в файл
    void write_to_file(Ipp32fc *cell);
    // сигнал для расчета спектра
    void get_fft_step(Ipp32fc *rb_cell, unsigned int cell_size);
    // сигнал для частотного сдвига
    void get_shift_step(Ipp32fc *rb_cell, unsigned int cell_size);
    // сигнал для вывода
    void get_sound_step(Ipp32fc *rb_cell, unsigned int cell_size);
};

#endif // MRFILTERING_H
