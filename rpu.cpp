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

#include "rpu.h"

// при созднии объекта класса RPU порт управления по умолчанию 0x378, конфигурация - 4 канала
RPU::RPU()
{
    // подтягиваем функцию
    HINSTANCE hinstLib = LoadLibrary(TEXT("inpout32.dll"));
    gfpOut_char = lpOutc(GetProcAddress(hinstLib, "DlPortWritePortUchar"));

    // перед конфигурацией надо раздать номера
    first_tract.set_index(FIRST);
    second_tract.set_index(SECOND);
    third_tract.set_index(THIRD);
    fourth_tract.set_index(FOURTH);

    set_lpt_idx(0);
    set_config(RPU_DEFAULT_CONFIG);

    first_tract.send_tract_settings_to_RPU();
    second_tract.send_tract_settings_to_RPU();
    third_tract.send_tract_settings_to_RPU();
    fourth_tract.send_tract_settings_to_RPU();
}

RPU::~RPU()
{
}

void RPU::send_code(unsigned short addr, unsigned char code)
{
    gfpOut_char(addr, code);
}

// изменение адреса LPT порта
void RPU::set_lpt_idx(int new_lpt_idx)
{
    switch(new_lpt_idx){
        case 0:
            lpt = LPT_1;
            break;
        case 1:
            lpt = LPT_2;
            break;
        case 2:
            lpt = LPT_3;
            break;
        case 3:
            lpt = LPT_4;
            break;
        case 4:
            lpt = LPT_5;
            break;
    }

    first_tract.set_lpt(lpt);
    second_tract.set_lpt(lpt);
    third_tract.set_lpt(lpt);
    fourth_tract.set_lpt(lpt);

    kalibrator.set_lpt(lpt);
}

// возвращает индекс LPT порта
int RPU::get_lpt_idx()
{
    switch(lpt){
        case LPT_1:
            return 0;
        case LPT_2:
            return 1;
        case LPT_3:
            return 2;
        case LPT_4:
            return 3;
        case LPT_5:
            return 4;
    }
    return 0;
}

int RPU::get_lpt_addres()
{
    return lpt;
}

// изменение конфигурации РПУ и определение активности трактов
void RPU::set_config(RPU_CONFIG new_config)
{
    config = new_config;

    first_tract.set_config(config);
    second_tract.set_config(config);
    third_tract.set_config(config);
    fourth_tract.set_config(config);

    switch(config){
        case FOUR_CHANNEL:
            first_tract.set_tract_active(true);
            second_tract.set_tract_active(false);
            third_tract.set_tract_active(false);
            fourth_tract.set_tract_active(false);
            break;
        case THREE_ONE_CHANNEL:
        case TWO_TWO_CHANNEL:
            first_tract.set_tract_active(true);
            second_tract.set_tract_active(true);
            third_tract.set_tract_active(false);
            fourth_tract.set_tract_active(false);
            break;
        case TWO_ONE_ONE_CHANNEL:
            first_tract.set_tract_active(true);
            second_tract.set_tract_active(true);
            third_tract.set_tract_active(true);
            fourth_tract.set_tract_active(false);
            break;
        case ONE_ONE_ONE_ONE_CHANNEL:
            first_tract.set_tract_active(true);
            second_tract.set_tract_active(true);
            third_tract.set_tract_active(true);
            fourth_tract.set_tract_active(true);
            break;
    }

    set_config_to_RPU();
}

// возвращает индекс конфигурации РПУ
int RPU::get_config_idx()
{
    switch(config){
        case FOUR_CHANNEL:
            return 0;

        case THREE_ONE_CHANNEL:
            return 1;

        case TWO_TWO_CHANNEL:
            return 2;

        case TWO_ONE_ONE_CHANNEL:
            return 3;

        case ONE_ONE_ONE_ONE_CHANNEL:
            return 4;
    }
    return -1;
}

int RPU::get_tract_number()
{
    switch(config){
        case FOUR_CHANNEL:
            return 1;

        case THREE_ONE_CHANNEL:
        case TWO_TWO_CHANNEL:
            return 2;

        case TWO_ONE_ONE_CHANNEL:
            return 3;

        case ONE_ONE_ONE_ONE_CHANNEL:
            return 4;
    }
    return 0;
}

// кодограмма управления режимами работы
void RPU::set_config_to_RPU()
{
    //QMessageBox::information(NULL, "rpu", "set_config_to_RPU");

    // байтовый код, который будет отправлен в порт
    byte OperationMode = 0x00;

    // в зависимости от выбранного режима определяется код режима работы
    switch(config)
    {
        case FOUR_CHANNEL:              // конфигурация 4
                                        // .   .   .   R3  R2  R1  N2  N1
            OperationMode = 0x0C;       // 0   0   0   0   1   1   0   0 = 0x0C
            break;

        case THREE_ONE_CHANNEL:         // конфигурация 3 + 1
                                        // .   .   .   R3  R2  R1  N2  N1
            OperationMode = 0x08;       // 0   0   0   0   1   0   0   0 = 0x08
            break;

        case TWO_TWO_CHANNEL:           // конфигурация 2 + 2
                                        // .   .   .   R3  R2  R1  N2  N1
            OperationMode = 0x10;       // 0   0   0   1   0   0   0   0 = 0x10
            break;

        case TWO_ONE_ONE_CHANNEL:       // конфигурация 2 + 1 + 1
                                        // .   .   .   R3  R2  R1  N2  N1
            OperationMode = 0x04;       // 0   0   0   0   0   1   0   0 = 0x04
            break;

        case ONE_ONE_ONE_ONE_CHANNEL:   // конфигурация 1 + 1 + 1 + 1
                                        // .   .   .   R3  R2  R1  N2  N1
            OperationMode = 0x00;       // 0   0   0   0   0   0   0   0 = 0x00
            break;
    }

    // отправка кода
    send_code(lpt + LPT_DATA_REG, OperationMode);

    // СТРОБ ( 5 --- 4)
    send_code(lpt + LPT_CONTROL_REG, 0x4); // 0 1 0 0
    send_code(lpt + LPT_CONTROL_REG, 0x5); // 0 1 0 1
    send_code(lpt + LPT_CONTROL_REG, 0x4); // 0 1 0 0
}
