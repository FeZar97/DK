#ifndef RPU_H
#define RPU_H

#include <rpu_tract.h>
#include <rpu_kalibrator.h>

class RPU : public QObject
{
    Q_OBJECT

public:
    RPU();
    ~RPU();

    LPT_ADDRESS         lpt;                     // адрес используемого LPT порта
    RPU_CONFIG          config;                  // конфигурация трактов

    RPU_tract           first_tract;             // первый тракт
    RPU_tract           second_tract;            // второй тракт
    RPU_tract           third_tract;             // третий тракт
    RPU_tract           fourth_tract;            // четвертый тракт

    RPU_kalibrator      kalibrator;              // калибратор

    lpOutc              gfpOut_char; // функция для отправки команд через LPT
    void                send_code(unsigned short addr, unsigned char code);

    void                set_lpt_idx(int new_lpt_idx); // изменение адреса LPT порта
    unsigned int        get_lpt_idx(); // возвращает индекс LPT порта
    unsigned int        get_lpt_addres(); // возвращает адрес LPT порта

    void                set_config(RPU_CONFIG new_config); // изменение конфигурации РПУ
    unsigned int        get_config_idx(); // возвращает индекс конфигурации РПУ

    void                set_config_to_RPU(); // кодограмма управления режимами работы
};

#endif // RPU_H
