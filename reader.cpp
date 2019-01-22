#include "reader.h"

READER::READER(DSP_params *new_dsp_params,
               SDR_params *new_sdr_params)
{
    if(new_dsp_params != nullptr)
        dsp_params = new_dsp_params;
    else
        dsp_params = new DSP_params();

    if(new_sdr_params != nullptr)
        sdr_params = new_sdr_params;
    else
        sdr_params = new SDR_params();
}

READER::~READER()
{
}

// воркер
void READER::start_reading()
{
    int n_read;

    // считывание просиходит до тех пор, пока либо не кончится время, либо не произойдет принудительная остановка
    while(!dsp_params->read_params->emergency_end_recording){

        rtlsdr_reset_buffer(sdr_params->sdr_ptr);

        // общение с железкой
        rtlsdr_read_sync(sdr_params->sdr_ptr,
                         dsp_params->read_params->read_cell,
                         int(dsp_params->read_params->read_rb_cell_size),
                         &n_read);

        // конвертация 8u -> 32f
        ippsConvert_8u32f(dsp_params->read_params->read_cell,
                          reinterpret_cast<Ipp32f*>(dsp_params->read_params->read_rb[dsp_params->read_params->read_rb_cell_idx]),
                          int(dsp_params->read_params->read_rb_cell_size));

        // подавление постоянной составляющей
        ippsSubC_32fc_I(dsp_params->fft_params->dc_offset,
                        dsp_params->read_params->read_rb[dsp_params->read_params->read_rb_cell_idx],
                        dsp_params->read_params->read_rb_cell_size / 2);

        // сопряжение
        if(dsp_params->fft_params->fft_inversion)
            ippsConj_32fc_I(dsp_params->read_params->read_rb[dsp_params->read_params->read_rb_cell_idx],
                            dsp_params->read_params->read_rb_cell_size / 2);

        // спектр
        if(dsp_params->fft_params->fft_mode == READER_FFT)
            emit get_fft_step(dsp_params->read_params->read_rb[dsp_params->read_params->read_rb_cell_idx],
                              dsp_params->read_params->read_rb_cell_size / 2);

        // фильтрация
        if(dsp_params->flt_params->r_frec != 0.5){
            emit get_filtration_step(dsp_params->read_params->read_rb[dsp_params->read_params->read_rb_cell_idx],
                                     dsp_params->read_params->read_rb_cell_size);
        }else{
            // частотный сдвиг
            emit get_shift_step(dsp_params->read_params->read_rb[dsp_params->read_params->read_rb_cell_idx],
                                dsp_params->read_params->read_rb_cell_size);
        }

        // вывод в файл
        if(dsp_params->read_params->use_first_file && dsp_params->read_params->use_files)
            emit write_to_file(dsp_params->read_params->read_rb[dsp_params->read_params->read_rb_cell_idx]);

        // инкремент итератора
        dsp_params->read_params->read_rb_cell_idx = (dsp_params->read_params->read_rb_cell_idx + 1) % DSP_READ_RB_SIZE;

        // вывод звука
        //emit get_sound_step(dsp_params->read_params->read_rb[dsp_params->read_params->read_rb_cell_idx],
        //                    dsp_params->read_params->read_rb_cell_size);
    }

    // если завершилось штатно, то флаг emergency_end_recording == 0
    // если завершилось экстренно, то emergency_end_recording == 1
    emit end_of_recording(!dsp_params->read_params->emergency_end_recording);
}
