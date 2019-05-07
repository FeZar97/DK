#include "dsp.h"

DSP::DSP(SDR *new_sdr)
{
    dsp_params = new DSP_params();

    sdr = new_sdr;

    reader     = new READER(sdr->sdr_params, dsp_params);
    iqcompensator = new IQCompensator(dsp_params);
    flt        = new FILTER(sdr->sdr_params, dsp_params);
    fft        = new FFT_CALCER(dsp_params);
    shifter    = new SHIFTER(sdr->sdr_params, dsp_params);
    demod      = new DEMODULATOR(sdr->sdr_params, dsp_params);
    sounder    = new SOUNDER(sdr->sdr_params, dsp_params);

    reader_rec = new WAV_RECORDER();
    flt_rec    = new WAV_RECORDER();
    shift_rec  = new WAV_RECORDER();
    sound_rec  = new WAV_RECORDER();

    wav_thread.start();
    sound_thread.start();
    demod_thread.start();
    fft_shift_thread.start();
    fft_thread.start();
    filtration_thread.start();
    iqcompensator_thread.start();
    reader_thread.start();

    bind_objects();
}

DSP::~DSP()
{
    sound_thread.quit();
    sound_thread.wait();

    demod_thread.quit();
    demod_thread.wait();

    wav_thread.quit();
    wav_thread.wait();

    fft_shift_thread.quit();
    fft_shift_thread.wait();

    fft_thread.quit();
    fft_thread.wait();

    filtration_thread.quit();
    filtration_thread.wait();

    iqcompensator_thread.quit();
    iqcompensator_thread.wait();

    reader_thread.quit();
    reader_thread.wait();
}

/* перенос воркеров в свои потоки,
   сигнально-слотовая связь между воркерами
   настройка удаления воркеров */
void DSP::bind_objects()
{
    reader->moveToThread(&reader_thread);
    connect(&reader_thread, &QThread::finished, reader, &QObject::deleteLater);

    iqcompensator->moveToThread(&iqcompensator_thread);
    connect(&iqcompensator_thread, &QThread::finished, iqcompensator, &QObject::deleteLater);
    connect(reader, &READER::get_compenaste_step, iqcompensator, &IQCompensator::get_compensate_step);

    flt->moveToThread(&filtration_thread);
    connect(&filtration_thread, &QThread::finished, flt, &QObject::deleteLater);
    connect(reader, &READER::get_filtration_step, flt, &FILTER::get_filtration_step); // переход от чтения к фильтрации

    shifter->moveToThread(&fft_shift_thread);
    connect(&fft_shift_thread, &QThread::finished, shifter, &QObject::deleteLater);
    connect(flt, &FILTER::get_shift_step, shifter, &SHIFTER::get_shift_step);

    fft->moveToThread(&fft_thread);
    connect(&fft_thread, &QThread::finished, fft, &QObject::deleteLater);
    connect(reader, &READER::get_fft_step, fft, &FFT_CALCER::get_fft_step);
    connect(flt, &FILTER::get_fft_step, fft, &FFT_CALCER::get_fft_step);
    connect(shifter, &SHIFTER::get_fft_step, fft, &FFT_CALCER::get_fft_step);

    demod->moveToThread(&demod_thread);
    connect(shifter, &SHIFTER::get_demod_step, demod, &DEMODULATOR::get_demod_step);
    connect(&demod_thread, &QThread::finished, demod, &QObject::deleteLater);

    sounder->moveToThread(&sound_thread);
    connect(demod, &DEMODULATOR::get_sound_step, sounder, &SOUNDER::get_sound_step);
    connect(&sound_thread, &QThread::finished, sounder, &QObject::deleteLater);

    reader_rec->moveToThread(&wav_thread);
    connect(reader, &READER::write_to_file_32f, reader_rec, &WAV_RECORDER::get_write_step_32f);
    connect(&wav_thread, &QThread::finished, reader_rec, &QObject::deleteLater);

    flt_rec->moveToThread(&wav_thread);
    connect(flt, &FILTER::write_to_file_32f, flt_rec, &WAV_RECORDER::get_write_step_32f);
    connect(&wav_thread, &QThread::finished, flt_rec, &QObject::deleteLater);

    shift_rec->moveToThread(&wav_thread);
    connect(shifter, &SHIFTER::write_to_file32f, shift_rec, &WAV_RECORDER::get_write_step_32f);
    connect(&wav_thread, &QThread::finished, shift_rec, &QObject::deleteLater);

    sound_rec->moveToThread(&wav_thread);
    connect(sounder, &SOUNDER::write_to_file16s, sound_rec, &WAV_RECORDER::get_write_step_16s);
    connect(&wav_thread, &QThread::finished, sound_rec, &QObject::deleteLater);
}

/*
 конфигурирование приемника
 пересчет параметров
 выделение памяти

 возвращает 0 при успешном выполнении
           -1 при наличии ошибок в конфигурировании SDR
           -2 при наличии ошибок в recalc_dsp_params
           -3 при ошибках в подготовке wav файлов
*/
int DSP::prepair_to_record()
{
    // выставление выбранных параметров на приемнике
    if(sdr->set_params_to_device())
        return -1;

    // пересчет параметров и выделение памяти
    if(recalc_dsp_params())
        return -2;

    // крайняя проверка перед запуском - если возвратится -1, то приемник недоступен => нельзя запускать
    sdr->sdr_params->is_open = (rtlsdr_set_center_freq(sdr->sdr_params->sdr_ptr, uint(sdr->sdr_params->central_freq)) == -1 ? false : true);

    return 0;
}

// пересчет параметров ЦОС
// возращает 0 при успешном расчете
//          -1 ошибка в расчете read_rb_cell_size
//          -2 ошибка в расчете fft_max_averages_number
int DSP::recalc_dsp_params()
{
    if(!dsp_params->read_params.readout_per_seconds)
        dsp_params->read_params.readout_per_seconds = DSP_READOUT_PER_SECONDS;

    // количество float сэмплов
    dsp_params->read_params.rb.cell_size = 2 * sdr->sdr_params->sample_rate / dsp_params->read_params.readout_per_seconds;
    // qDebug() << "read_rb_cell_size = " << dsp_params->read_params->read_rb_cell_size;

    if(dsp_params->read_params.rb.cell_size < 1)
        return -1;

    // на фильтрацию поступает столько же байт, сколько получает reader
    dsp_params->flt_params.rb.cell_size = dsp_params->read_params.rb.cell_size;

    // на частотный сдвиг поступает столько же байт, сколько получает reader
    dsp_params->shift_params.rb.cell_size = dsp_params->read_params.rb.cell_size;

    // на вход БПФ всегда поступает одинаковое кол-во байт
    dsp_params->fft_params.fft_input_cell_size = dsp_params->read_params.rb.cell_size;

    // максимальное количество усреднений = отношению кол-ва комплексных отсчетов, поступающих на вход FFT_CALCER, к размеру БПФ
    dsp_params->fft_params.fft_max_averages_number = dsp_params->fft_params.fft_input_cell_size / (2 * DSP_FFT_SIZE);
    if(dsp_params->fft_params.fft_max_averages_number < 1)
        return -2;

    dsp_params->fft_params.fft_averages_number = dsp_params->fft_params.fft_max_averages_number;
    //dsp_params->fft_params.forced_noise_matrix_update = true;

    // размер ячейки КБ демодуляции  = 48к / readout_per_seconds
    dsp_params->demod_params.rb.cell_size = DSP_SOUND_SAMPLE_RATE / dsp_params->read_params.readout_per_seconds;

    // количество float сэмплов, передаваемых между обработчиками за один такт = 2 * sdr_sample_rate / readout_per_seconds

    // коэф-т понижения частоты дискретизации для АМ-демодуляций
    dsp_params->demod_params.am_down_sample_factor = sdr->sdr_params->sample_rate / DSP_SOUND_SAMPLE_RATE;
    dsp_params->demod_params.fm_first_down_sample_factor = sdr->sdr_params->sample_rate / DSP_FM_DEMOD_SAMPLE_RATE;
    dsp_params->demod_params.fm_second_down_sample_factor = DSP_FM_DEMOD_SAMPLE_RATE / DSP_SOUND_SAMPLE_RATE;

    // размер ячейки звукового КБ зависит от выбранной частоты ЦАП звуковой карты и частоты обращений к приемнику
    dsp_params->sound_params.rb.cell_size = DSP_SOUND_SAMPLE_RATE / dsp_params->read_params.readout_per_seconds;

    /*
    qDebug() << "read_cell_size = " << dsp_params->read_params.rb.cell_size;
    qDebug() << "fft_max_averages_number = " << dsp_params->fft_params.fft_max_averages_number;
    qDebug() << "first_down_sample_factor = " << dsp_params->demod_params.first_down_sample_factor;
    qDebug() << "demod_cell_size = " << dsp_params->demod_params.rb.cell_size;
    qDebug() << "second_down_sample_factor = " << dsp_params->demod_params.second_down_sample_factor;
    qDebug() << "sound_cell_size = " << dsp_params->sound_params.rb.cell_size;
    */

    prepair_memory(); // выделение памяти в объектах

    emit global_update_interface();
    return 0;
}

// выделение памяти под необходимые КБ
void DSP::prepair_memory()
{
    dsp_params->read_params.realloc_mem();
    dsp_params->flt_params.realloc_mem();
    dsp_params->shift_params.realloc_mem();
    dsp_params->demod_params.realloc_mem(dsp_params->flt_params.rb.cell_size);
    dsp_params->sound_params.realloc_mem();

}

void DSP::create_file_names()
{
    QString base_file_name = QDateTime::currentDateTime().toString("d.M.yyyy(hh;mm;ss)")
                           + "_Freq(" + QString::number(sdr->sdr_params->central_freq)
                           + ")_SampleRate(" + QString::number(sdr->sdr_params->sample_rate) + ")";

    switch(sdr->sdr_params->direct_sampling_idx){
        case 0:
            base_file_name += "_quadrature";
            break;
        case 1:
            base_file_name += "_I branch";
            break;
        case 2:
            base_file_name += "_Q branch";
            break;
    }

    reader_rec->params->file_name = base_file_name + "_reader.wav";
    flt_rec->params->file_name    = base_file_name + "_flt.wav";
    shift_rec->params->file_name  = base_file_name + "_shift.wav";
    sound_rec->params->file_name  = base_file_name + "_sound.wav";
}

void DSP::close_files()
{
    dsp_params->read_params.write_to_file  = false;
    dsp_params->flt_params.write_to_file   = false;
    dsp_params->shift_params.write_to_file = false;
    dsp_params->sound_params.write_to_file = false;

    reader_rec->close_file(sdr->sdr_params->sample_rate, 2);
    flt_rec->close_file(sdr->sdr_params->sample_rate, 2);
    shift_rec->close_file(sdr->sdr_params->sample_rate, 2);
    sound_rec->close_file(DSP_SOUND_SAMPLE_RATE, 1);
}

void DSP::setDynRng(float _dnRng)
{
    dsp_params->fft_params.fft_dynamic_range = _dnRng;
}
