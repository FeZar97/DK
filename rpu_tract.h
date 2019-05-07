#ifndef RPU_TRACT_H
#define RPU_TRACT_H

#include <definitions.h>

class RPU_tract : public QObject
{
    Q_OBJECT
private:
    LPT_ADDRESS lpt;             // адрес LPT порта

    RPU_CONFIG  config;          // конфигурация приемника
    NUMERALS    index;           // номер тракта

public:
    int         central_freq;    // центральная частота настройки

private:
    bool        in_att_idx;      // состояние входного аттенюатора
    int         hf_att_idx;      // значение аттенюатора ВЧ
    int         if_att_idx;      // значение аттенюатора ПЧ
    IF_BAND     if_band_idx;     // ширина полосы ПЧ

    bool        is_tract_active; // активность тракта

    void        set_config_to_RPU();       // отправка КОДА УПРАВЛЕНИЯ РЕЖИМАМИ РАБОТЫ
    void        set_central_freq_to_RPU(int presNb, int freq); // отправка КОДА УПРАВЛЕНИЯ ПЕРВЫМ ГЕТЕРОДИНОМ
    void        set_tract_params_to_RPU(int presNb); // запись всех параметров

public:
    RPU_tract();
    ~RPU_tract();

    lpOutc      gfpOut_char;
    void        send_code(unsigned short addr, unsigned char code); // функция для отправки команд через LPT

    void        set_default_tract_params(); // установка настроек по умолчанию
    void        print_tract_info(); // вывод информации о тракте

    void        set_lpt(LPT_ADDRESS new_lpt); // изменение адреса LPT порта
    LPT_ADDRESS get_lpt(); // возвращает адрес текущего LPT порта

    void        set_config(RPU_CONFIG new_config); // изменение типа тракта
    RPU_CONFIG  get_config(); // возвращает тип тракта

    void        set_index(NUMERALS new_index); // изменение номера тракта
    NUMERALS    get_index(); // возвращает номера тракта

    void        set_central_freq(int new_central_freq); // изменение частоты настройки тракта
    int         get_central_freq(); // возвращает текущее значение частоты настройки тракта

    void        set_in_att_idx(bool new_in_att_idx); // изменение значения входного аттенюатора
    bool        get_in_att_idx(); // возвращает текущее значение входного аттенюатора

    void        set_hf_att_idx(int new_hf_att_idx); // изменение значения аттенюатора ВЧ
    int         get_hf_att_idx(); // возвращает индекс текущего значения ВЧ аттенюатора

    void        set_if_att_idx(int new_if_att_idx); // изменение значения аттенюатора ПЧ
    int         get_if_att_idx(); // возвращает индекс текущего значения ПЧ аттенюатора

    void        set_if_band_idx(int new_if_band_idx); // изменение значения ширины полосы ПЧ
    int         get_if_band_idx(); // возвращает текущую полосу ПЧ

    void        set_tract_active(bool new_state); // обновление активности трактов
    bool        get_tract_active(); // возвращает TRUE если окно активно в данном режима работы, FALSE иначе

    void        send_tract_settings_to_RPU(int presNb = -1, int freq = RPU_DEFAULT_TRACT_FREQ); // отправка в РПУ кодограмм с настройками

signals:
    void        global_update_interface();
};

#endif // RPU_TRACT_H
