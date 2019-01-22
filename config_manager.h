#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <QDialog>
#include <definitions.h>
#include <stylehelper.h>
#include <rpu.h>
#include <sdr.h>
#include <dsp.h>


namespace Ui {
class config_manager;
}

class config_manager : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QPoint previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

    Ui::config_manager *ui;

    MouseType m_leftMouseButtonPressed;
    QPoint m_previousPosition;
    MouseType checkResizableField(QMouseEvent *event); // определение области нажатия


public:
    explicit config_manager(QWidget *parent = nullptr,
                            RPU *new_rpu    = nullptr,
                            SDR *new_sdr    = nullptr,
                            DSP *new_dsp    = nullptr);
    ~config_manager();
    QPoint previousPosition() const;

    bool          is_visible;

    RPU           *rpu;
    SDR           *sdr;
    DSP           *dsp;

public slots:
    void setPreviousPosition(QPoint previousPosition);
    void update_interface(); // обновление интерфейса
    void update_rpu_tab(); // обновление вкладки "РПУ"
    void update_sdr_tab(); // обновление вкладки "SDR"
    void update_dsp_tab(); // обновление вкладки "ЦОС"
    void set_ui_style(); // применение стиля

    void update_adc_bar(int new_val);
    void update_flt_bar(int new_val);
    void update_real_bar(int new_val);

    void on_CloseButton_clicked();

private slots:
/// -----------------------------------РПУ---------------------------------------------
    void on_LPTPortCB_currentIndexChanged(int new_lpt_port_idx); // смена адреса LPT порта
    void on_TractConfigCB_currentIndexChanged(int new_rpu_config_idx); // смена конфигурации трактов

    void on_KalibStatusBox_currentIndexChanged(int new_work_status_idx); // включение выключение калибратора
    void on_KalibAttenTypeBox_currentIndexChanged(int new_att_idx); // изменение значения аттенюатора калибратора
    void on_KalibExitTypeBox_currentIndexChanged(int new_exit_type_idx); // изменение выхода калибратора
    void on_KalibModulationFrequencyBox_valueChanged(int new_modulation_freq_in_kHz); // изменение частоты модуляции
    void on_KalibSignalTypeBox_currentIndexChanged(int new_signal_type_idx); // изменение типа модулирующего сигнала

    // тракт 1
    void on_Tract1_CentralFreqSB_valueChanged(int new_central_freq);
    void on_Tract1_AttInBox_currentIndexChanged(int new_in_att_idx);
    void on_Tract1_AttHFBox_currentIndexChanged(int new_hf_att_idx);
    void on_Tract1_AttIFBox_currentIndexChanged(int new_att_if_idx);
    void on_Tract1_BandIFBox_currentIndexChanged(int new_band_if_idx);

/// -----------------------------------SDR---------------------------------------------
    void on_RefreshButton_clicked(); // поиск новых приемников
    void on_ConfirmButton_clicked(); // подключение к новому приемнику
    void on_CentralFreqSB_editingFinished(); // изменение центральной частоты настройки
    void on_SampleRateSB_valueChanged(int new_sample_rate); // изменение частоты дискретизации
    void on_GainValBox_currentIndexChanged(int new_gain_idx); // изменение КУ
    void on_DirectSamplingCB_currentIndexChanged(int new_direct_sampling_idx); // изменение режима оцифровки

/// -----------------------------------DSP---------------------------------------------
// запись
    void on_RecordTimeCB_currentIndexChanged(int new_recording_time_idx); // смена времени записи
    void on_ADCSignalCB_clicked(bool new_use_first_file); // выбор файлов на запись
    void on_FltSignalCB_clicked(bool new_use_second_file); // выбор файлов на запись
    void on_RealSignalCB_clicked(bool new_use_third_file); // выбор файлов на запись
    void on_RecordButton_clicked(); // начало записи в файл
    void on_CurrentPathButton_clicked(); // выбор папки

// фильтрация
    void on_FiltrationCB_clicked(bool new_state);
    void on_RFrecDSB_valueChanged(double new_r_frec);

// БПФ
    void on_FftModeCB_currentIndexChanged(int new_fft_mode); // смена "режима" БПФ
    void on_ReadOutPerSecSB_valueChanged(int new_readout_per_sec); // изменение кол-ва усреднений в секунду
    void on_AverageNumSB_valueChanged(int new_fft_averages_number); // изменение числа усреднений
    void on_InfoCB_clicked(bool new_fft_info); // вывод информации на спектр
    void on_InversionCB_clicked(bool new_fft_inversion); // смена флага инверсии спектра
    void on_WindowCB_currentIndexChanged(int new_win_idx); // выбор окна
    void on_WindowAlphaSB_valueChanged(double new_aplha); // изменение коэффициента alpha
    void on_ShiftFreqSB_valueChanged(double new_freq_shift); // изменение частотного сдвига
    void on_DCOffsetRe_valueChanged(double new_offset_re);
    void on_DCOffsetIm_valueChanged(double new_offset_im);
    void on_FftShiftStepCB_clicked(bool new_state); // кратность шага изменения частоты сноса
    void on_NullBinCircleCB_clicked(bool new_state); // вершина нулевого бина обводится окружностью


/// АСКР
    void on_FirstValidatePB_clicked(); // АСКР проверка №1
    void on_comboBox_currentIndexChanged(int new_pres_idx); // выбор преселектора

signals:
    void global_update_interface();
    void previousPositionChanged(QPoint previousPosition);
    void prepair_wav_recorder();
    void bind_slots_signals();
    void stop();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // CONFIG_MANAGER_H
