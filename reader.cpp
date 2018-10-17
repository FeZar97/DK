#include "reader.h"

READER::READER(DSP_params *new_dsp_params,
               SDR_params *new_sdr_params)
{
    if(new_dsp_params != NULL)
        dsp_params = new_dsp_params;
    else
        dsp_params = new DSP_params();

    if(new_sdr_params != NULL)
        sdr_params = new_sdr_params;
    else
        sdr_params = new SDR_params();
}

READER::~READER()
{
    // еще один бесполезный деструктор
}

// воркер
void READER::start_reading()
{
    rtlsdr_reset_buffer(sdr_params->sdr_ptr);

    // количество обращения к приемнику
    int iterations = times[dsp_params->read_params->rec_time_idx] * dsp_params->read_params->readout_per_seconds, i = 0, n_read;

    // считывание просиходит до тех пор, пока либо не кончится время, либо не произойдет принудительная остановка
    while(/*i < iterations && */!dsp_params->read_params->emergency_end_recording){

        rtlsdr_reset_buffer(sdr_params->sdr_ptr);

        // общение с железкой
        rtlsdr_read_sync(sdr_params->sdr_ptr,
                         dsp_params->read_params->read_cell,
                         dsp_params->read_params->read_rb_cell_size,
                         &n_read);

        // конвертация 8u -> 32f
        ippsConvert_8u32f(dsp_params->read_params->read_cell,
                         (Ipp32f*)dsp_params->read_params->read_rb[dsp_params->read_params->read_rb_cell_idx],
                          dsp_params->read_params->read_rb_cell_size);

        // Алиса в зазеркалье
        if(dsp_params->fft_params->fft_inversion)
            ippsConj_32fc_I(dsp_params->read_params->read_rb[dsp_params->read_params->read_rb_cell_idx],
                            dsp_params->read_params->read_rb_cell_size / 2);

        // на бпф
        if(dsp_params->fft_params->fft_mode == READER_FFT)
            emit get_fft_step(dsp_params->read_params->read_rb[dsp_params->read_params->read_rb_cell_idx]);

        // АКВАФОР убирает вредное, сохраняя полезное
        //if(dsp_params->flt_params->use_filter)
        //    emit get_filtration_step(read_rb[dsp_params->flt_params->filtration_rb_cell_idx]);

        // двигаем частоту
        if(dsp_params->fft_params->fft_mode == SHIFT_FFT){
            emit get_shift_step(dsp_params->read_params->read_rb[dsp_params->read_params->read_rb_cell_idx]);
        }

        // запишем музыку
        if(dsp_params->wav_params->use_first_file)
            emit write_to_file(dsp_params->read_params->read_rb[dsp_params->read_params->read_rb_cell_idx]);

        // в копилочку и показать сколько осталось
        // emit update_ReadProgressBar(int(100 * float(++i) / float(iterations)));
        dsp_params->read_params->read_rb_cell_idx = (dsp_params->read_params->read_rb_cell_idx + 1) % DSP_READ_RB_SIZE;
    }

    // если завершилось штатно, то флаг emergency_end_recording == 0
    // если завершилось экстренно, то emergency_end_recording == 1
    if(dsp_params->read_params->emergency_end_recording)
        emit end_of_recording(false);
    else
        emit end_of_recording(true);
}
