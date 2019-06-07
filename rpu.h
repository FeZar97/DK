/*
    This file is part of DigitalKalmar(Кальмар-SDR)

    DigitalKalmar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DigitalKalmar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DigitalKalmar.  If not, see <https://www.gnu.org/licenses/>.
*/

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

    LPT_ADDRESS    lpt;                               // адрес используемого LPT порта
    RPU_CONFIG     config;                            // конфигурация трактов

    RPU_tract      first_tract;                       // первый тракт
    RPU_tract      second_tract;                      // второй тракт
    RPU_tract      third_tract;                       // третий тракт
    RPU_tract      fourth_tract;                      // четвертый тракт

    RPU_kalibrator kalibrator;                        // калибратор

    lpOutc         gfpOut_char;                       // функция для отправки команд через LPT
    void           send_code(unsigned short addr,
                             unsigned char code);

    void           set_lpt_idx(int new_lpt_idx);      // изменение адреса LPT порта
    int            get_lpt_idx();                     // возвращает индекс LPT порта
    int            get_lpt_addres();                  // возвращает адрес LPT порта

    void           set_config(RPU_CONFIG new_config); // изменение конфигурации РПУ
    int            get_config_idx();                  // возвращает индекс конфигурации РПУ
    int            get_tract_number();                // возвращает кол-во активных трактов для данного конфига РПУ

    void           set_config_to_RPU();               // кодограмма управления режимами работы
};

#endif // RPU_H
