#ifndef RPU_TRACT_H
#define RPU_TRACT_H

#include "definitions.h"

class RPU_tract : public QObject
{
    Q_OBJECT
private:
    LPT_ADDRESS         lpt;                   // адрес LPT порта

    RPU_CONFIG          config;                // конфигурация приемника
    NUMERALS            index;                 // номер тракта

    unsigned int        central_freq;          // центральная частота настройки
    bool                in_att_idx;            // состояние входного аттенюатора
    unsigned short      hf_att_idx;            // значение аттенюатора ВЧ
    unsigned short      if_att_idx;            // значение аттенюатора ПЧ
    IF_BAND             if_band_idx;           // ширина полосы ПЧ

    bool                is_tract_active;       // активность тракта

/// КОДОГРАММЫ
    // отправка КОДА УПРАВЛЕНИЯ РЕЖИМАМИ РАБОТЫ
    void                set_config_to_RPU();

    // отправка КОДА УПРАВЛЕНИЯ ПЕРВЫМ ГЕТЕРОДИНОМ
    void                set_central_freq_to_RPU();

    // запись всех параметров
    void                set_tract_params_to_RPU();

public:
    RPU_tract();
    ~RPU_tract();

    // функция для отправки команд через LPT
    lpOutc             gfpOut_char;
    void               send_code(unsigned short addr, unsigned char code);

/// ВСПОМОГАТЕЛЬНЫЕ
    // установка настроек по умолчанию
    void              set_default_tract_params();
    // вывод информации о тракте
    void              print_tract_info();

/// ИНТЕРФЕЙС
    // изменение адреса LPT порта
    void              set_lpt(LPT_ADDRESS new_lpt);
    // возвращает адрес текущего LPT порта
    LPT_ADDRESS       get_lpt();

    // изменение типа тракта
    void              set_config(RPU_CONFIG new_config);
    // возвращает тип тракта
    RPU_CONFIG        get_config();

    // изменение номера тракта
    void              set_index(NUMERALS new_index);
    // возвращает номера тракта
    NUMERALS          get_index();

    // изменение частоты настройки тракта
    void              set_central_freq(unsigned int new_central_freq);
    // возвращает текущее значение частоты настройки тракта
    unsigned int      get_central_freq();

    // изменение значения входного аттенюатора
    void              set_in_att_idx(bool new_in_att_idx);
    // возвращает текущее значение входного аттенюатора
    bool              get_in_att_idx();

    // изменение значения аттенюатора ВЧ
    void              set_hf_att_idx(short  new_hf_att_idx);
    // возвращает индекс текущего значения ВЧ аттенюатора
    unsigned short    get_hf_att_idx();

    // изменение значения аттенюатора ПЧ
    void              set_if_att_idx(short new_if_att_idx);
    // возвращает индекс текущего значения ПЧ аттенюатора
    unsigned short    get_if_att_idx();

    // изменение значения ширины полосы ПЧ
    void              set_if_band_idx(short new_if_band_idx);
    // возвращает текущую полосу ПЧ
    unsigned short    get_if_band_idx();

    // в зависимости от режима работы РПУ конкретное окно тракта может быть как активным, так и не активным
    void              set_tract_active(bool new_state);
    // возвращает TRUE если окно активно в данном режима работы, FALSE иначе
    bool              get_tract_active();

    // тестировка преселекторов
    void              test_preselectors();
/// КОДОГРАММЫ
    // отправка в РПУ кодограмм с настройками
    void              send_tract_settings_to_RPU();

signals:
    void              global_update_interface();
};

#endif // RPU_TRACT_H
