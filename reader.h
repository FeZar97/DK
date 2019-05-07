#ifndef READER_H
#define READER_H

#include <definitions.h>

class READER : public QObject
{
    Q_OBJECT

public:
    SDR_params *sdr_params;
    DSP_params *dsp_params;

    READER(SDR_params *new_sdr_params = nullptr, DSP_params *new_dsp_params = nullptr);
    ~READER();

public slots:
    void start_reading();
    void get_one_read_step();

signals:
    void get_compenaste_step(Ipp32fc *cell);

    void get_fft_step(Ipp32fc *cell); // БПФ

    void get_filtration_step(Ipp32fc *cell); // фильтрация

    void write_to_file_32f(Ipp32f *cell, int cell_size); // запись в файл
};

#endif // READER_H
