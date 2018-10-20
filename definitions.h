
/* TODO
 *        1) получение информации из регистров статуса;
 *        2) правильно добавить иконки в проект;
 *        3) ошибка в конфигурации аттенюатора ВЧ;
 *        4) интерфейс вывода спектра и управления SDR;*/

/* In Progress
          1) ;*/

/* for discussion
 * 1)   как передавать QSettings в другой класс;
 * 6)   в разделе "Код управления режимами работы" (файл CurrentTractForm.cpp) для режимов
 *      и с синхронными и с автономными каналами нет указаний к выбору каналов для автономных каналов
 *      (вероятно это можно сделать самостоятельно, но все же стоит спросить);
 * 7)   в разделе "Код управления режимами работы" (файл CurrentTractForm.cpp) для режима
 *      "Две пары синхронных каналов" по вордовскому описанию ВЕДУЩИМИ каналами являются первый и третий,
 *      а в коде одной паре соответсвует второй канал, а другой паре - третий канал. Чему верить? В кьюте
 *      код переделан согласно вордовскому описанию, билдеровский вариант (RpuClass, строка 464) не изменял;
 * 8)   РАБОТАЮЩАЯ ВЕРСИЯ ОПРЕДЕЛЕНИЯ СТРОБА:
 *      чтобы определить значение строба для данной кодограммы нужно:
 *         1) взять из Таблицы_Кодограмм трехразрядное значение, соответсвующее данной кодограмме,
 *            с учетом инверсий в разрядах 9 и 11;
 *         2) добавить СПРАВА один разряд;
 *         3) полученное четырехразрядное число и определяет нужный строб;
 *         При записи в крайний правый разряд единицы получаем "верхнее" значение строба (в двоичном виде);
 *         При записи в крайний правый разряд нуля получаем "нижнее" значение строба (в двоичном виде).
 * 12) в разделе "КОД УПРАВЛЕНИЯ БЛОКАМИ ПРИЕМНИКОВ" (файл CurrentTractForm.cpp строка 803) две первых строки
 *     перемнных были формата шорт, но им присваиваются интовые значения - это норма?
 * 15) файл CurrentTractForm.cpp строка 834 - магические числа
 * 16) файл CurrentTractForm.cpp строка 845 - побитовые операции с магическими числами*/

#ifndef DEFINITIONS
#define DEFINITIONS

#define     VERSION                             "v0.18.3.20"
#define     NAME                                "Цифровой тракт РПУ \"Кальмар\" "

#include <cmath>

#include <float.h>

#include <inpout32.h>

#include <ippCustom.h>

#include <QDateTime>
#include <QDebug>
#include <QDesktopWidget>
#include <QFile>
#include <QFileDialog>
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
     ONE_ONE_ONE_ONE_CHANNEL, // тип 5 ~ 1 канал  + 1 канал + 1 канал + 1 канал.
     MIN_CONFIG = FOUR_CHANNEL,
     MAX_CONFIG= ONE_ONE_ONE_ONE_CHANNEL};

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
enum FFT_MODE{
    READER_FFT, // спектр сигнала с АЦП
    FLT_FFT};   // спектр отфильтрованного сигнала

// оконные функции
enum WINDOW{
    NONE,
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
#define     RPU_DEFAULT_TRACT_TYPE                  FOUR_CHANNEL                        // 4-х канальный режим

// парпаметры трактов
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

#define     RPU_DEFAULT_TRACT_ACTIVE                true

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

// размеры кольцевых буферов
#define     DSP_READ_RB_SIZE                        10
#define     DSP_FLT_RB_SIZE                         10
#define     DSP_SHIFT_RB_SIZE                       10

#define     LINE                                "\n-----------------------------------------------------------------------------"

// набор КУ [дБ * 10]
const static int gains[] = {0, 9, 14, 27, 37, 77, 87, 125, 144, 157, 166, 197, 207, 229, 254, 280, 297, 328, 338, 364, 372, 386, 402, 421, 434, 439, 445, 480, 496};

// длительность записи [в секундах]
const static int times[] = {10, 30, 60, 90, 120, 360};

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

        sdr_ptr             = NULL;
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
        read_rb                 = NULL;

        rec_time_idx            = DSP_DEFAULT_RECORDING_TIME_IDX;
        readout_per_seconds     = DSP_READOUT_PER_SECONDS;

        use_files = false;
        use_first_file = false;
        use_second_file = false;
        use_third_file = false;
    }
};

// параметры фильтрации
class FLT_params{
public:
// глобальные
    bool                is_using;                // флаг использования фильтрации
    unsigned int        up_factor;               // коэф-т интерполяции
    unsigned int        down_factor;             // коэф-т децимации
    unsigned int        out_sample_rate;         // частота сигнала после передискретизации

    Ipp32fc             **filtration_rb;         // КБ фильтрации
    unsigned int        filtration_rb_cell_size; // размер ячейки КБ фильтрации
    unsigned int        filtration_rb_cell_idx;  // итератор по КБ фильтрации

// локальные
    Ipp32f              *filter_taps;            // указатель на массив отсчетов ИХ фильтра
    int                 filter_length;           // длина ИХ фильтра

    // спецификации для IPP
    IppsFIRSpec_32f     *pSpec;
    Ipp8u               *buf;

    Ipp32f              *temp_32fc;              // буфер для преобразования 8u -> 32f

    // входные вектора для фильтрации
    Ipp32f              *temp_32f_re;
    Ipp32f              *temp_32f_im;

    // результат фильтрации
    Ipp32f              *flt_32f_re;
    Ipp32f              *flt_32f_im;

    // линии задержки
    Ipp32f              *delay_re;
    Ipp32f              *delay_im;

    FLT_params(){
        up_factor = DSP_DEFAULT_UP_FACTOR;
        down_factor = DSP_DEFAULT_DOWN_FACTOR;

        filter_taps = NULL;
        filter_length = 0;

        pSpec = NULL;
        buf = NULL;

        temp_32fc = NULL;

        temp_32f_re = NULL;
        temp_32f_im = NULL;

        flt_32f_re = NULL;
        flt_32f_im = NULL;

        delay_re = NULL;
        delay_im = NULL;

        filtration_rb = NULL;
        filtration_rb_cell_size = 0;
        filtration_rb_cell_idx = 0;
    }
};

// параметры БПФ
class FFT_params
{
public:
    // глобальные
    unsigned int       fft_dynamic_range;       // динамический диапазон
    int                fft_noise_level;         // уровень шума

    FFT_MODE           fft_mode;                // спектр какого сигнала выводить
    unsigned int       fft_input_cell_size;     // размер входного массива
    unsigned int       fft_averages_number;     // кол-во усреднений спектра
    unsigned int       fft_max_averages_number; // максимально допустимое число усреднений
    bool               fft_inversion;           // флаг инверсии спектра
    WINDOW             fft_current_window;      // используемое окно
    bool               fft_info;                // флаг вывода информации на экран
    float              fft_win_alpha;           // параметр оконной функции

    Ipp32f             fft_res[DSP_FFT_SIZE];   // энергетический спектр


    FFT_params(){
        fft_win_alpha = DSP_DEFAULT_WIN_ALPHA;
        fft_dynamic_range = DSP_DEFAULT_DYNAMIC_RANGE;
        fft_noise_level = DSP_DEFAULT_NOISE_LEVEL;

        fft_mode = DSP_DEFAULT_FFT_MODE;
        fft_input_cell_size = 0;
        fft_averages_number = DSP_DEFAULT_AVERAGE_NUMBER;
        fft_max_averages_number = 0;
        fft_inversion = DSP_DEFAULT_FFT_INVERSION;
        fft_current_window = DSP_DEFAULT_FFT_WINDOW;
        fft_info = DSP_DEFAULT_FFT_INFO;

        ippsZero_32f(fft_res, DSP_FFT_SIZE);
    }
};

// параметры сдвига
class SHIFT_params
{
public:
    // глобальные
    double              shift_freq;          // частота сдвига
    Ipp32fc             **shift_rb;          // КБ сдвига
    unsigned int        shift_rb_cell_size;  // размер ячейки КБ сдвига
    unsigned int        shift_rb_cell_idx;   // итератор по КБ сдвига

    // локальные
    Ipp32fc             *complex_sin;       // комплексная синусоида для сдвига
    Ipp32f              CurrentPhase;       // фаза синусоиды

    SHIFT_params(){
        shift_freq = 0;
        shift_rb = NULL;
        shift_rb_cell_size = 0;
        shift_rb_cell_idx = 0;

        complex_sin = 0;
        CurrentPhase = 0;
    }
};

// параметры wav файлов
class WAV_params{
public:

    QFile          file;
    WAVEHEADER     header;
    quint64        pos;
    quint64        total_size;
    QString        file_name;         // имя первого файла

    int            input_cell_size;
    Ipp8u          *out_buf;

    WAV_params(){
        pos = 0;
        total_size = 0;
        file_name = "";
        input_cell_size = 0;
        out_buf = NULL;
    }
};

// общие настройки
class DSP_params{
public:
    READ_params         *read_params;
    FFT_params          *fft_params;
    FLT_params          *flt_params;
    SHIFT_params        *shift_params;
    WAV_params          *wav_params;

    DSP_params(){
        read_params  = new READ_params();
        fft_params   = new FFT_params();
        flt_params   = new FLT_params();
        shift_params = new SHIFT_params();
        wav_params   = new WAV_params();
    }
};

#endif // DEFINITIONS
