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

#include "rpu_tract.h"

RPU_tract::RPU_tract()
{
    HINSTANCE hinstLib  = LoadLibrary(TEXT("inpout32.dll"));
    gfpOut_char         = lpOutc(GetProcAddress(hinstLib, "DlPortWritePortUchar"));

    set_default_tract_params();
}

void RPU_tract::send_code(unsigned short addr, unsigned char code)
{
    gfpOut_char(addr, code);
}

RPU_tract::~RPU_tract()
{

}

// установка настроек по умолчанию
void RPU_tract::set_default_tract_params()
{
    set_lpt(RPU_DEFAULT_LPT);                 // LPT 1

    set_config(RPU_DEFAULT_CONFIG);           // 4 канала в первом тракте

    set_central_freq(RPU_DEFAULT_TRACT_FREQ); // 15 МГц
    set_in_att_idx(RPU_DEFAULT_IN_ATT_IDX);   // OFF
    set_hf_att_idx(RPU_DEFAULT_HF_ATT_IDX);   // OFF
    set_if_att_idx(RPU_DEFAULT_IF_ATT_IDX);   // OFF
    set_if_band_idx(RPU_DEFAULT_IF_BAND_IDX); // 3 кГц

    set_tract_active(false);
}

// вывод информации о тракте
void RPU_tract::print_tract_info()
{
    //QString tract_info = create_WindowTitle() + "\n";
    QString tract_info = "";

    tract_info += "central_freq = " + QString::number(get_central_freq()) + " Гц, "
               + "АттВх = " + (get_in_att_idx() ? "ON " : "OFF ")
               + "АттВЧ = " + (get_hf_att_idx() ? QString::number(get_hf_att_idx() * RPU_HF_ATT_STEP) + " дБ " : "OFF ")
               + "АттПЧ = " + (get_if_att_idx() ? QString::number(get_if_att_idx() * RPU_IF_ATT_STEP) + " дБ " : "OFF ");

    tract_info += "Полоса = ";
    switch(get_if_band_idx())
    {
        case ZERO:
            tract_info += "3 кГц, ";
            break;
        case FIRST:
            tract_info += "8 кГц, ";
            break;
        case SECOND:
            tract_info += "20 кГц, ";
            break;
        case THIRD:
            tract_info += "150 кГц, ";
            break;
        default:
            break;
    }

    tract_info += "Порт = " + QString::number(get_lpt(), 16);
}

// изменение адреса LPT порта
void RPU_tract::set_lpt(LPT_ADDRESS new_lpt)
{
    lpt = new_lpt;
}

// возвращает адрес текущего LPT порта
LPT_ADDRESS RPU_tract::get_lpt()
{
    return lpt;
}

// изменение типа тракта
void RPU_tract::set_config(RPU_CONFIG new_config)
{
    config = new_config;
}

// возвращает тип тракта
RPU_CONFIG RPU_tract::get_config()
{
    return config;
}

// изменение номера тракта
void RPU_tract::set_index(NUMERALS new_index)
{
    index = new_index;
}

NUMERALS RPU_tract::get_index()
{
    return index;
}

// изменение частоты настройки тракта
void RPU_tract::set_central_freq(int new_central_freq)
{
    // изменения происодят только если новая частота удовлетворяет ДРЧ
    if(new_central_freq >= RPU_MIN_TRACT_FREQ && new_central_freq <= RPU_MAX_TRACT_FREQ){
        central_freq = new_central_freq;
    }else{
        if(get_central_freq() == RPU_MIN_TRACT_FREQ || get_central_freq() == RPU_MAX_TRACT_FREQ){
            if(new_central_freq < RPU_MIN_TRACT_FREQ){
                QMessageBox::information(nullptr, NAME, "Достигнута минимальная частота настройки РПУ Кальмар.");
            }else{
                QMessageBox::information(nullptr, NAME, "Достигнута максимальная частота настройки РПУ Кальмар.");
            }
        }else{
            if(new_central_freq < RPU_MIN_TRACT_FREQ){
                central_freq = RPU_MIN_TRACT_FREQ;
            }
            if(new_central_freq > RPU_MAX_TRACT_FREQ){
                central_freq = RPU_MAX_TRACT_FREQ;
            }
        }
    }

    send_tract_settings_to_RPU();
    //set_central_freq_to_RPU();
    //set_tract_params_to_RPU(-1);

    emit global_update_interface();
}

// возвращает текущее значение частоты настройки тракта
int RPU_tract::get_central_freq()
{
    return central_freq;
}

// изменение значения входного аттенюатора
void RPU_tract::set_in_att_idx(bool new_in_att_idx)
{
    if(get_in_att_idx() != new_in_att_idx){
        in_att_idx = new_in_att_idx;
        send_tract_settings_to_RPU();
    }
}

// возвращает текущее значение входного аттенюатора
bool RPU_tract::get_in_att_idx()
{
    return in_att_idx;
}

// изменение значения аттенюатора ВЧ
void RPU_tract::set_hf_att_idx(int new_hf_att_idx)
{
    if(new_hf_att_idx < RPU_MIN_HF_ATT_IDX || new_hf_att_idx > RPU_MAX_HF_ATT_IDX){
        new_hf_att_idx = RPU_DEFAULT_HF_ATT_IDX;
    }

    if(get_hf_att_idx() != new_hf_att_idx){
        hf_att_idx = ushort(new_hf_att_idx);
        send_tract_settings_to_RPU();
    }
}

// возвращает индекс текущего значения ВЧ аттенюатора
int RPU_tract::get_hf_att_idx()
{
    return hf_att_idx;
}

// изменение значения аттенюатора ПЧ
void RPU_tract::set_if_att_idx(int new_if_att_idx)
{
    if(new_if_att_idx < RPU_MIN_IF_ATT_IDX || new_if_att_idx > RPU_MAX_IF_ATT_IDX){
        new_if_att_idx = RPU_DEFAULT_IF_ATT_IDX;
    }

    if(get_if_att_idx() != new_if_att_idx){
        if_att_idx = ushort(new_if_att_idx);
        send_tract_settings_to_RPU();
    }
}

// возвращает индекс текущего значения ПЧ аттенюатора
int RPU_tract::get_if_att_idx()
{
    return if_att_idx;
}

// изменение значения ширины полосы ПЧ
void RPU_tract::set_if_band_idx(int new_if_band_idx)
{
    if(new_if_band_idx < RPU_MIN_IF_BAND_IDX || new_if_band_idx > RPU_MAX_IF_BAND_IDX){
        new_if_band_idx = RPU_DEFAULT_IF_BAND_IDX;
    }

    if(if_band_idx != new_if_band_idx){
        if_band_idx = IF_BAND(new_if_band_idx);
        send_tract_settings_to_RPU();
    }
}

// возвращает текущую полосу ПЧ
int RPU_tract::get_if_band_idx()
{
    return if_band_idx;
}

// в зависимости от режима работы РПУ конкретное окно тракта может быть как активным, так и не активным
void RPU_tract::set_tract_active(bool new_state)
{
    is_tract_active = new_state;
}

// возвращает true если тракт активен в данном режиме работы
bool RPU_tract::get_tract_active()
{
    return is_tract_active;
}

// отправка в РПУ кодограмм с настройками
void RPU_tract::send_tract_settings_to_RPU(int presNb, int freq)
{
    set_central_freq_to_RPU(presNb, freq);
    set_tract_params_to_RPU(presNb);
}

/*---------------------------------КОД УПРАВЛЕНИЯ РЕЖИМАМИ РАБОТЫ---------------------------------

R1 ,R2, R3 – код управления режимами работы
+----+----+----+----------------------------------------+-----------------+
| R3 | R2 | R1 |                Режим работы            |  Ведущий канал  |
+----+----+----+----------------------------------------+-----------------+
| 0	 | 0  | 0  |          Четыре автономных канала      |       Нет       |
+----+----+----+----------------------------------------+-----------------+
| 0	 | 0  | 1  | Два синхронных и два автономных канала	|      Второй     |
+----+----+----+----------------------------------------+-----------------+
| 0	 | 1  |	0  | Три синхронных и один автономный канал	|      Третий     |
+----+----+----+----------------------------------------+-----------------+
| 0	 | 1  |	1  |        Четыре синхронных канала        |    Четвертый    |
+----+----+----+----------------------------------------+-----------------+
| 1  | 0  |	0  |      Две пары  синхронных каналов      | Первый и третий |
+----+----+----+----------------------------------------+-----------------+


N1, N2 – код управляемого канала (для автономных каналов)
+----+----+-------------------+
| N2 | N1 |	Управляемый канал |
+----+----+-------------------+
| 0  | 0  |       Первый      |
+----+----+-------------------+
| 0  | 1  |       Второй      |
+----+----+-------------------+
| 1  | 0  |       Третий      |
+----+----+-------------------+
| 1  | 1  |      Четвертый    |
+----+----+-------------------+

При  синхронной работе каналов управление необходимо осуществлять только ведущим каналом,
на остальные оно поступает автоматически.

1.    Алгоритм определения кодограммы:
      1) в соответствии с режимом работы из таблицы_1 берутся значения разрядов R3, R2 и R1;
      2) из таблицы_1 берется номер ВЕДУЩЕГО канала для СИНХРОННЫХ каналов;
      3) для данного номера ВЕДУЩЕГО канала по таблице_2 определяются значения разрядов N2 и N1;
      4) если в данном режиме работы предусмотрены АВТОНОМНЫЕ каналы, то им ставятся в соответствие
         УПРАВЛЯЮЩИЕ каналы, которые остались не занятыми в пункте 2.

2. ВЕДУЩИЙ канал - это канал, управляющий группой синхронных каналов (например в режиме 3+1 будет два управляющих канала: третий канал - ведущий для трех каналов и
*/
void RPU_tract::set_config_to_RPU()
{
    int code = 0, geterodinNumber = 0;

    if(config < RPU_MIN_CONFIG || config > RPU_MAX_CONFIG)
        code = 0xFF;

    switch(config)
    {
        //                       R3 R2 R1 N2 N1
        // 4 синхронных канала -  0  1  1  1  1 (все в соответствии с инструкцией)
        //                        0x0C(00001100)
        //                      + 0x03(00000011)
        //                      = 0x0F(00001111)
        case FOUR_CHANNEL: // 4
            geterodinNumber = 0x03; // по таблице_1 ВЕДУЩИМ каналом является четвертый
            code = 0x0C | geterodinNumber;
            break;

        //                                          R3 R2 R1 N2 N1
        // Три синхронных и один автономный канал -  0  1  0  1  0
        //                                           0x08(00001000)
        //                                         + 0x02(00000010)
        //                                         = 0x0A(00001010)
        case THREE_ONE_CHANNEL: // 3+1
            if(index == FIRST) // если это трехканальный тракт, то ВЕДУЩИМ каналом должен быть третий канал
                geterodinNumber = 0x02;
            else // для оставшегося АВТОНОМНОГО канала берется гетеродин 0x03
                geterodinNumber = 0x03;
            code = 0x08 | geterodinNumber;
            break;

        //                               R3 R2 R1 N2 N1          R3 R2 R1 N2 N1
        // Две пары синхронных каналов -  1  0  0  0  0           1  0  0  1  0
        //                                0x10(00010000)          0x10(00010000)
        //                              + 0x00(00000000)    и   + 0x02(00000010)
        //                              = 0x10(00010000)        = 0x12(00010010)
        case TWO_TWO_CHANNEL: // 2+2
            if(index == FIRST) // если это первая синхронная пара, то ВЕДУЩИМ каналом должен быть первый канал
                geterodinNumber = 0x00;
            else                 // если это вторая синхронная пара, то ВЕДУЩИМ каналом должен быть третий канал
                geterodinNumber = 0x02;
            code = 0x10 | geterodinNumber;
            break;

        //                                          R3 R2 R1 N2 N1
        // Два синхронных и два автономных канала -  0  0  1  0  1
        //                                           0x04(00000100)
        //                                         + 0x01(00000001)
        //                                         = 0x05(00000101)
        case TWO_ONE_ONE_CHANNEL: // 2+1+1
            if( index == FIRST ) // если это первый (двухканальный синхронный) тракт, то ВЕДУЩИМ каналом должен быть второй канал
               geterodinNumber = 0x01;
            else if( index == SECOND ) // если это второй тракт (первый из АВТОНОМНЫХ каналов), то УПРАВЛЯЮЩИМ каналом будет первый канал
               geterodinNumber = 0x00;
            else // если это третий тракт (второй из АВТОНОМНЫХ каналов), то УПРАВЛЯЮЩИМ каналом будет третий канал
               geterodinNumber = 0x02;
            code = 0x04 | geterodinNumber;
            break;

        //                            R3 R2 R1 N2 N1
        // Четыре автономных канала -  0  0  0  Х  Х
        //                             0x00(00000000)
        //                           + 0x0Х(000000ХХ)
        //                           = 0x0Х(000000ХХ)
        case  ONE_ONE_ONE_ONE_CHANNEL: // 1 + 1 + 1 + 1
            if( index == FIRST )      // если это первый тракт, то УПРАВЛЯЮЩИМ каналом будет первый канал
               geterodinNumber = 0x00;
            else if( index == SECOND ) // если это второй тракт, то УПРАВЛЯЮЩИМ каналом будет второй канал
               geterodinNumber = 0x01;
            else if( index == THIRD ) // если это третий тракт, то УПРАВЛЯЮЩИМ каналом будет третий канал
               geterodinNumber = 0x02;
            else                        // если это четвертый тракт, то УПРАВЛЯЮЩИМ каналом будет четвертый канал
                geterodinNumber = 0x03;
            code = geterodinNumber;
            break;
    }

    if(code != 0xFF){
        // комбинации из предыдущей функции ( R3 - R1 определяют режим трактов, N2 и N1 - код управляемго канала )
        // для 4 синхронных каналов код: Х Х Х R3 R2 R1 N2 N1
        //                               Х Х Х  0  1  1  1  1
        // с таким кодом  будут задействованы контакты DB25: 2, 3, 4, 5 => эквивалентное число:
        //                                                                 2(N1) + 4 + 8 + 16 + 0 = 30

        // код
        send_code(lpt + LPT_DATA_REG, byte(code));

        // строб по адресу 010 ( с учетом инверсии сигналов DB25)
        send_code(lpt + LPT_CONTROL_REG, 0x4);
        send_code(lpt + LPT_CONTROL_REG, 0x5);
        send_code(lpt + LPT_CONTROL_REG, 0x4);
    }
}

/*---------------------------------КОД УПРАВЛЕНИЯ ПЕРВЫМ ГЕТЕРОДИНОМ---------------------------------

 D1...D3  - 32-ти  разрядный последовательный код частоты настройки блока первого гетеродина записывается в
 регистры 3-х БИС интегральных синтезаторов частот КФ1015ПЛ4.
 Первые 13 разрядов кода управляют делителем опорного сигнала ОД,
 следующие 18 разрядов управляют основным счетчиком  N, а 32-й разряд изменяет коэффициент деления
 предварительного делителя основного счетчика P.
 Запись информации в БИС осуществляется начиная с первого разряда.

 Формат управляющего слова:
   +----+----+---+----+-----+-----+-----------+----------+
   | 32 | 31 |30 | 29 | 28* | 27* | 26 ... 14 | 13 ... 1 |
   +----+----+---+----+-----+-----+-----------+----------+
   | P  |                    N                |   ОД     |
   +----+-------------------------------------+----------+

    * при P=1 игнорируются. (27 и 28 разряды)

 Информационные разряды D1...D3 сопровождаются импульсами синхронизации С1. Запись информации во входной регистр
 БИС осуществляется по заднему фронту синхроимпульсов. После окончания записи информации во входной регистр БИС
 производится ее перезапись в рабочий регистр одиночным импульсом Т положительной полярности.

 Значения опорных делителей и признаков коэффициентов деления предварительных делителей постоянные:
      OД1 = 100,  P1 = 0;
      OД2 = 10,   P2 = 1;
      OД3 = 100,  P3 = 1;

 Расчет требуемых коэффициентов деления основных делителей производится следующим образом.
 Определяется частота первого гетеродина:
    F = Fн + Fпч1,
 где частота настройки Fн и первая промежуточная частота Fпч1 взяты в кГц.,
    Fпч1 = 65500 кГц.
 Величины коэффициентов деления N для каждой их трех БИС определяются следующим образом:
    A = int{ ( F - 1000 * int{ (F / 1000) } ) / 500 };
    N1 = F - 1000 * int{F / 1000} + 3000 - A * 1000;
    N2 = int{F / 1000} - 3 + A;
    N3 = int{F / 100} + int{ ( (N1-2500) - 100 * int{ (N1-2500) / 100 } ) / 50 };
 где А - промежуточная  константа ;
 int{ } - целая часть числа, заключенного в скобки.
*/
void RPU_tract::set_central_freq_to_RPU(int presNb, int freq)
{
    int centralFreq = (presNb == -1 ? get_central_freq() : freq);

    int F_geterod = centralFreq / 1000 + 65500;
    int A  = int( ( F_geterod - 1000 * ( int(F_geterod / 1000) ) ) / 500);
    int N1 = F_geterod - 1000 * ( int(F_geterod / 1000) ) + 3000 - A * 1000;
    int N2 = int(F_geterod / 1000) - 3 + A;
    int N3 = int(F_geterod / 100) + int( ( (N1 - 2500) - 100 * int( (N1 - 2500) / 100) ) / 50);

    // в порт посылается код режима работы
    set_config_to_RPU();

    // откуда взяты эти значения
    // 0x64 = 100 (почему на 19?)
    // 0x0A = 10
    DWORD D1 = DWORD((0x64 << 19) | (N1 << 1)    );
    DWORD D2 = DWORD((0x0A << 19) | (N2 << 1) | 1);
    DWORD D3 = DWORD((0x64 << 19) | (N3 << 1) | 1);

    // учет особенности при P=1
    DWORD vrem1, vrem2, vrem3;

    // 0x1FFF =
    vrem1 = ((D2 >> 4) & 0x1FFF);
    vrem2 = D2 & 0xF;
    vrem3 = 0x0A << 19;
    D2 = vrem3 | vrem2 | (vrem1 << 6);

    vrem1 = ((D3 >> 4) & 0x1FFF);
    vrem2 = D3 & 0xF;
    vrem3 = 0x64 << 19;
    D3 = vrem3 | vrem2 | (vrem1 << 6);

    byte ManageGeterKod = 0;

    // предварительный сброс последней единички (1010)
    send_code(lpt + LPT_CONTROL_REG, 0xA);

    for(int i = 0; i < 32; i++)
    {
        ManageGeterKod = 0x8;
        ManageGeterKod |=  byte(((D1 >> 31) << 2) | ((D2 >> 31) << 1) | (D3 >> 31));

        send_code(lpt + LPT_DATA_REG, ManageGeterKod);

        // СТРОБ  ( B -- A )
        send_code(lpt + LPT_CONTROL_REG, 0xB);
        send_code(lpt + LPT_CONTROL_REG, 0xA);

        ManageGeterKod ^= 0x08;

        send_code(lpt + LPT_DATA_REG, ManageGeterKod);

        // СТРОБ  ( B -- A )
        send_code(lpt + LPT_CONTROL_REG, 0xB);
        send_code(lpt + LPT_CONTROL_REG, 0xA);

        D1 <<= 1;
        D2 <<= 1;
        D3 <<= 1;
    }

    ManageGeterKod = 0x00;
    send_code(lpt + LPT_DATA_REG, ManageGeterKod);
    // СТРОБ  ( B -- A )
    send_code(lpt + LPT_CONTROL_REG, 0xB);
    send_code(lpt + LPT_CONTROL_REG, 0xA);

    ManageGeterKod = 0x10;
    send_code(lpt + LPT_DATA_REG, ManageGeterKod);
    // СТРОБ  ( B -- A )
    send_code(lpt + LPT_CONTROL_REG, 0xB);
    send_code(lpt + LPT_CONTROL_REG, 0xA);

    ManageGeterKod = 0x00;
    send_code(lpt + LPT_DATA_REG, ManageGeterKod);
    // СТРОБ  ( B -- A )
    send_code(lpt + LPT_CONTROL_REG, 0xB);
    send_code(lpt + LPT_CONTROL_REG, 0xA);
}

/*---------------------------------КОД УПРАВЛЕНИЯ БЛОКАМИ ПРИЕМНИКОВ---------------------------------
D4 - 14-ти разрядный последовательный код управления блоками приемников

Формат управляющего слова:
+----+----+----+----+----+---+--------+---+---+---+---+---+---+---+
| 14 | 13 | 12 | 11 | 10 | 9 |	  8   | 7 | 6 | 5 | 4 | 3 | 2 | 1 |
+----+----+----+----+----+---+--------+---+---+---+---+---+---+---+
|  Полоса |       Прес       | Атт Вх |   Атт ВЧ  |    Атт ПЧ     |
+----+----+----+----+----+---+--------+---+---+---+---+---+---+---+

Полоса – код полосы пропускания задается в соответствии с таблицей.
+--------------+---------+
| Полоса       | Разряды |
| пропускания, +----+----+
| кГц          | 13 | 14 |
+--------------+----+----+
|     150  *   |  0	|  0 |
+--------------+----+----+
|     20       |  0	|  1 |
+--------------+----+----+
|     8        |  1	|  0 |
+--------------+----+----+
|     3    *   |  1	|  1 |
+--------------+----+----+
* кодограммы для фильтров 150 и 3 кГц неправильные: для 150 кГц - 1 1, а для 3 кГц - 0 0;

Прес - код преселектора, задается в соответствии с таблицей
+---------------+---------------+
| Частота, кГц  |    Разряды    |
|               +---+---+---+---+
|               | 9 | 10| 11| 12|
+---------------+---+---+---+---+
| 200 - 1000    | 1 | 1 | 1 | 1 |
+---------------+---+---+---+---+
| 1000 - 1500   | 1	| 0	| 0	| 0 |
+---------------+---+---+---+---+
| 1500 - 2200   | 0	| 1	| 1	| 1 |
+---------------+---+---+---+---+
| 2200 - 3200   | 0	| 1	| 1	| 0 |
+---------------+---+---+---+---+
| 3200 - 4700   | 0	| 1	| 0	| 1 |
+---------------+---+---+---+---+
| 4700 - 6800   | 0	| 1	| 0	| 0 |
+---------------+---+---+---+---+
| 6800 - 9900   | 0	| 0	| 1	| 1 |
+---------------+---+---+---+---+
| 9900- 14400   | 0	| 0	| 1	| 0 |
+---------------+---+---+---+---+
| 14400 - 20700 | 0	| 0	| 0	| 1 |
+---------------+---+---+---+---+
| 20700 - 32000 | 0	| 0	| 0	| 0 |
+---------------+---+---+---+---+

Атт Вх - код включения входного аттенюатора 20 дБ (1 – вкл, 0 - выкл.)

Атт ВЧ - двоичный код  аттенюатора ВЧ (дискрет 6 дБ)
  7 разряд -  6 дБ,
  6 разряд - 12 дБ,
  5 разряд - 24 дБ.

Атт ПЧ - двоичный код  аттенюатора ПЧ (дискрет 2 дБ)
 4 разряд - 2 дБ,
 3 разряд - 4 дБ,
 2 разряд - 8 дБ,
 1 разряд - 16 дБ.

Информационная последовательность D4 сопровождается импульсами синхронизации С2 и стробом Е положительной полярности.
Запись информации во входные регистры блоков приемника осуществляется по переднему фронту синхроимпульсов,
начиная с первого разряда.

Управление блоками приемников и первого гетеродина может осуществляться как одновременно,
так и независимо друг от друга.
*/
// если presNb == -1, то номер преселектора определяется автоматически
// если presNb != -1, то presNb == номеру используемого преселктора
void RPU_tract::set_tract_params_to_RPU(int presNb)
{
    short  band_mask = 0, preselektor_mask, InputAttenuator_mask;
    ushort  HFAttenuator_mask, IFAttenuator_mask, D4;
    int    carrier_kHz = get_central_freq() / 1000;

    // в порт посылается код конфигурации трактов
    set_config_to_RPU();

    // определяем маску полосы
    /* 13) расхождение с документацией:
                       для полосы 150 кГц должен быть код 0x0000,
                       для 20 кГц - 0x0001,
                       для 8 кГц - 0x0002,
                       для 3 кГц - 0x0003

       UPD 06.09.17
       после проверки на РПУ:
       код 0x0000 соответсвует полосе 3 кГц
       код 0x0003 соответсвует полосе 150 кГц
       коды для полос 8 и 20 кГц не изменились
    */
    switch(if_band_idx){
        case FOURTH_BAND: // 150 кГц
            band_mask = 0x0003;
            break;

        case THIRD_BAND: // 20 кГц
            band_mask = 0x0001;
            break;

        case SECOND_BAND: // 8 кГц
            band_mask = 0x0002;
            break;

        case FIRST_BAND: // 3 кГц
            band_mask = 0x0000;
            break;
    }

// определение маски преселектора
    preselektor_mask = 0;

/*
 * UPD 7.9.2017
 * все кодограммы корректны
 */
    if(presNb == -1){
        if(carrier_kHz > 200 && carrier_kHz <= 1000)
           preselektor_mask = 0x0009;

        if(carrier_kHz > 1000 && carrier_kHz <= 1500)
           preselektor_mask = 0x0008;

        if(carrier_kHz > 1500 && carrier_kHz <= 2200)
           preselektor_mask = 0x0007;

        if(carrier_kHz > 2200 && carrier_kHz <= 3200)
           preselektor_mask = 0x0006;

        if(carrier_kHz > 3200 && carrier_kHz <= 4700)
           preselektor_mask = 0x0005;

        if(carrier_kHz > 4700 && carrier_kHz <= 6800)
           preselektor_mask = 0x0004;

        if(carrier_kHz > 6800 && carrier_kHz <= 9900)
           preselektor_mask = 0x0003;

        if(carrier_kHz > 9900 && carrier_kHz <= 14400)
           preselektor_mask = 0x0002;

        if(carrier_kHz > 14400 && carrier_kHz <= 20700)
           preselektor_mask = 0x0001;

        if(carrier_kHz > 20700 && carrier_kHz <= 32000)
           preselektor_mask = 0x0000;
    }else{
        preselektor_mask = 9 - presNb;
    }

    // определяем маску АттВх
    InputAttenuator_mask = 0;

    // если включен, то 0x1, если выключен, то 0x0
    if(get_in_att_idx())
       InputAttenuator_mask = 0x0001;
    else
       InputAttenuator_mask = 0x0000;

    // определяем маску АттВЧ
    HFAttenuator_mask = get_hf_att_idx();

    // определяем маску АттПЧ
    IFAttenuator_mask = get_if_att_idx();

    //--- используя все данные формируем в D4 последовательный 14-ти разрядный код ---//
    // D4 - 2 байта, из которых 14 бит информационные
    D4 = ushort((band_mask << 12) | (preselektor_mask << 8) | (InputAttenuator_mask << 7) | (HFAttenuator_mask << 4) | IFAttenuator_mask);

    // зачем этот сдвиг на два? (мб для полного заполнения двух байт?)
    D4 <<= 2;
    // теперь информационная часть содержится в разрядах 15,...,2 а в битах 1 и 0 - нули

    send_code(lpt + LPT_CONTROL_REG, 0xA);

    // один байт
    uchar ManageRPUKod = 0;

    // 14 итераций
    for(int i = 0; i < 14; i++){
        // 0x80 = 1000 0000 = 128
        ManageRPUKod  = uchar(0x80);

        // 0x8000 = 1000 0000 0000 0000 = 32768

        // операция (D4 & 0x8000) будет всегда давать 0, т.к. D4 = short (информация с 0 по 15 бит),
        // а 0x8000 = int со всеми нулями с 0 по 30 бит
        //
        ManageRPUKod |= char( (D4 & 0x8000) >> 10 );
        send_code(lpt + LPT_DATA_REG, ManageRPUKod);
        // СТРОБ  ( B -- A )
        send_code(lpt + LPT_CONTROL_REG, 0xB);
        send_code(lpt + LPT_CONTROL_REG, 0xA);

        // 0x40 = 0100 0000 = 64
        ManageRPUKod ^= 0x40;
        send_code(lpt + LPT_DATA_REG, ManageRPUKod);
        // СТРОБ  ( B -- A )
        send_code(lpt + LPT_CONTROL_REG, 0xB);
        send_code(lpt + LPT_CONTROL_REG, 0xA);

        D4 <<= 1;
    }

    // зачем отправляются нули?
    ManageRPUKod = 0x00;
    send_code(lpt + LPT_DATA_REG, ManageRPUKod);
    // СТРОБ  ( B -- A )
    send_code(lpt + LPT_CONTROL_REG, 0xB);
    send_code(lpt + LPT_CONTROL_REG, 0xA);
}
