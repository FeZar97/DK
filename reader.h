#ifndef READER_H
#define READER_H

#include <definitions.h>

class READER : public QObject
{
    Q_OBJECT

public:
    SDR_params *sdr_params;
    DSP_params *dsp_params;

    READER(DSP_params *new_dsp_params = NULL, SDR_params *new_sdr_params = NULL);
    ~READER();


public slots:
    // сигнал постпуает от кнопки начала записи
    void start_reading();

signals:
    // обновление прогрес бара чтения
    void update_ReadProgressBar(int val);

    // отправка сигнала-запроса на фильтрацию порции
    void get_filtration_step(Ipp32fc *rb_cell);

    // отправка сигнала на запись сырого ЦП
    void write_to_file(Ipp32fc *rb_cell);

    // сдвиг спектра
    void get_shift_step(Ipp32fc *rb_cell, unsigned int cell_size);

    // если завершилось штатно, то флаг emergency_end_recording == 0
    // если завершилось экстренно, то emergency_end_recording == 1
    void end_of_recording(bool status);
};

#endif // READER_H
