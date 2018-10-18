#include "dsp.h"

DSP::DSP(SDR *new_sdr, simpleGraph *new_graph)
{
    if(new_sdr != NULL)
        sdr = new_sdr;
    else
        sdr = new SDR();

    dsp_params = new DSP_params();

    if(new_graph != NULL)
        graph = new_graph;
    else
        graph = new simpleGraph();
}

DSP::~DSP()
{
}

/* перерасчет параметров записи
 * выделение памяти
 * конфигурирование приемника
 * флаги записи
 * создание объектов-воркеров и потоков */
bool DSP::prepair_to_record(SDR *new_sdr)
{
    sdr = new_sdr;
    sdr->set_params_to_device();

    bool result = true;

    result &= recalc_dsp_params();

    set_record_flags(true);             // поготовка флагов

    create_objects();                   // создание потоков и объектов, распределение объектов по потокам
    prepair_memory();                   // выделение памяти в объектах

    // крайняя проверка перед запуском - если возвратится -1, то приемник недоступен => нельзя запускать
    result &= (rtlsdr_set_center_freq(sdr->sdr_params->sdr_ptr, sdr->sdr_params->central_freq) == -1 ? false : true);

    return result;
}

// выставление флагов в соответствии с конфигурацией
void DSP::set_record_flags(bool flag)
{
     dsp_params->read_params->is_recording = flag;
     dsp_params->read_params->emergency_end_recording = false;
     dsp_params->read_params->end_reading = false;

     if(dsp_params->flt_params->is_using)
         dsp_params->read_params->end_filtering = false;
     else
         dsp_params->read_params->end_filtering = true;

     dsp_params->read_params->end_fft = false;
}

void DSP::rename_files()
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

    dsp_params->wav_params->first_file_name  = base_file_name + ".wav";
    dsp_params->wav_params->second_file_name = base_file_name + "_filtering.wav";
    dsp_params->wav_params->third_file_name  = base_file_name + "_real.wav";
}

// создание объектов
void DSP::create_objects()
{
    create_threads(); // создание потоков

    create_reader();
    create_filter();
    create_shifter();
    create_fft_calcer();
    create_wav_recorders();
}
void DSP::create_threads()
{
    reader_thread       = new QThread;
    filtration_thread   = new QThread;
    fft_thread          = new QThread;
    fft_shift_thread    = new QThread;
}
void DSP::create_reader()
{
    reader = new READER(dsp_params, sdr->sdr_params);
    connect(reader_thread, &QThread::started, reader, &READER::start_reading); // начало записи
    reader->moveToThread(reader_thread);
}
void DSP::create_filter()
{
    flt = new MRFiltering(dsp_params, sdr->sdr_params);
    connect(reader, &READER::get_filtration_step, flt, &MRFiltering::get_filtration_step); // переход от чтения к фильтрации
    flt->moveToThread(filtration_thread);
}
void DSP::create_shifter()
{
    shifter = new fft_shifter(dsp_params, sdr->sdr_params);
    connect(reader, &READER::get_shift_step, shifter, &fft_shifter::get_shift_step);
    shifter->moveToThread(fft_shift_thread);
}
void DSP::create_fft_calcer()
{
    fft = new fft_calcer(dsp_params);
    connect(reader, &READER::get_fft_step, fft, &fft_calcer::get_fft_step);
    connect(shifter, &fft_shifter::get_fft_step, fft, &fft_calcer::get_fft_step);
    fft->moveToThread(fft_thread);
}
void DSP::create_wav_recorders()
{
    wav_thread = new QThread;

    wav_rec = new wav_recorder(dsp_params, sdr->sdr_params);
    connect(reader, &READER::write_to_file, wav_rec, &wav_recorder::write_to_file);
    connect(shifter, &fft_shifter::write_to_file, wav_rec, &wav_recorder::write_to_file);
    wav_rec->moveToThread(wav_thread);
}

// пересчет параметров
bool DSP::recalc_dsp_params()
{
    bool success_flag = true;

    dsp_params->read_params->read_rb_cell_size = 2 * sdr->sdr_params->sample_rate / dsp_params->read_params->readout_per_seconds;

    if(dsp_params->read_params->read_rb_cell_size < 1)
        success_flag = false;

    if(dsp_params->flt_params->is_using){
        dsp_params->flt_params->out_sample_rate = sdr->sdr_params->sample_rate * dsp_params->flt_params->up_factor / dsp_params->flt_params->down_factor;
        dsp_params->flt_params->filtration_rb_cell_size = 2 * dsp_params->flt_params->out_sample_rate / dsp_params->read_params->readout_per_seconds;
        if(dsp_params->flt_params->out_sample_rate < 1 || dsp_params->flt_params->filtration_rb_cell_size < 1)
            success_flag = false;
    }

    switch(dsp_params->fft_params->fft_mode){
        case READER_FFT:
            dsp_params->fft_params->fft_input_cell_size = dsp_params->read_params->read_rb_cell_size;
            break;
        case FLT_FFT:
            dsp_params->fft_params->fft_input_cell_size = dsp_params->flt_params->filtration_rb_cell_size;
            break;
        case SHIFT_FFT:
            dsp_params->fft_params->fft_input_cell_size = dsp_params->read_params->read_rb_cell_size;
            break;
    }

    dsp_params->fft_params->fft_max_averages_number = dsp_params->fft_params->fft_input_cell_size / (2 * DSP_FFT_SIZE);
    if(dsp_params->fft_params->fft_max_averages_number < 1)
        success_flag = false;
    dsp_params->fft_params->fft_averages_number = dsp_params->fft_params->fft_max_averages_number;

    return success_flag;
}

// выделение памяти под необходимые КБ, создание потоков
void DSP::prepair_memory()
{
    prepair_reader(); // подготовка reader к началу записи
    prepair_mr_filter(); // подготовка mr_filter к началу записи
    prepair_fft_shifter(); // подготовка fft_shifter к началу записи
    prepair_wav_recorder(); // подготовка wav_recorder к началу записи
}
// подготовка reader к началу записи
void DSP::prepair_reader()
{
    int i;
    dsp_params->read_params->read_rb_cell_idx = 0;

    dsp_params->read_params->read_rb = new Ipp32fc*[DSP_READ_RB_SIZE];
    for(i = 0; i < DSP_READ_RB_SIZE; i++)
        dsp_params->read_params->read_rb[i] = new Ipp32fc[dsp_params->read_params->read_rb_cell_size / 2];

    dsp_params->read_params->read_cell = new Ipp8u[dsp_params->read_params->read_rb_cell_size];
}
void DSP::prepair_mr_filter()
{
    /*
    // если используется этап фильтрации
    if(dsp_params->flt_params->is_using){
        int i;

        dsp_params->flt_params->filtration_rb = new Ipp32fc*[DSP_FLT_RB_SIZE];
        for(i = 0; i < DSP_FLT_RB_SIZE; i++)
            dsp_params->flt_params->filtration_rb[i] = new Ipp32fc[dsp_params->flt_params->filtration_rb_cell_size / 2];

        switch(dsp_params->flt_params->out_sample_rate){

            case 9600:
                dsp_params->flt_params->filter_length = FLT_9600_LENGTH;
                dsp_params->flt_params->filter_taps = new Ipp32f[dsp_params->flt_params->filter_length];
                for(int i = 0; i < dsp_params->flt_params->filter_length; i++)
                    dsp_params->flt_params->filter_taps[i] = filter_9600[i];
                break;

            case 20000:
                dsp_params->flt_params->filter_length = FLT_20000_LENGTH;
                dsp_params->flt_params->filter_taps = new Ipp32f[dsp_params->flt_params->filter_length];
                for(int i = 0; i < dsp_params->flt_params->filter_length; i++)
                    dsp_params->flt_params->filter_taps[i] = filter_20000[i];
                break;
            case 120000:
                dsp_params->flt_params->filter_length = FLT_120000_LENGTH;
                dsp_params->flt_params->filter_taps = new Ipp32f[dsp_params->flt_params->filter_length];
                for(int i = 0; i < dsp_params->flt_params->filter_length; i++){
                    dsp_params->flt_params->filter_taps[i] = filter_120000[i];
                }
            case 160000:
                dsp_params->flt_params->filter_length = FLT_160000_LENGTH;
                dsp_params->flt_params->filter_taps = new Ipp32f[dsp_params->flt_params->filter_length];
                for(int i = 0; i < dsp_params->flt_params->filter_length; i++){
                    dsp_params->flt_params->filter_taps[i] = filter_160000[i];
                }
                break;
        }

        dsp_params->flt_params->temp_32fc = new Ipp32f[dsp_params->read_params->read_rb_cell_size];

        dsp_params->flt_params->temp_32f_re = new Ipp32f[dsp_params->read_params->read_rb_cell_size / 2];
        dsp_params->flt_params->temp_32f_im = new Ipp32f[dsp_params->read_params->read_rb_cell_size / 2];

        dsp_params->flt_params->flt_32f_re = new Ipp32f[dsp_params->flt_params->filtration_rb_cell_size / 2];
        dsp_params->flt_params->flt_32f_im = new Ipp32f[dsp_params->flt_params->filtration_rb_cell_size / 2];

        dsp_params->flt_params->delay_re = new Ipp32f[(dsp_params->flt_params->filter_length + dsp_params->flt_params->up_factor - 1 ) / dsp_params->flt_params->up_factor];
        dsp_params->flt_params->delay_im = new Ipp32f[(dsp_params->flt_params->filter_length + dsp_params->flt_params->up_factor - 1 ) / dsp_params->flt_params->up_factor];

        // инициализация фильтра
        int specSize, bufSize;
        ippsFIRMRGetSize(dsp_params->flt_params->filter_length,
                         dsp_params->flt_params->up_factor,
                         dsp_params->flt_params->down_factor,
                         ipp32f,
                         &specSize,
                         &bufSize);

        dsp_params->flt_params->pSpec = (IppsFIRSpec_32f*)ippsMalloc_8u(specSize);
        dsp_params->flt_params->buf = ippsMalloc_8u(bufSize);

        ippsFIRMRInit_32f(*(&dsp_params->flt_params->filter_taps),
                          dsp_params->flt_params->filter_length,
                          dsp_params->flt_params->up_factor,
                          0,
                          dsp_params->flt_params->down_factor,
                          0,
                          dsp_params->flt_params->pSpec);
    }
*/
}
void DSP::prepair_fft_shifter()
{
    int i;
    dsp_params->shift_params->shift_rb_cell_idx = 0;

    dsp_params->shift_params->shift_rb = new Ipp32fc*[DSP_SHIFT_RB_SIZE];
    for(i = 0; i < DSP_SHIFT_RB_SIZE; i++)
        dsp_params->shift_params->shift_rb[i] = new Ipp32fc[dsp_params->read_params->read_rb_cell_size / 2];

    dsp_params->shift_params->complex_sin = new Ipp32fc[dsp_params->read_params->read_rb_cell_size / 2];
}
void DSP::prepair_wav_recorder()
{
    rename_files();

    if(dsp_params->wav_params->use_first_file){
        dsp_params->wav_params->first_file_pos = 0;
        dsp_params->wav_params->first_file_total_size = times[dsp_params->read_params->rec_time_idx] * sdr->sdr_params->sample_rate * 2;
        qDebug() << "total = " << dsp_params->wav_params->first_file_total_size << endl;
        dsp_params->wav_params->first_file.setFileName(dsp_params->wav_params->first_file_name);
        dsp_params->wav_params->first_file.open(QIODevice::WriteOnly);
    }

    wav_rec->make_wav_headers();

    if(dsp_params->wav_params->use_first_file)
        dsp_params->wav_params->first_file.write((char*)(&dsp_params->wav_params->first_header), sizeof(WAVEHEADER));
}

// старт потоков
void DSP::start_threads()
{
    wav_thread->start();
    fft_shift_thread->start();
    fft_thread->start();
    //filtration_thread->start();
    reader_thread->start();
}
