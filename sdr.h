#ifndef SDR_H
#define SDR_H

#include <definitions.h>

class SDR : public QObject
{
    Q_OBJECT

public:

    SDR_params *sdr_params;
    SDR();

    int get_sdr_count(); // возвращает кол-во доступных приемников

    void set_central_freq(unsigned int new_central_freq); // смена частоты настройки
    void set_gain_idx(unsigned int new_gain_idx); // смена КУ
    void set_sample_rate(unsigned int new_sample_rate); // смена частоты дискретизации

    bool set_params_to_device(); // конфигурация приемника
    QString get_device_params(); // возвращает настройки, высталвенные на приемнике

signals:
    // сигнал на пеерсчет параметров в DSP
    void recalc_dsp_params();

    void global_update_interface();
};

#endif // SDR_H
