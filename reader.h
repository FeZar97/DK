#ifndef READER_H
#define READER_H

#include <definitions.h>

class READER : public QObject
{
    Q_OBJECT

public:
    SDR_params *sdr_params;
    DSP_params *dsp_params;

    READER(DSP_params *new_dsp_params = nullptr, SDR_params *new_sdr_params = nullptr);
    ~READER();

public slots:
    void start_reading();

signals:
    void get_fft_step(Ipp32fc *cell, unsigned int cell_size); // БПФ

    void get_filtration_step(Ipp32fc *cell, unsigned int cell_size); // фильтрация

    void write_to_file(Ipp32fc *rb_cell); // запись в файл

    // если завершилось штатно, то флаг emergency_end_recording == 0
    // если завершилось экстренно, то emergency_end_recording == 1
    void end_of_recording(bool status);

    void get_sound_step(Ipp32fc *rb_cell, unsigned int cell_size); // вывод звука
};

#endif // READER_H
