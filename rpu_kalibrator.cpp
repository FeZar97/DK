#include "rpu_kalibrator.h"

RPU_kalibrator::RPU_kalibrator()
{
    // подтягиваем функцию
    HINSTANCE hinstLib = LoadLibrary(TEXT("inpout32.dll"));
    gfpOut_char = (lpOutc)GetProcAddress(hinstLib, "DlPortWritePortUchar");

    signal_type      = RPU_DEFAULT_KALIB_SIGNAL_TYPE;
    modulation_freq  = RPU_DEFAULT_KALIB_MOD_FREQ;
    att_idx          = RPU_DEFAULT_KALIB_ATT_IDX;
    exit_type        = RPU_DEFAULT_KALIB_EXIT;
    work_status      = RPU_DEFAULT_KALIB_STATUS;

    lpt              = RPU_DEFAULT_LPT;
}

RPU_kalibrator::~RPU_kalibrator()
{
}

void RPU_kalibrator::send_code(unsigned short addr, unsigned char code)
{
    gfpOut_char(addr, code);
}

// изменение типа калибровочного сигнала
void RPU_kalibrator::set_signal_type(MOD_SIGNAL new_signal_type)
{
    if(new_signal_type == SINUS || new_signal_type == RADIOIMP){
        signal_type  = new_signal_type;
    }else{
        signal_type  = SINUS;
    }

    // отправка кодограммы
    set_signal_type_to_RPU(FIRST);
    set_work_status_to_RPU();
}

MOD_SIGNAL RPU_kalibrator::get_signal_type()
{
    return signal_type;
}

// изменение частоты модуляции
void RPU_kalibrator::set_modulation_freq(unsigned short new_modulation_freq_in_kHz)
{
    // перевод в Гц
    new_modulation_freq_in_kHz *= 1000;

    // проверка на допустимые границы
    if(new_modulation_freq_in_kHz < RPU_MIN_KALIB_MOD_FREQ)
        modulation_freq = RPU_MIN_KALIB_MOD_FREQ;
    else
        if(new_modulation_freq_in_kHz > RPU_MAX_KALIB_MOD_FREQ)
            modulation_freq = RPU_MAX_KALIB_MOD_FREQ;
        else
            modulation_freq = new_modulation_freq_in_kHz;

    // отправка кодограммы
    set_modulation_freq_to_RPU();
}

unsigned short RPU_kalibrator::get_modulation_freq()
{
    return modulation_freq;
}

// изменение значения аттенюатора калибратора
void RPU_kalibrator::set_att_idx(unsigned short new_att_idx)
{
    att_idx = new_att_idx;

    // отправка кодограммы
    set_attenuation_to_RPU();
}

unsigned short RPU_kalibrator::get_att_idx()
{
    return att_idx;
}

// изменение выхода калибратора
void RPU_kalibrator::set_exit_type(KALIB_TYPE new_exit_type)
{
    exit_type = new_exit_type;

    // отправка кодограммы
    set_signal_type_to_RPU(FIRST);
    set_work_status_to_RPU();
}

KALIB_TYPE RPU_kalibrator::get_exit_type()
{
    return exit_type;
}

// включение выключение калибратора
void RPU_kalibrator::set_work_status(STATUS new_work_status)
{
    work_status = new_work_status;

    // отправка кодограммы
    set_work_status_to_RPU();
}

STATUS RPU_kalibrator::get_work_status()
{
    return work_status;
}

void RPU_kalibrator::set_lpt(LPT_ADDRESS new_lpt)
{
    lpt = new_lpt;
}

LPT_ADDRESS RPU_kalibrator::get_lpt()
{
    return lpt;
}

RPU_kalibrator &RPU_kalibrator::operator=(RPU_kalibrator &RightKalib)
{
    signal_type     = RightKalib.get_signal_type();
    modulation_freq = RightKalib.get_modulation_freq();
    att_idx         = RightKalib.get_att_idx();
    exit_type       = RightKalib.get_exit_type();
    work_status     = RightKalib.get_work_status();
    lpt             = RightKalib.get_lpt();

    return *this;
}

// установка в РПУ частоты модуляции калибратора
void RPU_kalibrator::set_modulation_freq_to_RPU()
{
    int ModulationFreq = get_modulation_freq();
    int N5 = int(30000 / ModulationFreq);
    DWORD D5 = 0;

    D5 = ((D5 | 0x0A) << 19)  | ((D5 | N5) << 3) | 1;

    // учет особенности при P=1
    byte D5_0 = (char)D5;

    ((byte*)(&D5))[0] = (byte)(((D5_0>>2) & 0x0f) | (D5_0 & 0xc1));

    send_code(lpt + LPT_CONTROL_REG, 0x8);// строб в CR0

    char ManageKod = 0;

    for(int i = 0; i < 32; i++){
        ManageKod  = 0x02;
        ManageKod |=  byte( (D5 & 0x80000000) >> 31);

        send_code(lpt + LPT_DATA_REG, ManageKod);
        // СТРОБ  ( 9 -- 8 )
        send_code(lpt + LPT_CONTROL_REG, 0x9);
        send_code(lpt + LPT_CONTROL_REG, 0x8);

        ManageKod ^= 0x02;
        send_code(lpt + LPT_DATA_REG, ManageKod);
        // СТРОБ  ( 9 -- 8 )
        send_code(lpt + LPT_CONTROL_REG, 0x9);
        send_code(lpt + LPT_CONTROL_REG, 0x8);

        D5 <<= 1;
    }

    ManageKod = 0x00;
    send_code(lpt + LPT_DATA_REG, ManageKod);
    // СТРОБ  ( 9 -- 8 )
    send_code(lpt + LPT_CONTROL_REG, 0x9);
    send_code(lpt + LPT_CONTROL_REG, 0x8);

    ManageKod = 0x04;
    send_code(lpt + LPT_DATA_REG, ManageKod);
    // СТРОБ  ( 9 -- 8 )
    send_code(lpt + LPT_CONTROL_REG, 0x9);
    send_code(lpt + LPT_CONTROL_REG, 0x8);

    ManageKod = 0x00;
    send_code(lpt + LPT_DATA_REG, ManageKod);
    // СТРОБ  ( 9 -- 8 )
    send_code(lpt + LPT_CONTROL_REG, 0x9);
    send_code(lpt + LPT_CONTROL_REG, 0x8);
}

// установка в РПУ уровня аттенюации калибратора
void RPU_kalibrator::set_attenuation_to_RPU()
{
    char D5 = 0x00;

    // тут нужен индекс аттенюации, а не само значение
    int KalibAttenuation = get_att_idx() * 2;

    if(KalibAttenuation <= 30){
       D5 = (byte)(KalibAttenuation >> 1);
    }else{
        KalibAttenuation -= 30;
        D5 = (byte)(KalibAttenuation >> 1);
        D5 <<= 4;
        D5 |= 0xFF;
    }

    send_code(lpt + LPT_CONTROL_REG, 0x8); // строб в CR0

    byte ManageKod = 0x00;

    for(int i = 0; i < 8; i++){
        ManageKod  = 0x0A;
        ManageKod |=  byte(D5 & 0x01);
        send_code(lpt + LPT_DATA_REG, ManageKod);
        // СТРОБ  ( 9 -- 8 )
        send_code(lpt + LPT_CONTROL_REG, 0x9);
        send_code(lpt + LPT_CONTROL_REG, 0x8);

        ManageKod ^= 0x02;
        send_code(lpt + LPT_DATA_REG, ManageKod);
        // СТРОБ  ( 9 -- 8 )
        send_code(lpt + LPT_CONTROL_REG, 0x9);
        send_code(lpt + LPT_CONTROL_REG, 0x8);

        D5 >>= 1;
    }

    ManageKod = 0x00;
    send_code(lpt + LPT_DATA_REG, ManageKod);
    // СТРОБ  ( 9 -- 8 )
    send_code(lpt + LPT_CONTROL_REG, 0x9);
    send_code(lpt + LPT_CONTROL_REG, 0x8);
}

// включение / отключение калибратора в РПУ
void RPU_kalibrator::set_work_status_to_RPU()
{
    send_code(lpt + LPT_CONTROL_REG, 0xC);

    byte ManageKod;

    if(get_work_status() == ON)
        if(get_exit_type() == EXTERNAL)
           ManageKod = 0x10;
        else
           ManageKod = 0x1F;
    else
       ManageKod = 0x00;

    send_code(lpt + LPT_DATA_REG, ManageKod);
    // СТРОБ  ( D -- C )(110)
    send_code(lpt + LPT_CONTROL_REG, 0xD);
    send_code(lpt + LPT_CONTROL_REG, 0xC);
}

// установка типа калибровочного сигнала в РПУ
void RPU_kalibrator::set_signal_type_to_RPU(NUMERALS SupportTract)
{
    byte D5 = 0x00;

    if(get_signal_type() == RADIOIMP)
       D5 |= 0x01;
    D5 <<= 1;

    if(get_exit_type() == EXTERNAL)
       D5 |= 0x01;
    D5 <<= 2;

    switch(SupportTract){
        case FIRST:
            D5 |= (char)0x2;
            break;
        case SECOND:
            D5 |= (char)0x3;
            break;
        case THIRD:
            D5 |= (char)0x0;
            break;
        case FOURTH:
            D5 |= (char)0x1;
            break;
        default:
            break;
    }

    send_code(lpt + LPT_CONTROL_REG, 0x8);
    char ManageKod = 0x00;

    for(int i = 0; i < 4; i++){
        ManageKod = char(((D5 >> (3 - i)) & 0x1) | 0x10);

        send_code(lpt + LPT_DATA_REG, ManageKod);
        // СТРОБ  ( 9 -- 8 )
        send_code(lpt + LPT_CONTROL_REG, 0x9);
        send_code(lpt + LPT_CONTROL_REG, 0x8);

        ManageKod ^= 0x02;
        send_code(lpt + LPT_DATA_REG, ManageKod);
        // СТРОБ  ( 9 -- 8 )
        send_code(lpt + LPT_CONTROL_REG, 0x9);
        send_code(lpt + LPT_CONTROL_REG, 0x8);
    }

    ManageKod = 0x00;

    send_code(lpt + LPT_DATA_REG, ManageKod);
    // СТРОБ  ( 9 -- 8 )
    send_code(lpt + LPT_CONTROL_REG, 0x9);
    send_code(lpt + LPT_CONTROL_REG, 0x8);
}

