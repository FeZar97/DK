#include "dsp.h"

DSP::DSP(SDR *new_sdr, simpleGraph *new_graph)
{
    if(new_sdr != nullptr)
        sdr = new_sdr;
    else
        sdr = new SDR();

    dsp_params = new DSP_params();

    if(new_graph != nullptr)
        graph = new_graph;
    else
        graph = new simpleGraph();

    reader          = nullptr;
    flt             = nullptr;
    fft             = nullptr;
    shifter         = nullptr;
    first_wav_rec   = nullptr;
    second_wav_rec  = nullptr;
    third_wav_rec   = nullptr;
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

    bool result = recalc_dsp_params();

    set_record_flags(true);             // поготовка флагов

    create_objects();                   // создание потоков и объектов, распределение объектов по потокам
    prepair_memory();                   // выделение памяти в объектах

    // крайняя проверка перед запуском - если возвратится -1, то приемник недоступен => нельзя запускать
    sdr->sdr_params->is_open = (rtlsdr_set_center_freq(sdr->sdr_params->sdr_ptr, sdr->sdr_params->central_freq) == -1 ? false : true);

    qDebug() << "is open: " << sdr->sdr_params->is_open;

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
    QString base_file_name = dsp_params->wav_params->directory
                           + QDateTime::currentDateTime().toString("d.M.yyyy(hh;mm;ss)")
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

    first_wav_rec->params->file_name  = base_file_name + ".wav";
    second_wav_rec->params->file_name = base_file_name + "_filtering.wav";
    third_wav_rec->params->file_name  = base_file_name + "_real.wav";
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
    create_sounder();
}
void DSP::create_threads()
{
    reader_thread       = new QThread;
    filtration_thread   = new QThread;
    fft_thread          = new QThread;
    fft_shift_thread    = new QThread;
    sound_thread        = new QThread;

    qDebug() << "create threads...";
}
void DSP::create_reader()
{
    reader = new READER(dsp_params, sdr->sdr_params);
    connect(reader_thread, &QThread::started, reader, &READER::start_reading); // начало записи
    reader->moveToThread(reader_thread);

    qDebug() << "create reader...";
}
void DSP::create_filter()
{
    flt = new MRFiltering(dsp_params, sdr->sdr_params);
    connect(reader, &READER::get_filtration_step, flt, &MRFiltering::get_filtration_step); // переход от чтения к фильтрации
    flt->moveToThread(filtration_thread);

    qDebug() << "create filter...";
}
void DSP::create_shifter()
{
    shifter = new fft_shifter(dsp_params, sdr->sdr_params);
    connect(reader, &READER::get_shift_step, shifter, &fft_shifter::get_shift_step);
    connect(flt, &MRFiltering::get_shift_step, shifter, &fft_shifter::get_shift_step);
    shifter->moveToThread(fft_shift_thread);

    qDebug() << "create shifter...";
}
void DSP::create_fft_calcer()
{
    fft = new fft_calcer(dsp_params);
    connect(reader, &READER::get_fft_step, fft, &fft_calcer::get_fft_step);
    connect(flt, &MRFiltering::get_fft_step, fft, &fft_calcer::get_fft_step);
    connect(shifter, &fft_shifter::get_fft_step, fft, &fft_calcer::get_fft_step);
    fft->moveToThread(fft_thread);

    qDebug() << "create fft calcer...";
}
void DSP::create_wav_recorders()
{
    wav_thread = new QThread;

    first_wav_rec = new wav_recorder();
    connect(reader, &READER::write_to_file, first_wav_rec, &wav_recorder::write_to_file);

    second_wav_rec = new wav_recorder();
    connect(flt, &MRFiltering::write_to_file, second_wav_rec, &wav_recorder::write_to_file);

    third_wav_rec = new wav_recorder();
    connect(shifter, &fft_shifter::write_to_file, third_wav_rec, &wav_recorder::write_to_file);

    first_wav_rec->moveToThread(wav_thread);
    second_wav_rec->moveToThread(wav_thread);
    third_wav_rec->moveToThread(wav_thread);

    qDebug() << "create wav recorders...";
}

void DSP::create_sounder()
{
    sounder = new sound_maker(dsp_params, sdr->sdr_params);
    connect(reader, &READER::get_sound_step, sounder, &sound_maker::get_sound_step);
    connect(flt, &MRFiltering::get_sound_step, sounder, &sound_maker::get_sound_step);
    connect(shifter, &fft_shifter::get_sound_step, sounder, &sound_maker::get_sound_step);
    sounder->moveToThread(sound_thread);

    qDebug() << "create sounder...";
}

// пересчет параметров
bool DSP::recalc_dsp_params()
{
    bool success_flag = true;

    if(!dsp_params->read_params->readout_per_seconds)
        dsp_params->read_params->readout_per_seconds = 10;

    dsp_params->read_params->read_rb_cell_size = 2 * sdr->sdr_params->sample_rate / dsp_params->read_params->readout_per_seconds;
    qDebug() << "read_rb_cell_size = " << dsp_params->read_params->read_rb_cell_size;

    if(dsp_params->read_params->read_rb_cell_size < 1)
        success_flag = false;

    if(dsp_params->flt_params->is_using)
        dsp_params->flt_params->filtration_rb_cell_size = dsp_params->read_params->read_rb_cell_size;

    switch(dsp_params->fft_params->fft_mode){

        case READER_FFT:
            dsp_params->fft_params->fft_input_cell_size = dsp_params->read_params->read_rb_cell_size;
            break;

        case FLT_FFT:
            dsp_params->fft_params->fft_input_cell_size = dsp_params->read_params->read_rb_cell_size;
            break;

        case SHIFT_FFT:
            dsp_params->fft_params->fft_input_cell_size = dsp_params->read_params->read_rb_cell_size;
            break;
    }

    qDebug() << "fft_input_cell_size = " << dsp_params->fft_params->fft_input_cell_size;

    dsp_params->fft_params->fft_max_averages_number = dsp_params->fft_params->fft_input_cell_size / (2 * DSP_FFT_SIZE);
    qDebug() << "fft_max_averages_number = " << dsp_params->fft_params->fft_max_averages_number;

    if(dsp_params->fft_params->fft_max_averages_number < 1)
        success_flag = false;

    dsp_params->fft_params->fft_averages_number = dsp_params->fft_params->fft_max_averages_number;

    qDebug() << "recalc_dsp_params... " << success_flag << endl;

    return success_flag;
}

// выделение памяти под необходимые КБ, создание потоков
void DSP::prepair_memory()
{
    prepair_reader(); // подготовка reader к началу записи
    prepair_mr_filter(); // подготовка mr_filter к началу записи
    prepair_fft_shifter(); // подготовка fft_shifter к началу записи
    prepair_sound_maker(); // подготовка sounder к началу записи
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
    // если используется этап фильтрации
    if(dsp_params->flt_params->is_using){

        int i;

        dsp_params->flt_params->filtration_rb = new Ipp32fc*[DSP_FLT_RB_SIZE];
        for(i = 0; i < DSP_FLT_RB_SIZE; i++)
            dsp_params->flt_params->filtration_rb[i] = new Ipp32fc[dsp_params->read_params->read_rb_cell_size / 2];
    }
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

    if(dsp_params->read_params->use_first_file){
        first_wav_rec->params->pos = 0;
        first_wav_rec->params->total_size = times[dsp_params->read_params->rec_time_idx] * sdr->sdr_params->sample_rate * 2;
        first_wav_rec->params->file.setFileName(first_wav_rec->params->file_name);
        first_wav_rec->params->file.open(QIODevice::WriteOnly);
        first_wav_rec->params->input_cell_size = dsp_params->read_params->read_rb_cell_size;
        first_wav_rec->params->out_buf = new Ipp8s[dsp_params->read_params->read_rb_cell_size];
    }

    if(dsp_params->read_params->use_second_file){
        second_wav_rec->params->pos = 0;
        second_wav_rec->params->total_size = times[dsp_params->read_params->rec_time_idx] * sdr->sdr_params->sample_rate * 2;
        second_wav_rec->params->file.setFileName(second_wav_rec->params->file_name);
        second_wav_rec->params->file.open(QIODevice::WriteOnly);
        second_wav_rec->params->input_cell_size = dsp_params->read_params->read_rb_cell_size;
        second_wav_rec->params->out_buf = new Ipp8s[dsp_params->read_params->read_rb_cell_size];
    }

    if(dsp_params->read_params->use_third_file){
        third_wav_rec->params->pos = 0;
        third_wav_rec->params->total_size = times[dsp_params->read_params->rec_time_idx] * sdr->sdr_params->sample_rate * 2;
        third_wav_rec->params->file.setFileName(third_wav_rec->params->file_name);
        third_wav_rec->params->file.open(QIODevice::WriteOnly);
        third_wav_rec->params->input_cell_size = dsp_params->read_params->read_rb_cell_size;
        third_wav_rec->params->out_buf = new Ipp8s[dsp_params->read_params->read_rb_cell_size];
    }

    make_wav_headers();

    if(dsp_params->read_params->use_first_file)
        first_wav_rec->params->file.write((char*)(&first_wav_rec->params->header), sizeof(WAVEHEADER));
    if(dsp_params->read_params->use_second_file)
        second_wav_rec->params->file.write((char*)(&second_wav_rec->params->header), sizeof(WAVEHEADER));
    if(dsp_params->read_params->use_third_file)
        third_wav_rec->params->file.write((char*)(&third_wav_rec->params->header), sizeof(WAVEHEADER));
}
void DSP::prepair_sound_maker()
{
    int i;
    dsp_params->sound_params->sound_rb_cell_idx = 0;

    dsp_params->sound_params->sound_rb = new Ipp16s*[DSP_SOUND_RB_SIZE];
    for(i = 0; i < DSP_SOUND_RB_SIZE; i++)
        dsp_params->sound_params->sound_rb[i] = new Ipp16s[dsp_params->read_params->read_rb_cell_size / 2];
}

void DSP::make_wav_headers()
{
    // первый файл
    memcpy(first_wav_rec->params->header.chunkId, "RIFF", 4); // 0x52494646 в big-endian представлении
    first_wav_rec->params->header.chunkSize = first_wav_rec->params->total_size - 8; // data_size + 36
    memcpy(first_wav_rec->params->header.format, "WAVE", 4); // (0x57415645 в big-endian представлении)
    memcpy(first_wav_rec->params->header.subchunk1Id, "fmt ", 4); // (0x666d7420 в big-endian представлении)
    first_wav_rec->params->header.subchunk1Size = 16; // 16 для формата PCM
    first_wav_rec->params->header.audioFormat = 1;
    first_wav_rec->params->header.numChannels = 2;
    first_wav_rec->params->header.sampleRate = sdr->sdr_params->sample_rate;
    first_wav_rec->params->header.charRate = first_wav_rec->params->header.sampleRate * first_wav_rec->params->header.numChannels * sizeof(char);
    first_wav_rec->params->header.blockAlign = first_wav_rec->params->header.numChannels * sizeof(char);
    first_wav_rec->params->header.bitsPerSample = 8;
    memcpy(first_wav_rec->params->header.subchunk2Id, "data", 4);
    first_wav_rec->params->header.subchunk2Size = first_wav_rec->params->total_size;


    // второй файл
    memcpy(second_wav_rec->params->header.chunkId, "RIFF", 4); // 0x52494646 в big-endian представлении
    second_wav_rec->params->header.chunkSize = second_wav_rec->params->total_size - 8; // data_size + 36
    memcpy(second_wav_rec->params->header.format, "WAVE", 4); // (0x57415645 в big-endian представлении)
    memcpy(second_wav_rec->params->header.subchunk1Id, "fmt ", 4); // (0x666d7420 в big-endian представлении)
    second_wav_rec->params->header.subchunk1Size = 16; // 16 для формата PCM
    second_wav_rec->params->header.audioFormat = 1;
    second_wav_rec->params->header.numChannels = 2;
    second_wav_rec->params->header.sampleRate = sdr->sdr_params->sample_rate;
    second_wav_rec->params->header.charRate = second_wav_rec->params->header.sampleRate * second_wav_rec->params->header.numChannels * sizeof(char);
    second_wav_rec->params->header.blockAlign = second_wav_rec->params->header.numChannels * sizeof(char);
    second_wav_rec->params->header.bitsPerSample = 8;
    memcpy(second_wav_rec->params->header.subchunk2Id, "data", 4);
    second_wav_rec->params->header.subchunk2Size = second_wav_rec->params->total_size;


    // третий файл
    memcpy(third_wav_rec->params->header.chunkId, "RIFF", 4); // 0x52494646 в big-endian представлении
    third_wav_rec->params->header.chunkSize = third_wav_rec->params->total_size - 8; // data_size + 36
    memcpy(third_wav_rec->params->header.format, "WAVE", 4); // (0x57415645 в big-endian представлении)
    memcpy(third_wav_rec->params->header.subchunk1Id, "fmt ", 4); // (0x666d7420 в big-endian представлении)
    third_wav_rec->params->header.subchunk1Size = 16; // 16 для формата PCM
    third_wav_rec->params->header.audioFormat = 1;
    third_wav_rec->params->header.numChannels = 2;
    third_wav_rec->params->header.sampleRate = sdr->sdr_params->sample_rate;
    third_wav_rec->params->header.charRate = third_wav_rec->params->header.sampleRate * third_wav_rec->params->header.numChannels * sizeof(char);
    third_wav_rec->params->header.blockAlign = third_wav_rec->params->header.numChannels * sizeof(char);
    third_wav_rec->params->header.bitsPerSample = 8;
    memcpy(third_wav_rec->params->header.subchunk2Id, "data", 4);
    third_wav_rec->params->header.subchunk2Size = third_wav_rec->params->total_size;
}

// старт потоков
void DSP::start_threads()
{
    sound_thread->start();
    wav_thread->start();
    fft_shift_thread->start();
    fft_thread->start();
    filtration_thread->start();
    reader_thread->start();
}
