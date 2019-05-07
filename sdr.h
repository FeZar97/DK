#ifndef SDR_H
#define SDR_H

#include <definitions.h>

class SDR : public QObject
{
    Q_OBJECT

public:
    SDR();
    ~SDR();

    SDR_params *sdr_params;

    int get_sdr_count(); // возвращает кол-во доступных приемников

    void set_gain_idx(int new_gain_idx); // смена КУ тюнера
    void set_rtl_agc(bool new_rtl_agc); // усиление rtl чипа
    void set_sample_rate(int new_sample_rate); // смена частоты дискретизации

    int set_params_to_device(); // конфигурация приемника
    QString get_device_params(); // возвращает настройки, высталвенные на приемнике

    void read_sdr_info(); // считывание инфы о приемнике из EEPROM

public slots:
    void set_central_freq(int new_central_freq); // смена частоты настройки

signals:
    void recalc_dsp_params(); // пересчет параметров в DSP
    void update_interface();
};

#endif // SDR_H
