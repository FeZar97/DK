#include "sound_maker.h"

sound_maker::sound_maker(DSP_params *new_dsp_params,
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

sound_maker::~sound_maker()
{
    delete dsp_params;
    delete sdr_params;
}
void sound_maker::get_sound_step(Ipp32fc *rb_cell, unsigned int cell_size)
{
    // преобразовать 32fc в 16s
    // понизить частоту дискретизации с сохранением "фазы"
    // отправить полученную ячейку в AudioOutput



}
