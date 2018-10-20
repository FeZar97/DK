#ifndef DSP_H
#define DSP_H

#include <definitions.h>

#include <filter.h>

#include <simplegraph.h>
#include <reader.h>
#include <mrfiltering.h>
#include <fft_calcer.h>
#include <fft_shifter.h>
#include <wav_recorder.h>

class DSP : public QObject
{
    Q_OBJECT

public:
    simpleGraph         *graph; // экран
    SDR                 *sdr;
    DSP_params          *dsp_params; // параметры
/// ---------------------------------------- воркеры --------------------------------------
    READER              *reader;
    MRFiltering         *flt;
    fft_calcer          *fft;
    fft_shifter         *shifter;
    wav_recorder        *first_wav_rec;
    wav_recorder        *second_wav_rec;
    wav_recorder        *third_wav_rec;
/// ---------------------------------------- потоки ---------------------------------------
    QThread             *reader_thread;
    QThread             *filtration_thread;
    QThread             *fft_thread;
    QThread             *fft_shift_thread;
    QThread             *first_wav_thread;
    QThread             *second_wav_thread;
    QThread             *third_wav_thread;




    explicit DSP(SDR *new_sdr = NULL, simpleGraph *new_graph = NULL);
    ~DSP();

public slots:
    bool                prepair_to_record(SDR *new_sdr);  // КРУТАЯ ВЕЩЬ
    bool                recalc_dsp_params();              // пересчет параметров ЦОС
public:
    void                set_record_flags(bool flag);      // выставление флагов в соответствии с конфигурацией
    void                rename_files();

    void                prepair_memory();                 // выделение памяти под буферы, создание потоков
    void                create_threads();                 // создание потоков
    void                prepair_reader();                 // подготовка reader к началу записи
    void                prepair_mr_filter();              // подготовка mr_filter к началу записи
    void                prepair_fft_shifter();            // подготовка fft_shifter к началу записи
    void                prepair_wav_recorder();           // подготовка wav_recorder к началу записи
    void                make_wav_headers();

    void                create_objects();
    void                create_reader();                  // инициализация объектов
    void                create_filter();                  // инициализация объектов
    void                create_shifter();                 // инициализация объектов
    void                create_fft_calcer();              // инициализация объектов
    void                create_wav_recorders();           // инициализация объектов

    void                start_threads();                  // старт потоков
};

#endif // DSP_H
