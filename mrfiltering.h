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
    void get_fft_step(Ipp32fc *rb_cell);
};

#endif // MRFILTERING_H
