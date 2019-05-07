#include "demodulator.h"

DEMODULATOR::DEMODULATOR(SDR_params *new_sdr_params, DSP_params *new_dsp_params): sdr_params{new_sdr_params}, dsp_params{new_dsp_params}
{}

DEMODULATOR::~DEMODULATOR()
{}

void DEMODULATOR::get_demod_step(Ipp32fc *cell)
{
    switch(dsp_params->demod_params.demod_mode){

        case AM:{
            // понижение частоты дискретизации с sdr_sample_rate до 48кГц
            ippsSampleDown_32fc(cell,
                                dsp_params->flt_params.rb.cell_size / 2,
                                dsp_params->demod_params.down_buf_fc,
                                &dsp_params->demod_params.down_buf_len,
                                dsp_params->demod_params.am_down_sample_factor,
                                &dsp_params->demod_params.am_down_sample_phase);

            ippsMagnitude_32fc(dsp_params->demod_params.down_buf_fc,
                               dsp_params->demod_params.rb.get_current_cell(),
                               DSP_SOUND_SAMPLE_RATE / dsp_params->read_params.readout_per_seconds);

            break;
        }

        case USB:{
            ippsZero_32f(dsp_params->demod_params.rb.get_current_cell(),
                         dsp_params->demod_params.rb.cell_size);
            break;
        }

        case LSB:{
            ippsZero_32f(dsp_params->demod_params.rb.get_current_cell(),
                         dsp_params->demod_params.rb.cell_size);
            break;
        }

        case FM:{
            if(sdr_params->sample_rate != 2496000)
                break;
            // первое понижение частоты дискретизации (с sdr_sample_rate кГц до 192 кГц)
            ippsSampleDown_32fc(cell,
                                dsp_params->flt_params.rb.cell_size / 2,
                                dsp_params->demod_params.down_buf_fc,
                                &dsp_params->demod_params.down_buf_len,
                                dsp_params->demod_params.fm_first_down_sample_factor,
                                &dsp_params->demod_params.fm_first_down_sample_phase);

            ippsCplxToReal_32fc(dsp_params->demod_params.down_buf_fc,
                                dsp_params->demod_params.down_buf_re,
                                dsp_params->demod_params.down_buf_im,
                                dsp_params->flt_params.rb.cell_size / (2 * dsp_params->demod_params.fm_first_down_sample_factor));

            ippsAtan2_32f_A21(dsp_params->demod_params.down_buf_re,
                              dsp_params->demod_params.down_buf_im,
                              dsp_params->demod_params.down_buf_re,
                              dsp_params->flt_params.rb.cell_size / (2 * dsp_params->demod_params.fm_first_down_sample_factor));

            // "бархатный путь"
            dsp_params->demod_params.down_buf_im[0] = dsp_params->demod_params.first_last_sample
                                                    - dsp_params->demod_params.down_buf_re[0];
            dsp_params->demod_params.first_last_sample    = dsp_params->demod_params.down_buf_re[dsp_params->demod_params.rb.cell_size / (2 * dsp_params->demod_params.fm_first_down_sample_factor) - 1];

            // 2 - 1
            ippsSub_32f(dsp_params->demod_params.down_buf_re + 1,
                        dsp_params->demod_params.down_buf_re,
                        dsp_params->demod_params.down_buf_im,
                        dsp_params->flt_params.rb.cell_size / (2 * dsp_params->demod_params.fm_first_down_sample_factor) - 1);


            for(int i = 0; i < dsp_params->flt_params.rb.cell_size / (2 * dsp_params->demod_params.fm_first_down_sample_factor); i++){
                for(; dsp_params->demod_params.down_buf_im[i] >  IPP_PI ; dsp_params->demod_params.down_buf_im[i] -= 2 * IPP_PI);
                for(; dsp_params->demod_params.down_buf_im[i] < -IPP_PI ; dsp_params->demod_params.down_buf_im[i] += 2 * IPP_PI);

                dsp_params->demod_params.down_buf_im[i] *= Ipp32f(32767 / IPP_PI);
            }

            ippsCopy_32f(dsp_params->demod_params.down_buf_im,
                         dsp_params->demod_params.sound_filtration_buf,
                         dsp_params->flt_params.rb.cell_size / (2 * dsp_params->demod_params.fm_first_down_sample_factor));

            // дофильтровка до звуковых частот
            ippsFIRSR_32f(dsp_params->demod_params.down_buf_im,
                          dsp_params->demod_params.sound_filtration_buf,
                          dsp_params->flt_params.rb.cell_size / (2 * dsp_params->demod_params.fm_first_down_sample_factor),
                          dsp_params->demod_params.sound_flt_spec,
                          dsp_params->demod_params.sound_delay_line,
                          dsp_params->demod_params.sound_delay_line,
                          dsp_params->demod_params.sound_flt_buf);

            // понижение частоты дискретизации с 192кГц до DSP_SOUND_SAMPLE_RATE
            ippsSampleDown_32f(dsp_params->demod_params.sound_filtration_buf,
                               dsp_params->flt_params.rb.cell_size / (2 * dsp_params->demod_params.fm_first_down_sample_factor),
                               dsp_params->demod_params.rb.get_current_cell(),
                               &dsp_params->demod_params.down_buf_len,
                               dsp_params->demod_params.fm_second_down_sample_factor,
                               &dsp_params->demod_params.fm_second_down_sample_phase);
            break;
        }

        case BPSK:{
            break;
        }

        case QPSK:{
            break;
        }
    }

    // вывод звука
    get_sound_step(dsp_params->demod_params.rb.get_current_cell());

    // инкремент итератора по КБ демодуляции
    dsp_params->demod_params.rb.increase_cnt();
}
