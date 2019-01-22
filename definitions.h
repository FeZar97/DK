
#ifndef DEFINITIONS
#define DEFINITIONS

#define     VERSION                             "v0.19.1.22"
#define     NAME                                "Цифровой тракт РПУ \"Кальмар\" "

#include <cmath>

#include <float.h>

#include <inpout32.h>

#include <ippCustom.h>

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

#include <rtl-sdr.h>

typedef void	(__stdcall *lpOutc)(unsigned short, unsigned char);

#include <windows.h>
/// ----------------------------------перечисления---------------------------------------
// тип клика мыши, при перемещении курсора по этому типу будем определять
// что именно нужно сделать, перенести окно, или изменить его размер с одной из сторон
enum MouseType {
    Move,
    Spectr,
    None
};

// адреса LPT портов
enum LPT_ADDRESS{
    LPT_1 = 0x378,
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
     ONE_ONE_ONE_ONE_CHANNEL, // тип 5 ~ 1 канал  + 1 канал + 1 канал + 1 канал
};

// модулирующий сигнал
enum MOD_SIGNAL{SINUS,
                RADIOIMP};

// внутренний/внешний калибратор
enum KALIB_TYPE{INTERNAL,
                EXTERNAL};

// состояние калибратора
enum STATUS{OFF,
            ON};

// полоса ПЧ
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
              SHIFT_FFT}; // спектр сдвинутого сигнала

// оконные функции
enum WINDOW{NONE,
            BARLETT,
            BLACKMANN,
            HAMMING,
            HANN,
            KAISER};
/// -----------------------------------объявления-----------------------------------------

#define     CURRENT_DEGREE                          WITHOUT

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
#define     RPU_MAX_TRACT_FREQ                      32000000                            // 31,999999 Мгц - верхняя чатсота
#define     RPU_MIN_TRACT_FREQ                      150000                              // 1 Мгц - нижняя частота

#define     RPU_DEFAULT_IN_ATT_IDX                  OFF                                 // по умолчанию входной аттенюатор отключен

#define     RPU_DEFAULT_HF_ATT_IDX                  OFF                                 // по умолчанию ВЧ аттенюатор отключен
#define     RPU_HF_ATT_STEP                         6                                   // шаг сетки аттенюатора ВЧ (дБ)
#define     RPU_MAX_HF_ATT_IDX                      7                                   // максимальный индекс элемента из выпадающего списка ВЧ аттенюатора
#define     RPU_MIN_HF_ATT_IDX                      0                                   // минимальный индекс элемента из выпадающего списка ВЧ аттенюатора

#define     RPU_DEFAULT_IF_ATT_IDX                  OFF                                 // выключен
#define     RPU_IF_ATT_STEP                         2                                   // шаг сетки аттенюатора ПЧ (дБ)
#define     RPU_MAX_IF_ATT_IDX                      15                                   // максимальный индекс элемента из выпадающего списка ПЧ аттенюатора
#define     RPU_MIN_IF_ATT_IDX                      0                                   // минимальный индекс элемента из выпадающего списка ВЧ аттенюатора

#define     RPU_DEFAULT_IF_BAND_IDX                 0                                   // 3 кГц
#define     RPU_MAX_IF_BAND_IDX                     3                                   // максимальный индекс элемента из выпадающего списка ширины полосы ПЧ
#define     RPU_MIN_IF_BAND_IDX                     0                                   // минимальный индекс элемента из выпадающего списка ширины полосы ПЧ

#define     RPU_DEFAULT_ACTIVE_DIGIT                ZERO                                // индекс выбранного разряда по умолчанию
#define     RPU_MIN_ACTIVE_DIGIT                    ZERO                                // минимальный индекс разряда
#define     RPU_MAX_ACTIVE_DIGIT                    FIFTH                               // максимальный индекс разряда

#define     RPU_NUMBER_OF_PRESELECTORS              10                                  // количество преселекторов
// ---------------------------------------SDR---------------------------------------------
#define     SDR_MAX_TUNER_FREQ                      2400000001
#define     SDR_MIN_TUNER_FREQ                      100000

#define     SDR_DEFAULT_CENTRAL_FREQ                104200000
#define     SDR_DEFAULT_GAIN_IDX                    1
#define     SDR_DEFAULT_SAMPLE_RATE                 1400000
#define     SDR_DEFAULT_DIRECT_SAMPLING_IDX         0

// ---------------------------------------DSP---------------------------------------------
#define     DSP_DEFAULT_UP_FACTOR                   1
#define     DSP_DEFAULT_DOWN_FACTOR                 2
#define     DSP_FILTER_LENGTH                       517
#define     DSP_DEFAULT_FFT_MODE                    READER_FFT
#define     DSP_DEFAULT_AVERAGE_NUMBER              8
#define     DSP_DEFAULT_FFT_INVERSION               false
#define     DSP_DEFAULT_DYNAMIC_RANGE               30
#define     DSP_DEFAULT_NOISE_LEVEL                 -10
#define     DSP_DEFAULT_RECORDING_TIME_IDX          0
#define     DSP_READOUT_PER_SECONDS                 20
#define     DSP_DEFAULT_FFT_WINDOW                  NONE
#define     DSP_DEFAULT_FFT_INFO                    true
#define     DSP_DEFAULT_WIN_ALPHA                   0.5

#define     DSP_DEFAULT_FILE_RECORD                 false

#define     DSP_FFT_SIZE                            1024
#define     DSP_NOISE_SIZE                          25

#define     DSP_SOUND_SAMPLE_RATE                   48000

// размеры кольцевых буферов
#define     DSP_READ_RB_SIZE                        10
#define     DSP_FLT_RB_SIZE                         10
#define     DSP_SHIFT_RB_SIZE                       10
#define     DSP_SOUND_RB_SIZE                       10

#define     LINE                                "\n-----------------------------------------------------------------------------"

// набор КУ [дБ * 10]
const static int gains[] = {0, 9, 14, 27, 37, 77, 87, 125, 144, 157, 166, 197, 207, 229, 254, 280, 297, 328, 338, 364, 372, 386, 402, 421, 434, 439, 445, 480, 496};

// длительность записи [в секундах]
const static unsigned int times[] = {10, 30, 60, 90, 120, 360};

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
    // (0x666d7420 в big-endian представлении)
    char subchunk1Id[4];

    // 16 для формата PCM.
    // Это оставшийся размер подцепочки, начиная с этой позиции.
    unsigned long subchunk1Size;

    // Аудио формат, полный список можно получить здесь http://audiocoding.ru/wav_formats.txt
    // Для PCM = 1 (то есть, Линейное квантование).
    // Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
    unsigned short audioFormat;

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

// параметры SDR приемника
class SDR_params{
public:
    int                index;                   // номер используемого SDR приемника

    unsigned int       central_freq;            // центральная частота
    unsigned int       gain_idx;                // индекс КУ из набора gains
    unsigned int       sample_rate;             // частота дискретизации АЦП
    unsigned int       direct_sampling_idx;     // режим оцифровки

    rtlsdr_dev_t       *sdr_ptr;
    bool               is_open;                 // флаг готовности приемника
    bool               is_already_open;         // флаг занятости приемника другим процессом

    SDR_params(){
        index               = 0;

        central_freq        = SDR_DEFAULT_CENTRAL_FREQ;
        gain_idx            = SDR_DEFAULT_GAIN_IDX;
        sample_rate         = SDR_DEFAULT_SAMPLE_RATE;
        direct_sampling_idx = SDR_DEFAULT_DIRECT_SAMPLING_IDX;

        sdr_ptr             = nullptr;
        is_open             = false;
        is_already_open     = false;
    }
};

// параметры чтения
class READ_params
{
public:
    // глобальные
    bool                is_recording;            // флаг активной записи
    bool                end_reading;             // флаг штатного окончания чтения
    bool                end_filtering;           // флаг штатного окончания фильтрации
    bool                end_fft;                 // флаг штатного окончания БПФ
    bool                emergency_end_recording; // флаг экстренного окончания записи

    unsigned int        read_rb_cell_size;
    unsigned int        read_rb_cell_idx;
    Ipp32fc             **read_rb;               // КБ чтения

    unsigned int        rec_time_idx;            // индекс времени записи
    unsigned int        rec_time;                // длительность записи в секундах
    unsigned int        readout_per_seconds;     // ФПС

    // флаги записи в файл
    bool                use_files; // главный флаг, разрешающий разблокировку кнопки записи
    bool                use_first_file;
    bool                use_second_file;
    bool                use_third_file;

    // локальные
    Ipp8u               *read_cell;              // временный буфер

    READ_params(){
        is_recording            = false;
        end_reading             = true;
        end_filtering           = true;
        end_fft                 = true;
        emergency_end_recording = false;

        read_rb_cell_idx        = 0;
        read_rb                 = nullptr;

        rec_time_idx            = DSP_DEFAULT_RECORDING_TIME_IDX;
        readout_per_seconds     = DSP_READOUT_PER_SECONDS;

        use_files               = false;
        use_first_file          = false;
        use_second_file         = false;
        use_third_file          = false;
    }
};

// параметры фильтрации
class FLT_params{
public:
    double              r_frec;                  // относительная частота среза

    Ipp32fc             **filtration_rb;         // КБ фильтрации
    unsigned int        filtration_rb_cell_size; // размер ячейки КБ фильтрации
    unsigned int        filtration_rb_cell_idx;  // итератор по КБ фильтрации

    IppsFIRSpec_32fc    *flt_spec;
    Ipp32fc             flt_taps32[DSP_FILTER_LENGTH]; // ИХ
    Ipp32fc             delay_line[DSP_FILTER_LENGTH - 1]; // линия задержки
    Ipp8u               *buf;

    FLT_params():
        r_frec(0.15),

        filtration_rb(nullptr),
        filtration_rb_cell_size(0),
        filtration_rb_cell_idx(0),

        flt_spec(nullptr),
        buf(nullptr)
    {}

    void recalc_flt_params(){

        ippsZero_32fc(delay_line, DSP_FILTER_LENGTH - 1);
        int buf_size, spec_size;

        Ipp64f tmp64[DSP_FILTER_LENGTH];
        Ipp32f tmp32[DSP_FILTER_LENGTH];

        ippsFIRGenGetBufferSize(DSP_FILTER_LENGTH, &buf_size);
        buf = new Ipp8u[buf_size];
        ippsFIRGenLowpass_64f(r_frec, tmp64, DSP_FILTER_LENGTH, ippWinBlackman, ippTrue, buf);
        ippsConvert_64f32f(tmp64, tmp32, DSP_FILTER_LENGTH);

        delete[] buf;
        buf = nullptr;

        ippsFIRSRGetSize(DSP_FILTER_LENGTH, ipp32fc, &spec_size, &buf_size);
        flt_spec = reinterpret_cast<IppsFIRSpec_32fc*>(new Ipp8u[spec_size]);
        buf = new Ipp8u[buf_size];

        ippsRealToCplx_32f(tmp32, nullptr, flt_taps32, DSP_FILTER_LENGTH);

        ippsFIRSRInit_32fc(flt_taps32, DSP_FILTER_LENGTH, ippAlgAuto, flt_spec);
    }
};

// параметры БПФ
class FFT_params
{
public:
    // глобальные
    unsigned int       fft_dynamic_range;       // динамический диапазон
    int                fft_noise_level;         // уровень шума

    Ipp32fc            dc_offset;            // величина коррекции

    FFT_MODE           fft_mode;                // спектр какого сигнала выводить
    unsigned int       fft_input_cell_size;     // размер входного массива
    unsigned int       fft_averages_number;     // кол-во усреднений спектра
    unsigned int       fft_max_averages_number; // максимально допустимое число усреднений
    bool               fft_inversion;           // флаг инверсии спектра
    WINDOW             fft_current_window;      // используемое окно
    bool               fft_info;                // флаг вывода информации на экран
    float              fft_win_alpha;           // параметр оконной функции

    Ipp32f             fft_res[DSP_FFT_SIZE];   // энергетический спектр

    Ipp32f             noise_level;
    Ipp32f             noise_buf[DSP_NOISE_SIZE];
    int                noise_idx;
    Ipp32f             accum;
    int                accum_weight;

    int                max_level_idx;

    bool               null_bin_circle;

    FFT_params(){
        fft_win_alpha = DSP_DEFAULT_WIN_ALPHA;
        fft_dynamic_range = DSP_DEFAULT_DYNAMIC_RANGE;
        fft_noise_level = DSP_DEFAULT_NOISE_LEVEL;

        dc_offset.re = 0;
        dc_offset.im = 0;

        fft_mode = DSP_DEFAULT_FFT_MODE;
        fft_input_cell_size = 0;
        fft_averages_number = DSP_DEFAULT_AVERAGE_NUMBER;
        fft_max_averages_number = 0;
        fft_inversion = DSP_DEFAULT_FFT_INVERSION;
        fft_current_window = DSP_DEFAULT_FFT_WINDOW;
        fft_info = DSP_DEFAULT_FFT_INFO;

        noise_level = 0;
        noise_idx = 0;
        accum = 0;
        accum_weight = 0;

        max_level_idx = 0;

        null_bin_circle = false;

        ippsZero_32f(fft_res, DSP_FFT_SIZE);
        ippsZero_32f(noise_buf, DSP_NOISE_SIZE);
    }
};

// параметры сдвига
class SHIFT_params
{

public:
    double              shift_freq;          // частота сдвига
    Ipp32fc             **shift_rb;          // КБ сдвига
    unsigned int        shift_rb_cell_size;  // размер ячейки КБ сдвига
    unsigned int        shift_rb_cell_idx;   // итератор по КБ сдвига
    bool                step;                // флаг кратности шага изменения частоты сноса
    Ipp32fc             *complex_sin;       // комплексная синусоида для сдвига
    Ipp32f              CurrentPhase;       // фаза синусоиды

    SHIFT_params(): shift_freq{0},
                    shift_rb{nullptr},
                    shift_rb_cell_size{0},
                    shift_rb_cell_idx{0},
                    step{true},
                    complex_sin{nullptr},
                    CurrentPhase{0} {}
};

// параметры wav файлов
class WAV_params{

public:
    QFile          file;
    WAVEHEADER     header;
    quint64        pos;
    quint64        total_size;
    QString        file_name;
    QString        directory;
    int            input_cell_size;
    Ipp8s          *out_buf;

    WAV_params(): file{nullptr},
                  pos{0},
                  total_size{0},
                  file_name{""},
                  directory{"C:/"},
                  input_cell_size{0},
                  out_buf{nullptr} {}
};

// параметры выходного аудиопотока
class SOUND_params{

public:
    static const int sound_freq     = 48000;
    static const int channel_count  = 2;
    static const int sample_size    = 16;

    Ipp16s       **sound_rb;
    unsigned int sound_rb_cell_size;
    unsigned int sound_rb_cell_idx;

    SOUND_params(): sound_rb{nullptr},
                    sound_rb_cell_size{0},
                    sound_rb_cell_idx{0} {}
};

// общие настройки
class DSP_params{

public:
    READ_params         *read_params;
    FFT_params          *fft_params;
    FLT_params          *flt_params;
    SHIFT_params        *shift_params;
    WAV_params          *wav_params;
    SOUND_params        *sound_params;

    DSP_params(): read_params{new READ_params()},
                  fft_params{new FFT_params()},
                  flt_params{new FLT_params()},
                  shift_params{new SHIFT_params()},
                  wav_params{new WAV_params()},
                  sound_params{new SOUND_params()}
                  {}
};

#endif // DEFINITIONS
