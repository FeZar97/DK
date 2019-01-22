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
unsigned int RPU::get_lpt_idx()
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

unsigned int RPU::get_lpt_addres()
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

void RPU::test_rpu()
{
    test_preselectors();
    test_attenuators();
}

/*
 первый этап работы АСКР
 производится перестройка на центральные частоты преселекторов и измеряется уровень калибровочного сигнала
   200  - 1000  : 600 кГц
   1000 - 1500  : 1250 кГц
   1500 - 2200  : 1850 кГц
   2200 - 3200  : 2700 кГц
   3200 - 4700  : 3800 кГц  (должно быть 3950)
   4700 - 6800  : 5250 кГц  (должно быть 5750)
   6800 - 9900  : 7350 кГц  (должно быть 8750)
   9900 - 14400 : 12150 кГц
  14400 - 20700 : 17550 кГц
  20700 - 32000 : 26350 кГц

  1) выставление четырехканального режима РПУ
  2) выставление центральной частоты канала
  3) настройка калибратора
  4) код начала калибровки - строка 4 в таблице кодограмм
  5) измерения
  6) код окончания калибровки - строка 5 в таблице кодограмм
*/
void RPU::test_preselectors()
{
    switch(config){
        case FOUR_CHANNEL:
            first_tract.test_preselectors();
            break;

        case THREE_ONE_CHANNEL:
        case TWO_TWO_CHANNEL:
            first_tract.test_preselectors();
            second_tract.test_preselectors();
            break;

        case TWO_ONE_ONE_CHANNEL:
            first_tract.test_preselectors();
            second_tract.test_preselectors();
            third_tract.test_preselectors();
            break;

        case ONE_ONE_ONE_ONE_CHANNEL:
            first_tract.test_preselectors();
            second_tract.test_preselectors();
            third_tract.test_preselectors();
            fourth_tract.test_preselectors();
            break;
    }

    /*
    if(dsp->dsp_params->read_params->is_recording)
        emit stop();

// настройки SDR
    sdr->sdr_params->central_freq = 5500000;
    sdr->sdr_params->direct_sampling_idx = 2;
    sdr->sdr_params->gain_idx = 1;
    sdr->sdr_params->sample_rate = 230000;

// настройки тракта РПУ
    rpu->first_tract.set_central_freq(15000000);
    rpu->first_tract.set_if_band_idx(3);
    rpu->first_tract.set_in_att_idx(OFF);
    rpu->first_tract.set_hf_att_idx(0);
    rpu->first_tract.set_if_att_idx(0);

// настройки калибратора
    rpu->kalibrator.set_work_status(ON);
    rpu->kalibrator.set_att_idx(0);
    rpu->kalibrator.set_exit_type(INTERNAL);
    rpu->kalibrator.set_signal_type(SINUS);

// настройки ЦОС
    // для частоты 5 500 000 это оптимальные настройки для подавления DC
    dsp->dsp_params->fft_params->dc_offset.re = 127.005;
    dsp->dsp_params->fft_params->dc_offset.im = 127.005;
    dsp->dsp_params->read_params->readout_per_seconds = 10;

// кол-во обращений к преимнику/сек = 10
// объем считываемых данных = 2 * 230 000 / 10 = 46 000 байт

    if(dsp->prepair_to_record(sdr)){
        emit bind_slots_signals();

        dsp->fft_shift_thread->start();
        dsp->fft_thread->start();
        //dsp->reader_thread->start();

        Sleep(50);

        // на данном моменте на спектре должна быть видна синусоида калибратора
        int freqs[10] = {600000, 1250000, 1850000, 2700000, 3800000, 5250000, 7350000, 12150000, 17550000, 26350000}, i, j, n_read;
        char *input_buffer = new char[dsp->dsp_params->read_params->read_rb_cell_size];

        // заполняется один кольцевой буфер
        for(i = 0; i < DSP_READ_RB_SIZE; i++){
            rtlsdr_read_sync(sdr->sdr_params->sdr_ptr,
                             input_buffer,
                             dsp->dsp_params->read_params->read_rb_cell_size,
                             &n_read);

            ippsConvert_8u32f(dsp->dsp_params->read_params->read_cell,
                             (Ipp32f*)dsp->dsp_params->read_params->read_rb[dsp->dsp_params->read_params->read_rb_cell_idx],
                              dsp->dsp_params->read_params->read_rb_cell_size);
        }

        global_update_interface();
    }
    else{
        QMessageBox::critical(this, "Ошибка запуска", "Не удалось начать калибровку. Ошибка 1.");
        sdr->sdr_params->is_open = false;
        dsp->set_record_flags(false);
    }
    */
}

void RPU::test_attenuators()
{

}
