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

    void on_CloseButton_clicked();

    void update_reader_rec_label();
    void update_flt_rec_label();
    void update_shift_rec_label();
    void update_sound_rec_label();

private slots:
/// -----------------------------------РПУ---------------------------------------------
    void on_LPTPortCB_currentIndexChanged(int new_lpt_port_idx); // смена адреса LPT порта
    void on_TractConfigCB_currentIndexChanged(int new_rpu_config_idx); // смена конфигурации трактов

    // тракт 1
    void on_Tract1_CentralFreqSB_valueChanged(int new_central_freq);
    void on_Tract1_AttInBox_currentIndexChanged(int new_in_att_idx);
    void on_Tract1_AttHFBox_currentIndexChanged(int new_hf_att_idx);
    void on_Tract1_AttIFBox_currentIndexChanged(int new_att_if_idx);
    void on_Tract1_BandIFBox_currentIndexChanged(int new_band_if_idx);

    // калибратор
    void on_KalibStatusBox_currentIndexChanged(int new_work_status_idx); // включение выключение калибратора
    void on_KalibAttenTypeBox_currentIndexChanged(int new_att_idx); // изменение значения аттенюатора калибратора
    void on_KalibExitTypeBox_currentIndexChanged(int new_exit_type_idx); // изменение выхода калибратора
    void on_KalibModulationFrequencyBox_valueChanged(int new_modulation_freq_in_kHz); // изменение частоты модуляции
    void on_KalibSignalTypeBox_currentIndexChanged(int new_signal_type_idx); // изменение типа модулирующего сигнала

    // АСКР
    void on_FirstValidatePB_clicked();
    void on_spinBox_valueChanged(int _presNb);
    void on_FreqPresNbSB_valueChanged(int _freqPresNb);

/// -----------------------------------SDR---------------------------------------------
    void on_ReloadButton_clicked(); // поиск новых приемников
    void on_ClearButton_clicked(); // очистка информационного поля
    void on_ConnectButton_clicked(); // подключение к новому приемнику
    void on_DisconnectButton_clicked(); // отключение от приемника
    void on_SdrSampleRateCB_currentIndexChanged(int new_sample_rate_idx); // изменение частоты дискретизации
    void on_GainValBox_currentIndexChanged(int new_gain_idx); // изменение КУ
    void on_DirectSamplingCB_currentIndexChanged(int new_direct_sampling_idx); // изменение режима оцифровки
    void on_RtlAgcCB_clicked(bool new_agc_state);

/// -----------------------------------DSP---------------------------------------------
// запись
    void on_ADCSignalCB_clicked(bool new_state);
    void on_AdcPathButton_clicked();
    void on_FltSignalCB_clicked(bool new_state);
    void on_FltPathButton_clicked();
    void on_ShiftSignalCB_clicked(bool new_state);
    void on_ShiftPathButton_clicked();
    void on_SoundSignalCB_clicked(bool new_state);
    void on_SoundPathButton_clicked();

// фильтрация
    void on_FltBwSB_valueChanged(int _fltBw); // изменение относительной частоты фильтрации

// БПФ
    void on_FftModeCB_currentIndexChanged(int new_fft_mode); // смена "режима" БПФ
    void on_ReadOutPerSecSB_valueChanged(int new_readout_per_sec); // изменение FPS
    void on_AverageNumSB_valueChanged(int new_fft_averages_number); // изменение числа усреднений
    void on_InversionCB_clicked(bool new_fft_inversion); // смена флага инверсии спектра
    void on_WindowCB_currentIndexChanged(int new_win_idx); // выбор окна
    void on_WindowAlphaSB_valueChanged(double new_aplha); // изменение коэффициента alpha
    void on_ShiftFreqSB_valueChanged(double new_r_frec); // изменение частотного сдвига
    void on_DCOffsetGain_valueChanged(double new_dc_gain);
    void on_FftShiftStepCB_clicked(bool new_state); // кратность шага изменения частоты сноса
    void on_NullBinCircleCB_clicked(bool new_state); // вершина нулевого бина обводится окружностью

// демодуляция
    void on_AMDemodRB_clicked();
    void on_USBDemodRB_clicked();
    void on_LSBDemodRB_clicked();
    void on_FMDemodRB_clicked();
    void on_ScaleFactorSB_valueChanged(int _scFct);


signals:
    void global_update_interface();
    void previousPositionChanged(QPoint previousPosition);
    void prepair_wav_recorder();
    void first_rpu_test(); // первый тест РПУ
    void stop_receiving();
    void start_receiving();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // CONFIG_MANAGER_H
