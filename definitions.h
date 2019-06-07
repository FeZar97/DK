/*
    This file is part of DigitalKalmar(Кальмар-SDR)

    DigitalKalmar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DigitalKalmar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DigitalKalmar.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef DEFINITIONS
#define DEFINITIONS

#define VERSION  "v0.19.5.27"
#define NAME     "Цифровой тракт РПУ \"Кальмар\" "

#include <cmath>
#include <float.h>
#include <windows.h>

#include <QAudioOutput>
#include <QDateTime>
#include <QDebug>
#include <QDesktopWidget>
#include <QEvent>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QMessageBox>
#include <QObject>
#include <QPainter>
#include <QResizeEvent>
#include <QSettings>
#include <QString>
#include <QThread>
#include <QTime>
#include <QTimer>

#include <inpout32.h>
#include <rtl-sdr.h>

#include <ippCustom.h>

typedef void	(__stdcall *lpOutc)(unsigned short, unsigned char);

/// ----------------------------------enums---------------------------------------
// типы действия при зажатии ЛКМ в области окна программы
enum MouseType{Move,
               None};

// адреса LPT портов
enum LPT_ADDRESS{LPT_1 = 0x378,
                 LPT_2 = 0xDD00,
                 LPT_3 = 0xE010,
                 LPT_4 = 0xDC00,
                 LPT_5 = 0xD880};

// секции контактов на DB25
enum LPT_SECTION{LPT_DATA_REG,
                 LPT_STATE_REG,
                 LPT_CONTROL_REG,
                 LPT_EPP_ADDRESS,
                 LPT_EPP_DATA};

// конфигурация трактов
enum RPU_CONFIG
    {FOUR_CHANNEL,            // тип 1 ~ 4 канала,
     THREE_ONE_CHANNEL,       // тип 2 ~ 3 канала + 1 канал,
     TWO_TWO_CHANNEL,         // тип 3 ~ 2 канала + 2 канала,
     TWO_ONE_ONE_CHANNEL,     // тип 4 ~ 2 канала + 1 канал + 1 канал,
     ONE_ONE_ONE_ONE_CHANNEL};// тип 5 ~ 1 канал  + 1 канал + 1 канал + 1 канал

// модулирующий сигнал
enum MOD_SIGNAL{SINUS,
                RADIOIMP};

// внутренний/внешний калибратор
enum KALIB_TYPE{INTERNAL,
                EXTERNAL};

// состояние калибратора
enum STATUS{OFF,
            ON};

// полосы ПЧ
enum IF_BAND{FIRST_BAND,   //   3 кГц,
             SECOND_BAND,  //   8 кГц,
             THIRD_BAND,   //  20 кГц,
             FOURTH_BAND}; // 150 кГц,

// номер окна тракта
enum NUMERALS{ZERO,
              FIRST,
              SECOND,
              THIRD,
              FOURTH,
              FIFTH};

// отображаемый спектр
enum FFT_MODE{READER_FFT, // спектр сигнала с АЦП
              FLT_FFT,    // спектр отфильтрованного сигнала
              SHIFT_FFT,  // спектр сдвинутого сигнала
              DEMOD_FFT}; // спектр звукового сигнала

// оконные функции
enum WINDOW{NONE,
            BARLETT,
            BLACKMANN,
            HAMMING,
            HANN};

// виды демодуляции
enum DEMOD_MODE{AM,
                USB,
                LSB,
                FM,
                BPSK,
                QPSK};

// ---------------------------------------РПУ---------------------------------------------
#define     RPU_DEFAULT_LPT                         LPT_1

// ПАРАМЕТРЫ КАЛИБРАТОРА
#define     RPU_DEFAULT_KALIB_SIGNAL_TYPE           SINUS                               // синусоида
#define     RPU_MIN_KALIB_MOD_FREQ                  1000                                // 1 кГц
#define     RPU_MAX_KALIB_MOD_FREQ                  50000                               // 50 кГц
#define     RPU_DEFAULT_KALIB_MOD_FREQ              10000                               // 10 кГц
#define     RPU_DEFAULT_KALIB_ATT_IDX               0                                   // 0 дБ
#define     RPU_DEFAULT_KALIB_EXIT                  INTERNAL                            // внутренний
#define     RPU_DEFAULT_KALIB_STATUS                OFF                                 // выключен
#define     RPU_KALIB_ATT_STEP                      2                                   // шаг сетки аттенюатора калибратора (дБ)

//  режим работы РПУ
#define     RPU_DEFAULT_CONFIG                      FOUR_CHANNEL                        // 4-х канальный режим
#define     RPU_MIN_CONFIG                          FOUR_CHANNEL                        // 4-х канальный режим
#define     RPU_MAX_CONFIG                          ONE_ONE_ONE_ONE_CHANNEL             // 1 канал  + 1 канал + 1 канал + 1 канал

// параметры трактов
#define     RPU_DEFAULT_TRACT_FREQ                  15000000                            // 15 Мгц - частота по умолчанию
#define     RPU_MAX_TRACT_FREQ                      31999999                            // 31,999999 Мгц - верхняя чатсота
#define     RPU_MIN_TRACT_FREQ                      150000                              // 150 кГц - нижняя частота

#define     RPU_DEFAULT_IN_ATT_IDX                  OFF                                 // по умолчанию входной аттенюатор отключен
#define     RPU_IN_ATT_STEP                         10                                  // шаг сетки входного аттенюатора (дБ)
#define     RPU_MAX_IN_ATT_IDX                      1                                   // максимальный индекс элемента из выпадающего списка входного аттенюатора
#define     RPU_MIN_IN_ATT_IDX                      0                                   // минимальный индекс элемента из выпадающего списка входного аттенюатора

#define     RPU_DEFAULT_HF_ATT_IDX                  OFF                                 // по умолчанию ВЧ аттенюатор отключен
#define     RPU_HF_ATT_STEP                         6                                   // шаг сетки аттенюатора ВЧ (дБ)
#define     RPU_MAX_HF_ATT_IDX                      7                                   // максимальный индекс элемента из выпадающего списка ВЧ аттенюатора
#define     RPU_MIN_HF_ATT_IDX                      0                                   // минимальный индекс элемента из выпадающего списка ВЧ аттенюатора

#define     RPU_DEFAULT_IF_ATT_IDX                  OFF                                 // выключен
#define     RPU_IF_ATT_STEP                         2                                   // шаг сетки аттенюатора ПЧ (дБ)
#define     RPU_MAX_IF_ATT_IDX                      15                                  // максимальный индекс элемента из выпадающего списка ПЧ аттенюатора
#define     RPU_MIN_IF_ATT_IDX                      0                                   // минимальный индекс элемента из выпадающего списка ВЧ аттенюатора

#define     RPU_DEFAULT_IF_BAND_IDX                 0                                   // 3 кГц
#define     RPU_MAX_IF_BAND_IDX                     3                                   // максимальный индекс элемента из выпадающего списка ширины полосы ПЧ
#define     RPU_MIN_IF_BAND_IDX                     0                                   // минимальный индекс элемента из выпадающего списка ширины полосы ПЧ

#define     RPU_DEFAULT_ACTIVE_DIGIT                ZERO                                // индекс выбранного разряда по умолчанию

#define     RPU_NUMBER_OF_PRESELECTORS              10                                  // количество преселекторов
// ---------------------------------------SDR---------------------------------------------
#define     SDR_MAX_TUNER_FREQ                      2100000000
#define     SDR_MIN_TUNER_FREQ                      100000

#define     SDR_DEFAULT_INDEX                       0
#define     SDR_DEFAULT_CENTRAL_FREQ                104200000
#define     SDR_DEFAULT_GAIN_IDX                    1
#define     SDR_DEFAULT_RTL_AGC                     false
#define     SDR_DEFAULT_SAMPLE_RATE                 240000
#define     SDR_DEFAULT_DIRECT_SAMPLING_IDX         0

// ---------------------------------------DSP---------------------------------------------

#define     DSP_READOUT_PER_SECONDS                 10

#define     DSP_FLT_LENGTH                          701
#define     DSP_FLT_DEFAULT_BW                      20000
#define     DSP_FLT_DEFAULT_R_FREC                  0.25

#define     DSP_FFT_ORDER                           10
#define     DSP_FFT_SIZE                            1024

#define     DSP_FFT_DEFAULT_DYNAMIC_RANGE           30
#define     DSP_FFT_DEFAULT_NOISE_LEVEL             -10
#define     DSP_FFT_DEFAULT_NULL_BIN                false
#define     DSP_FFT_DEFAULT_MODE                    READER_FFT
#define     DSP_FFT_DEFAULT_AVERAGE_NUMBER          8
#define     DSP_FFT_DEFAULT_INVERSION               false
#define     DSP_DEFAULT_FFT_WINDOW                  NONE
#define     DSP_DEFAULT_WIN_ALPHA                   0.5
#define     DSP_DEFAULT_FFT_STYLE_IDX               0
#define     DSP_DEFAULT_FFT_GRID_IDX                0
#define     DSP_DEFAULT_FFT_SONO_PALETTE_IDX        0
#define     DSP_DEFAULT_FFT_FLT_ALPHA               0.2
#define     DSP_COMPENSATE_BUF_SIZE                 10

#define     DSP_DEMOD_DEFAULT_MODE                  FM
#define     DSP_FM_DEMOD_SAMPLE_RATE                192000

#define     DSP_SOUND_FLT_HIGH_FREQ                 16000
#define     DSP_SOUND_SAMPLE_RATE                   48000
#define     DSP_SOUND_DEFAULT_VOLUME                50

#define     DSP_WAV_BITS_PER_SAMPLE                 16

// размеры кольцевых буферов
#define     DSP_READ_RB_SIZE                        10
#define     DSP_FLT_RB_SIZE                         10
#define     DSP_SHIFT_RB_SIZE                       10
#define     DSP_DEMOD_RB_SIZE                       10
#define     DSP_SOUND_RB_SIZE                       10

// размеры виджетов
#define     UI_DEFAULT_SONO_WIDTH                   1024
#define     UI_DEFAULT_SONO_HEIGHT                  776
#define     UI_TOP_PANEL_HEIGHT_PX                  50

// набор КУ [дБ * 10]
const static int gains[] = {0, 9, 14, 27, 37, 77, 87, 125, 144, 157, 166, 197, 207, 229, 254, 280, 297, 328, 338, 364, 372, 386, 402, 421, 434, 439, 445, 480};

// частоты дискретизации АЦП
const static int sample_rates[] = {240000, 2496000};

// цетральные частоты пресекторов
const static unsigned int preselectors_central_freqs[] = {600000, 1250000, 1850000, 2700000, 3800000, 5250000, 7350000, 12150000, 17550000, 26350000};

// стандартный заголовок *.wav файла
struct WAVEHEADER
{
    // WAV-формат начинается с RIFF-заголовка:
    // Содержит символы "RIFF" в ASCII кодировке
    // (0x52494646 в big-endian представлении)
    char chunkId[4];

    // 36 + subchunk2Size, или более точно:
    // 4 + (8 + subchunk1Size) + (8 + subchunk2Size)
    // Это оставшийся размер цепочки, начиная с этой позиции.
    // Иначе говоря, это размер файла - 8, то есть,
    // исключены поля chunkId и chunkSize.
    unsigned long chunkSize;

    // Содержит символы "WAVE"
    // (0x57415645 в big-endian представлении)
    char format[4];

    // Формат "WAVE" состоит из двух подцепочек: "fmt " и "data":
    // Подцепочка "fmt " описывает формат звуковых данных:

    // Содержит символы "fmt "
    // (0x66 6d 74 20 в big-endian представлении)
    char subchunk1Id[4];

    // 16 для формата PCM.
    // Это оставшийся размер подцепочки, начиная с этой позиции.
    unsigned long subchunk1Size = 16;

    // Аудио формат, полный список можно получить здесь http://audiocoding.ru/wav_formats.txt
    // Для PCM = 1 (то есть, Линейное квантование).
    // Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
    unsigned short audioFormat = 1;

    // Количество каналов. Моно = 1, Стерео = 2 и т.д.
    unsigned short numChannels;

    // Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
    unsigned long sampleRate;

    // sampleRate * numChannels * bitsPerSample/8
    unsigned long charRate;

    // numChannels * bitsPerSample/8
    // Количество байт для одного сэмпла, включая все каналы.
    unsigned short blockAlign;

    // Так называемая "глубиная" или точность звучания. 8 бит, 16 бит и т.д.
    unsigned short bitsPerSample;

    // Подцепочка "data" содержит аудио-данные и их размер.

    // Содержит символы "data"
    // (0x64617461 в big-endian представлении)
    char subchunk2Id[4];

    // numSamples * numChannels * bitsPerSample/8
    // Количество байт в области данных.
    unsigned long subchunk2Size;

    // Далее следуют непосредственно Wav данные.
};

template <class T, size_t BUF_SIZE>
class RING_BUFFER{
public:
    size_t cell_size;
    size_t cell_idx;

    T*     ring_buffer[BUF_SIZE];

    explicit RING_BUFFER(): cell_size{0},
                            cell_idx{0},
                            ring_buffer{nullptr} {}

    ~RING_BUFFER(){
        delete_buf();
    }

    void delete_buf(){
        for(uint i = 0; i < BUF_SIZE; i++){
            delete[] ring_buffer[i];
            ring_buffer[i] = nullptr;
        }
    }

    void realloc_mem(){
        delete_buf();

        for(uint i = 0; i < BUF_SIZE; i++)
            ring_buffer[i] = new T[cell_size];
    }

    T* get_current_cell(){
        return ring_buffer[cell_idx];
    }

    void increase_cnt(){
        if(cell_idx == (BUF_SIZE - 1))
            cell_idx = 0;
        else
            cell_idx += 1;
        //cell_idx = (cell_idx == (BUF_SIZE - 1) ? 0 : (cell_idx + 1));
    }

    T& operator[](size_t i){
        return ring_buffer[i];
    }
};


// параметры SDR приемника
class SDR_params{
public:
    QVector<int>  findedIdxs;
    QString       sdrInfo;

    int           index;               // номер используемого SDR приемника

    int           central_freq;        // центральная частота
    int           gain_idx;            // индекс КУ из набора gains
    bool          rtl_agc;             // усиление на чипе realtek
    int           sample_rate;         // частота дискретизации АЦП
    int           direct_sampling_idx; // режим оцифровки

    rtlsdr_dev_t  *sdr_ptr;
    bool          is_open;             // флаг готовности приемника
    bool          is_already_open;     // флаг занятости приемника другим процессом

    SDR_params(): sdrInfo{""},
                  index{0},
                  central_freq{SDR_DEFAULT_CENTRAL_FREQ},
                  gain_idx {SDR_DEFAULT_GAIN_IDX},
                  rtl_agc {SDR_DEFAULT_RTL_AGC},
                  sample_rate{SDR_DEFAULT_SAMPLE_RATE},
                  direct_sampling_idx{SDR_DEFAULT_DIRECT_SAMPLING_IDX},
                  sdr_ptr{nullptr},
                  is_open{false},
                  is_already_open{false}{}
};

// параметры чтения
class READ_params
{
public:
    RING_BUFFER<Ipp32fc, DSP_READ_RB_SIZE>  rb;
    Ipp8u               *read_cell;              // временный буфер

    bool                is_receiving;            // флаг активной записи
    int                 readout_per_seconds;     // ФПС

    bool                write_to_file;           // запись в файл

    READ_params(): is_receiving{false},
                   readout_per_seconds{DSP_READOUT_PER_SECONDS},
                   write_to_file{false} {}

    void realloc_mem(){
        rb.realloc_mem();

        if(read_cell){
            delete[] read_cell;
            read_cell = nullptr;
        }
        read_cell = new Ipp8u[rb.cell_size];
    }
};

// параметры фильтрации
class FLT_params
{
public:
    int              fltBw; // ширина полосы фильтрации
    double           fltRFrec; // относительная полоса фильтрации (0; 0.5)

    RING_BUFFER<Ipp32fc, DSP_FLT_RB_SIZE>      rb;

    IppsFIRSpec_32fc *flt_spec;
    Ipp32fc          flt_taps32[DSP_FLT_LENGTH]; // ИХ
    Ipp32fc          delay_line[DSP_FLT_LENGTH - 1]; // линия задержки
    Ipp8u            *buf;
    bool             new_coeffs_ready;          // флаг готовности новых коэф-тов фильтра

    bool             write_to_file;       // запись в файл

    FLT_params():    fltBw{DSP_FLT_DEFAULT_BW},
                     fltRFrec{DSP_FLT_DEFAULT_R_FREC},
                     flt_spec{nullptr},
                     buf{nullptr},
                     new_coeffs_ready{false},
                     write_to_file{false}
    {
        ippsZero_32fc(delay_line, DSP_FLT_LENGTH - 1);
        int buf_size, spec_size;

        Ipp64f tmp64[DSP_FLT_LENGTH];
        Ipp32f tmp32[DSP_FLT_LENGTH];

        ippsFIRGenGetBufferSize(DSP_FLT_LENGTH, &buf_size);
        buf = new Ipp8u[buf_size];

        ippsFIRGenLowpass_64f(fltRFrec, tmp64, DSP_FLT_LENGTH, ippWinBlackman, ippTrue, buf);
        ippsConvert_64f32f(tmp64, tmp32, DSP_FLT_LENGTH);

        delete[] buf;
        buf = nullptr;

        ippsFIRSRGetSize(DSP_FLT_LENGTH, ipp32fc, &spec_size, &buf_size);
        flt_spec = reinterpret_cast<IppsFIRSpec_32fc*>(new Ipp8u[spec_size]);
        buf = new Ipp8u[buf_size];

        ippsRealToCplx_32f(tmp32, nullptr, flt_taps32, DSP_FLT_LENGTH);

        ippsFIRSRInit_32fc(flt_taps32, DSP_FLT_LENGTH, ippAlgAuto, flt_spec);
    }

    void recalc_flt_coeffs(){
        Ipp64f tmp64[DSP_FLT_LENGTH];
        Ipp32f tmp32[DSP_FLT_LENGTH];
        ippsFIRGenLowpass_64f(fltRFrec, tmp64, DSP_FLT_LENGTH, ippWinBlackman, ippTrue, buf);
        ippsConvert_64f32f(tmp64, tmp32, DSP_FLT_LENGTH);
        ippsRealToCplx_32f(tmp32, nullptr, flt_taps32, DSP_FLT_LENGTH);
        new_coeffs_ready = true;
    }

    void realloc_mem(){
        rb.realloc_mem();
    }
};

// параметры БПФ
class FFT_params
{
public:
    float               fft_dynamic_range;         // динамический диапазон
    float               fft_noise_level;           // уровень шума
    bool                null_bin;                  // отображние нулевого бина

    Ipp32f              dc_offset_gain;            // корректировка амплитуды
    Ipp32f              compensateBuf[DSP_COMPENSATE_BUF_SIZE];
    int                 compensatePtr;

    FFT_MODE            fft_mode;                  // спектр какого сигнала выводить
    int                 fft_input_cell_size;       // размер входного массива
    int                 fft_averages_number;       // кол-во усреднений спектра
    int                 fft_max_averages_number;   // максимально допустимое число усреднений
    bool                fft_inversion;             // флаг инверсии спектра
    WINDOW              fft_current_window;        // используемое окно
    float               fft_win_alpha;             // параметр оконной функции
    int                 fft_stlIdx;                // индекс стиля отображения спектра
    int                 fft_frqGrdIdx;             // индекс сетки частот, используемой для подписи
    int                 fft_sonoPltIdx;            // индекс палитры сонограммы
    double              fft_fltBwAlpha;            // прозрачность полосы фильтрации

    Ipp32f              fft_res[DSP_FFT_SIZE];     // энергетический спектр

    IppsFFTSpec_C_32fc  *pFftSpec_fc;              // спецификация fc
    IppsFFTSpec_R_32f   *pFftSpec_f;               // спецификация r
    Ipp8u               *fft_buf_fc;               // буфер БПФ fc
    Ipp8u               *fft_buf_f;                // буфер БПФ f

    Ipp32fc             fft_dst_fc[DSP_FFT_SIZE];
    Ipp32f              fft_dst_f[DSP_FFT_SIZE + 2]; // in CCS format required N + 1 cells

    FFT_params():       fft_dynamic_range{DSP_FFT_DEFAULT_DYNAMIC_RANGE},
                        fft_noise_level{DSP_FFT_DEFAULT_NOISE_LEVEL},
                        null_bin{DSP_FFT_DEFAULT_NULL_BIN},

                        dc_offset_gain{127.36},
                        compensatePtr{0},

                        fft_mode{DSP_FFT_DEFAULT_MODE},
                        fft_input_cell_size{0},
                        fft_averages_number{DSP_FFT_DEFAULT_AVERAGE_NUMBER},
                        fft_max_averages_number{0},
                        fft_inversion{DSP_FFT_DEFAULT_INVERSION},
                        fft_current_window{DSP_DEFAULT_FFT_WINDOW},
                        fft_win_alpha{DSP_DEFAULT_WIN_ALPHA},
                        fft_stlIdx{DSP_DEFAULT_FFT_STYLE_IDX},
                        fft_frqGrdIdx{DSP_DEFAULT_FFT_GRID_IDX},
                        fft_sonoPltIdx{DSP_DEFAULT_FFT_SONO_PALETTE_IDX},
                        fft_fltBwAlpha{DSP_DEFAULT_FFT_FLT_ALPHA},

                        pFftSpec_fc{nullptr},
                        pFftSpec_f{nullptr},
                        fft_buf_fc{nullptr},
                        fft_buf_f{nullptr}
                        {
                            ippsZero_32f(fft_res, DSP_FFT_SIZE);
                            ippsZero_32f(compensateBuf, 20);

                            int sizeSpec = 0, sizeInit = 0, sizeBuffer = 0;

                            ippsFFTGetSize_C_32fc(DSP_FFT_ORDER, IPP_FFT_DIV_BY_SQRTN, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer);
                            Ipp8u *FFTSpec_fc = new Ipp8u[sizeSpec];
                            Ipp8u *FFTSpecBuf_fc = new Ipp8u[sizeInit];
                            fft_buf_fc = new Ipp8u[sizeBuffer];
                            ippsFFTInit_C_32fc(&pFftSpec_fc, DSP_FFT_ORDER, IPP_FFT_DIV_BY_SQRTN, ippAlgHintNone, FFTSpec_fc, FFTSpecBuf_fc);

                            Ipp8u *FFTSpec_f = new Ipp8u[sizeSpec];
                            Ipp8u *FFTSpecBuf_f = new Ipp8u[sizeInit];

                            ippsFFTGetSize_R_32f(DSP_FFT_ORDER, IPP_FFT_DIV_BY_SQRTN, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer);
                            FFTSpec_f = new Ipp8u[sizeSpec];
                            FFTSpecBuf_f = new Ipp8u[sizeInit];
                            fft_buf_f = new Ipp8u[sizeBuffer];
                            ippsFFTInit_R_32f(&pFftSpec_f, DSP_FFT_ORDER, IPP_FFT_DIV_BY_SQRTN, ippAlgHintNone, FFTSpec_f, FFTSpecBuf_f);
                        }
};

// параметры сдвига
class SHIFT_params
{
public:
    RING_BUFFER<Ipp32fc, DSP_SHIFT_RB_SIZE> rb;
    double          r_frec;              // частота сдвига
    bool            step;                // флаг кратности шага изменения частоты сноса
    Ipp32fc         *complex_sin;        // комплексная синусоида для сдвига
    Ipp32f          sin_phase;           // фаза синусоиды

    bool            write_to_file;       // запись в файл

    SHIFT_params(): r_frec{0},
                    step{true},
                    complex_sin{nullptr},
                    sin_phase{0},
                    write_to_file{false} {}

    void realloc_mem(){
        rb.realloc_mem();

        if(complex_sin){
            delete[] complex_sin;
            complex_sin = nullptr;
        }
        complex_sin = new Ipp32fc[rb.cell_size / 2];
    }
};

// параметры wav файлов
class WAV_params{

public:
    QFile         file;
    QString       file_name;
    QString       path;
    WAVEHEADER    header;
    char          *out_buf;

    WAV_params(): file{nullptr},
                  file_name{""},
                  path{"C:/"},
                  out_buf{nullptr} {}
};

// параметры демодуляции
class DEMOD_params{
public:
    RING_BUFFER<Ipp32f, DSP_DEMOD_RB_SIZE> rb;

    DEMOD_MODE      demod_mode;         // вид модуляции

    // для двухполосной АМ
    int             am_down_sample_factor; // коэф-т прореживания
    int             am_down_sample_phase;  // фаза прореживание (при опредленных расчетах не используется)

    // для ОБП
    Ipp32fc         *complex_sin;        // комплексная синусоида для сдвига
    Ipp32f          sin_phase;           // фаза синусоиды

    // для ЧМ
    int             fm_first_down_sample_factor; // коэф-т прореживания
    int             fm_first_down_sample_phase;  // фаза прореживание (при опредленных расчетах не используется)
    Ipp32f          first_last_sample;        // последний отсчет предыдущего блока. предотввращает разрыв фазы между блоками

    // буферы для конвертации и демодуляции
    int             down_buf_len;
    Ipp32fc         *down_buf_fc;
    Ipp32f          *down_buf_re;
    Ipp32f          *down_buf_im;

    int             fm_second_down_sample_factor; // коэф-т прореживания
    int             fm_second_down_sample_phase;  // фаза прореживание (при опредленных расчетах не используется)
    Ipp32f          second_last_sample;        // последний отсчет предыдущего блока. предотввращает разрыв фазы между блоками

    // буферы для фильтрации, децимации
    Ipp32f          *sound_filtration_buf;
    Ipp32f          *down_buf;

    // sound filter для фильтрации в полосе 0-20кГц
    IppsFIRSpec_32f *sound_flt_spec;
    Ipp32f          sound_flt_taps32[DSP_FLT_LENGTH]; // ИХ
    Ipp32f          sound_delay_line[DSP_FLT_LENGTH - 1]; // линия задержки
    Ipp8u           *sound_flt_buf;

    DEMOD_params(): demod_mode{DSP_DEMOD_DEFAULT_MODE},
                    am_down_sample_factor{SDR_DEFAULT_SAMPLE_RATE / DSP_SOUND_SAMPLE_RATE},
                    am_down_sample_phase{0},
                    complex_sin{nullptr},
                    sin_phase{0},
                    fm_first_down_sample_factor{SDR_DEFAULT_SAMPLE_RATE / DSP_FM_DEMOD_SAMPLE_RATE},
                    fm_first_down_sample_phase{0},
                    first_last_sample{0},
                    down_buf_len{0},
                    down_buf_fc{nullptr},
                    down_buf_re{nullptr},
                    down_buf_im{nullptr},
                    fm_second_down_sample_factor{DSP_FM_DEMOD_SAMPLE_RATE / DSP_SOUND_SAMPLE_RATE},
                    fm_second_down_sample_phase{0},
                    second_last_sample{0},
                    sound_filtration_buf{nullptr},
                    sound_flt_buf{nullptr}
    {
        create_sound_filter();
    }

    // создание звукового фильтра
    void create_sound_filter(){
        int buf_size, spec_size;
        Ipp64f tmp64[DSP_FLT_LENGTH];

        ippsZero_32f(sound_delay_line, DSP_FLT_LENGTH - 1);

        ippsFIRGenGetBufferSize(DSP_FLT_LENGTH, &buf_size);
        sound_flt_buf = new Ipp8u[buf_size];

        ippsFIRGenLowpass_64f(DSP_SOUND_FLT_HIGH_FREQ / double(DSP_FM_DEMOD_SAMPLE_RATE),
                              tmp64,
                              DSP_FLT_LENGTH,
                              ippWinBlackman,
                              ippTrue,
                              sound_flt_buf);

        ippsConvert_64f32f(tmp64, sound_flt_taps32, DSP_FLT_LENGTH);

        delete[] sound_flt_buf;
        sound_flt_buf = nullptr;

        ippsFIRSRGetSize(DSP_FLT_LENGTH, ipp32f, &spec_size, &buf_size);
        sound_flt_spec = reinterpret_cast<IppsFIRSpec_32f*>(new Ipp8u[spec_size]);
        sound_flt_buf = new Ipp8u[buf_size];

        ippsFIRSRInit_32f(sound_flt_taps32, DSP_FLT_LENGTH, ippAlgAuto, sound_flt_spec);
    }

    void realloc_mem(int flt_buf_size){

        rb.realloc_mem();

        if(complex_sin){
            delete[] complex_sin;
            complex_sin = nullptr;
        }
        complex_sin = new Ipp32fc[flt_buf_size / 2];

        if(down_buf_fc){
            delete[] down_buf_fc;
            down_buf_fc = nullptr;
        }
        down_buf_fc = new Ipp32fc[flt_buf_size / 2];

        if(down_buf_re){
            delete[] down_buf_re;
            down_buf_re = nullptr;
        }
        down_buf_re = new Ipp32f[flt_buf_size / 2];

        if(down_buf_im){
            delete[] down_buf_im;
            down_buf_im = nullptr;
        }
        down_buf_im = new Ipp32f[flt_buf_size / 2];

        if(sound_filtration_buf){
            delete[] sound_filtration_buf;
            sound_filtration_buf = nullptr;
        }
        sound_filtration_buf = new Ipp32f[flt_buf_size / 2];
    }
};

// параметры выходного аудиопотока
class SOUND_params{
public:
    RING_BUFFER<Ipp16s, DSP_SOUND_RB_SIZE> rb;

    int             sound_volume;       // громкость
    bool            is_mute;            // флаг приглушения
    int             scale_facor;        // коэффициент для конвертации 32f-16s
                                        //      для АМ = -6, для ЧМ = 1

    bool            write_to_file;

    // вывод звука
    QAudioOutput    *audio_out;         // интерфейс к звуковой карте
    QIODevice       *audio_device;      // куда пишем

    SOUND_params(): sound_volume{DSP_SOUND_DEFAULT_VOLUME},
                    is_mute{false},
                    scale_facor{1},
                    write_to_file{false},
                    audio_out{nullptr},
                    audio_device{nullptr} {}

    void realloc_mem(){
        rb.realloc_mem();
    }
};

// общие настройки
class DSP_params{

public:
    READ_params   read_params;
    FFT_params    fft_params;
    FLT_params    flt_params;
    SHIFT_params  shift_params;
    DEMOD_params  demod_params;
    SOUND_params  sound_params;

    DSP_params(): read_params{},
                  fft_params{},
                  flt_params{},
                  shift_params{},
                  demod_params{},
                  sound_params{} {}
};

#endif // DEFINITIONS
