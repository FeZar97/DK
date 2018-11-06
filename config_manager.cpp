#include "config_manager.h"
#include "ui_config_manager.h"

config_manager::config_manager(QWidget *parent,
                               RPU *new_rpu,
                               SDR *new_sdr,
                               DSP *new_dsp) : QDialog(parent), ui(new Ui::config_manager)
{
    ui->setupUi(this);
    this->setVisible(false);

    if(new_rpu != NULL)
        rpu = new_rpu;
    else
        rpu = new RPU();

    if(new_dsp != NULL)
        dsp = new_dsp;
    else
        dsp = new DSP();

    if(new_sdr != NULL)
        sdr = new_sdr;
    else
        sdr = new SDR();

    on_RefreshButton_clicked();

    set_ui_style();

    // Закрытие окна приложения
    connect(ui->CloseButton, &QToolButton::clicked, this, &QWidget::close);

    ui->RefreshButton->setStyleSheet(StyleHelper::getRefreshStyleSheet());
    ui->ConfirmButton->setStyleSheet(StyleHelper::getConfirmStyleSheet(true));
}

config_manager::~config_manager()
{
    delete ui;
}

QPoint config_manager::previousPosition() const
{
    return m_previousPosition;
}

void config_manager::setPreviousPosition(QPoint previousPosition)
{
    if (m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
}

// обновление интерфейса
void config_manager::update_interface()
{
    // обновление вкладки "РПУ"
    update_rpu_tab();

    // обновление вкладки "SDR"
    update_sdr_tab();

    // обновление вкладки "ЦОС"
    update_dsp_tab();
}

// обновление вкладки "РПУ"
void config_manager::update_rpu_tab()
{
    // выставление значений
    ui->LPTPortCB->setCurrentIndex(rpu->get_lpt_idx());
    ui->TractConfigCB->setCurrentIndex(rpu->get_config_idx());

    ui->KalibStatusBox->setCurrentIndex(rpu->kalibrator.get_work_status());
    ui->KalibAttenTypeBox->setCurrentIndex(rpu->kalibrator.get_att_idx());
    ui->KalibExitTypeBox->setCurrentIndex(rpu->kalibrator.get_exit_type());
    ui->KalibModulationFrequencyBox->setValue(rpu->kalibrator.get_modulation_freq() / 1000);
    ui->KalibSignalTypeBox->setCurrentIndex(rpu->kalibrator.get_signal_type());

    //калибратор
    bool enabled_flag;
    enabled_flag = (rpu->kalibrator.get_work_status() == OFF ? false : true);

    ui->KalibAttenLabel->setEnabled(enabled_flag);
    ui->KalibAttenTypeBox->setEnabled(enabled_flag);

    ui->KalibExitTypeLabel->setEnabled(enabled_flag);
    ui->KalibExitTypeBox->setEnabled(enabled_flag);

    ui->KalibModulationFrequencyLabel->setEnabled(enabled_flag);
    ui->KalibModulationFrequencyBox->setEnabled(enabled_flag);
    ui->KalibKHZLabel->setEnabled(enabled_flag);

    ui->KalibSignalTypeLabel->setEnabled(enabled_flag);
    ui->KalibSignalTypeBox->setEnabled(enabled_flag);

    // доступность трактов
    ui->RPUTabs->setTabEnabled(ZERO,   rpu->first_tract.get_tract_active());
    ui->RPUTabs->setTabEnabled(FIRST,  rpu->second_tract.get_tract_active());
    ui->RPUTabs->setTabEnabled(SECOND, rpu->third_tract.get_tract_active());
    ui->RPUTabs->setTabEnabled(THIRD,  rpu->fourth_tract.get_tract_active());

    // настройки трактов
    ui->Tract1_CentralFreqSB->setValue(rpu->first_tract.get_central_freq() / 1000);
    ui->Tract1_AttInBox->setCurrentIndex(rpu->first_tract.get_in_att_idx());
    ui->Tract1_AttHFBox->setCurrentIndex(rpu->first_tract.get_hf_att_idx());
    ui->Tract1_AttIFBox->setCurrentIndex(rpu->first_tract.get_if_att_idx());
    ui->Tract1_BandIFBox->setCurrentIndex(int(rpu->first_tract.get_if_band_idx()));
}

// обновление вкладки "SDR"
void config_manager::update_sdr_tab()
{
// выставление значений
    // если приемник еще не добавлен, но есть доступные приемники
    if(!sdr->sdr_params->is_open && sdr->get_sdr_count()){
        ui->CurrentDevSpinBox->setMaximum(sdr->get_sdr_count() - 1);
        ui->CurrentDevSpinBox->setMinimum(0);
    }else{
        // если приемник все еще не открыт
        if(!sdr->sdr_params->is_open){
            ui->CurrentDevSpinBox->setMaximum(-1);
            ui->CurrentDevSpinBox->setMinimum(-1);
        }
    }
    ui->CurrentDevSpinBox->setValue(sdr->sdr_params->index);
    ui->SampleRateSB->setValue(sdr->sdr_params->sample_rate);
    ui->GainValBox->setCurrentIndex(sdr->sdr_params->gain_idx);
    ui->CentralFreqSB->setValue(sdr->sdr_params->central_freq);
    ui->DirectSamplingCB->setCurrentIndex(sdr->sdr_params->direct_sampling_idx);

    // если приемник не добвлен, то кнопка доступна
    // если приемник уже подключен, то обновлять незачем
    ui->RefreshButton->setEnabled(!sdr->sdr_params->is_open);

    // если приемник еще не добавлен, но есть доступные приемники
    ui->CurrentDevNumLabel->setEnabled(!sdr->sdr_params->is_open && sdr->get_sdr_count());
    ui->CurrentDevSpinBox->setEnabled(!sdr->sdr_params->is_open && sdr->get_sdr_count());

    // кнопка выбора индекса используемого приемника также доступна, пока примник не подключен
    // и есть свободные приемники
    ui->ConfirmButton->setEnabled(!sdr->sdr_params->is_open && sdr->get_sdr_count());

    // частота дискретизации может меняться только при условии подключенного приемника и пока не идет запись
    ui->SampleRateLabel->setEnabled(sdr->sdr_params->is_open && !dsp->dsp_params->read_params->is_recording);
    ui->SampleRateSB->setEnabled(sdr->sdr_params->is_open);// && !handle->dsp->get_is_recording());

    // КУ может меняться с момента подключения приемника
    ui->GainLabel->setEnabled(sdr->sdr_params->is_open);
    ui->GainValBox->setEnabled(sdr->sdr_params->is_open);

    // центральная частота настройки может меняться с момента подключения приемника
    ui->CentralFreqLabel->setEnabled(sdr->sdr_params->is_open);
    ui->CentralFreqSB->setEnabled(sdr->sdr_params->is_open);

    // режим оцифровки может меняться только при условии подключенного приемника и пока не идет запись
    ui->DirectSamplingLabel->setEnabled(sdr->sdr_params->is_open && !dsp->dsp_params->read_params->is_recording);
    ui->DirectSamplingCB->setEnabled(sdr->sdr_params->is_open && !dsp->dsp_params->read_params->is_recording);
}

// обновление вкладки "ЦОС"
void config_manager::update_dsp_tab()
{
    bool enabled_flag = !dsp->dsp_params->read_params->is_recording;

    // значения
    ui->ShiftFreqSB->setMaximum(sdr->sdr_params->sample_rate / 2);
    ui->ShiftFreqSB->setMinimum((-1) * double(sdr->sdr_params->sample_rate / 2));
    ui->ShiftFreqSB->setValue(dsp->dsp_params->shift_params->shift_freq);
    ui->InversionCB->setChecked(dsp->dsp_params->fft_params->fft_inversion);
    ui->FftModeCB->setCurrentIndex(dsp->dsp_params->fft_params->fft_mode);
    ui->MaxAverageNumber->setText(QString::number(dsp->dsp_params->fft_params->fft_max_averages_number));
    ui->AverageNumSB->setMaximum(dsp->dsp_params->fft_params->fft_max_averages_number);
    ui->AverageNumSB->setValue(dsp->dsp_params->fft_params->fft_averages_number);
    ui->InfoCB->setChecked(dsp->dsp_params->fft_params->fft_info);
    ui->ReadOutPerSecSB->setValue(dsp->dsp_params->read_params->readout_per_seconds);
    ui->RecordTimeCB->setCurrentIndex(dsp->dsp_params->read_params->rec_time_idx);
    ui->DCOffsetSB->setValue(dsp->dsp_params->read_params->dc_offset);

    // доступность изменения частоты дискретизации
    ui->ReadOutPerSecLabel->setEnabled(enabled_flag);
    ui->ReadOutPerSecSB->setEnabled(enabled_flag);

    // доступность чек-боксов записи
    ui->ADCSignalCB->setChecked(dsp->dsp_params->read_params->use_first_file);
    if(!dsp->dsp_params->read_params->use_first_file)
        ui->ADCBar->setValue(0);
    ui->FltSignalCB->setChecked(dsp->dsp_params->read_params->use_second_file);
    if(!dsp->dsp_params->read_params->use_second_file)
        ui->FltBar->setValue(0);
    ui->RealSignalCB->setChecked(dsp->dsp_params->read_params->use_third_file);
    if(!dsp->dsp_params->read_params->use_third_file)
        ui->RealBar->setValue(0);

    // флаг записи в файлы
    // true, если хотя бы один файл выбран и открыт(запись в него все еще идет)
    enabled_flag =  (dsp->dsp_params->read_params->use_first_file  && dsp->dsp_params->read_params->is_recording)
                 || (dsp->dsp_params->read_params->use_second_file && dsp->dsp_params->read_params->is_recording)
                 || (dsp->dsp_params->read_params->use_third_file  && dsp->dsp_params->read_params->is_recording);

    ui->RecordButton->setEnabled(enabled_flag);

    // если флаг кратности == true, то шаг = (частота дискретизации) / (DSP_FFT_SIZE)
    // если флаг кратности == false, то шаг = 1
    ui->FftShiftStepCB->setChecked(dsp->dsp_params->shift_params->step);
    if(dsp->dsp_params->shift_params->step)
        ui->ShiftFreqSB->setSingleStep(sdr->sdr_params->sample_rate / float(DSP_FFT_SIZE));
    else
        ui->ShiftFreqSB->setSingleStep(50.);
}

// применение стиля
void config_manager::set_ui_style()
{
    this->setWindowFlags(Qt::FramelessWindowHint);              // Отключаем оформление окна
    this->setAttribute(Qt::WA_TranslucentBackground);           // Делаем фон главного виджета прозрачным
    this->setStyleSheet(StyleHelper::getWindowStyleSheet());    // Устанавливаем стиль виджета
    // Установка стилей для всех элементов
    ui->WindowTitleLabel->setStyleSheet(StyleHelper::getLabelStyleSheet());
    ui->CloseButton->setStyleSheet(StyleHelper::getCloseStyleSheet());
    ui->CloseButton->setStyleSheet(StyleHelper::getCloseStyleSheet());
    ui->RPUMainTopLine->setStyleSheet(StyleHelper::getLineStyleSheet());
}

void config_manager::update_adc_bar(int new_val)
{
    ui->ADCBar->setValue(new_val);
}

void config_manager::update_flt_bar(int new_val)
{
    ui->FltBar->setValue(new_val);
}

void config_manager::update_real_bar(int new_val)
{
    ui->RealBar->setValue(new_val);
}

/// --------------------------------РПУ----------------------------------------
// смена адреса LPT порта
void config_manager::on_LPTPortCB_currentIndexChanged(int new_lpt_port_idx)
{
    rpu->set_lpt_idx(new_lpt_port_idx);
    emit global_update_interface();
}

// смена конфигурации трактов
void config_manager::on_TractConfigCB_currentIndexChanged(int new_rpu_config_idx)
{
    rpu->set_config(RPU_CONFIG(new_rpu_config_idx));
    emit global_update_interface();
}

// включение выключение калибратора
void config_manager::on_KalibStatusBox_currentIndexChanged(int new_work_status_idx)
{
    rpu->kalibrator.set_work_status(STATUS(new_work_status_idx));
    emit global_update_interface();
}

// изменение значения аттенюатора калибратора
void config_manager::on_KalibAttenTypeBox_currentIndexChanged(int new_att_idx)
{
    rpu->kalibrator.set_att_idx(new_att_idx);
}

// изменение выхода калибратора
void config_manager::on_KalibExitTypeBox_currentIndexChanged(int new_exit_type_idx)
{
    rpu->kalibrator.set_exit_type(KALIB_TYPE(new_exit_type_idx));
}

// изменение частоты модуляции
void config_manager::on_KalibModulationFrequencyBox_valueChanged(int new_modulation_freq_in_kHz)
{
    rpu->kalibrator.set_modulation_freq(new_modulation_freq_in_kHz);
}

// изменение типа модулирующего сигнала
void config_manager::on_KalibSignalTypeBox_currentIndexChanged(int new_signal_type_idx)
{
    rpu->kalibrator.set_signal_type(MOD_SIGNAL(new_signal_type_idx));
}

void config_manager::on_Tract1_CentralFreqSB_valueChanged(int new_central_freq)
{
    rpu->first_tract.set_central_freq(new_central_freq * 1000);
}

void config_manager::on_Tract1_AttInBox_currentIndexChanged(int new_in_att_idx)
{
    rpu->first_tract.set_in_att_idx(new_in_att_idx);
}

void config_manager::on_Tract1_AttHFBox_currentIndexChanged(int new_hf_att_idx)
{
    rpu->first_tract.set_hf_att_idx(new_hf_att_idx);
}

void config_manager::on_Tract1_AttIFBox_currentIndexChanged(int new_att_if_idx)
{
    rpu->first_tract.set_if_att_idx(new_att_if_idx);
}

void config_manager::on_Tract1_BandIFBox_currentIndexChanged(int new_band_if_idx)
{
    rpu->first_tract.set_if_band_idx(new_band_if_idx);
}

/// --------------------------------SDR----------------------------------------
// поиск новых приемников
void config_manager::on_RefreshButton_clicked()
{
    update_interface();
}

// выбор приемника по его индексу
void config_manager::on_ConfirmButton_clicked()
{
    // если приемник еще не занят
    if(!sdr->sdr_params->is_open || (sdr->sdr_params->is_open && sdr->sdr_params->is_already_open)){
        int result = rtlsdr_open(&(sdr->sdr_params->sdr_ptr), ui->CurrentDevSpinBox->value());

        if(!result || (result && sdr->sdr_params->is_already_open)){
            sdr->sdr_params->index = ui->CurrentDevSpinBox->value();
            sdr->sdr_params->is_open = true;

            emit global_update_interface();
        }else{
            sdr->sdr_params->is_open = false;
        }
    }else{
        sdr->sdr_params->is_open = false;
    }

    update_interface();
}

// изменение частоты настройки
void config_manager::on_CentralFreqSB_editingFinished()
{
    sdr->set_central_freq((unsigned int)ui->CentralFreqSB->value());
    emit global_update_interface();
}

// изменение частоты дисркетизации
void config_manager::on_SampleRateSB_valueChanged(int new_sample_rate)
{
    sdr->set_sample_rate(new_sample_rate);
    emit global_update_interface();
}

// смена КУ
void config_manager::on_GainValBox_currentIndexChanged(int new_gain_idx)
{
    sdr->set_gain_idx(new_gain_idx);
    emit global_update_interface();
}

// смена режима сэмплирование
void config_manager::on_DirectSamplingCB_currentIndexChanged(int new_direct_sampling_idx)
{
    sdr->sdr_params->direct_sampling_idx = new_direct_sampling_idx;
    emit global_update_interface();
}
/// --------------------------------DSP----------------------------------------

// выбор окна
void config_manager::on_WindowCB_currentIndexChanged(int new_win_idx)
{
    dsp->dsp_params->fft_params->fft_current_window = WINDOW(new_win_idx);
    emit global_update_interface();
}

// изменение коэффициента alpha
void config_manager::on_WindowAlphaSB_valueChanged(double new_aplha)
{
    dsp->dsp_params->fft_params->fft_win_alpha = new_aplha;
}

// изменение числа усреднений
void config_manager::on_AverageNumSB_valueChanged(int new_fft_averages_number)
{
    dsp->dsp_params->fft_params->fft_averages_number = new_fft_averages_number;
    emit global_update_interface();
}

// смена "режима" БПФ
void config_manager::on_FftModeCB_currentIndexChanged(int new_fft_mode)
{
    dsp->dsp_params->fft_params->fft_mode = FFT_MODE(new_fft_mode);
    emit global_update_interface();
}

// смена флага инверсии спектра
void config_manager::on_InversionCB_clicked(bool new_fft_inversion)
{
    dsp->dsp_params->fft_params->fft_inversion = new_fft_inversion;
    emit global_update_interface();
}

void config_manager::on_ShiftFreqSB_valueChanged(double new_freq_shift)
{
    dsp->dsp_params->shift_params->shift_freq = new_freq_shift;
    emit global_update_interface();
}

// смена частотного сдвига для fft_shifter
void config_manager::on_DCOffsetSB_valueChanged(int new_dc_offset)
{
    dsp->dsp_params->read_params->dc_offset = new_dc_offset;
    dsp->dsp_params->fft_params->dc_correct = !bool(new_dc_offset);

    emit global_update_interface();
}

// кратность шага изменения частоты сноса
void config_manager::on_FftShiftStepCB_clicked(bool new_state)
{
    dsp->dsp_params->shift_params->step = new_state;
    emit global_update_interface();
}

void config_manager::mousePressEvent(QMouseEvent *event)
{
    // При клике левой кнопкой мыши
    if (event->button() == Qt::LeftButton ) {
        // Определяем, в какой области произошёл клик
        m_leftMouseButtonPressed = checkResizableField(event);
        setPreviousPosition(event->pos()); // и устанавливаем позицию клика
    }
    return QWidget::mousePressEvent(event);
}

void config_manager::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_leftMouseButtonPressed = None;
    }
    return QWidget::mouseReleaseEvent(event);
}

void config_manager::mouseMoveEvent(QMouseEvent *event)
{
    switch (m_leftMouseButtonPressed) {
        case Move:{
            int dx = event->x() - m_previousPosition.x();
            int dy = event->y() - m_previousPosition.y();
            setGeometry(x() + dx, y() + dy, width(), height());
            break;
        }
        default:
            checkResizableField(event);
            break;
    }
    return QWidget::mouseMoveEvent(event);
}

// флаг записи в первый файл
void config_manager::on_ADCSignalCB_clicked(bool new_use_first_file)
{
    dsp->dsp_params->read_params->use_first_file = new_use_first_file;
    global_update_interface();
}

// флаг записи во второй файл
void config_manager::on_FltSignalCB_clicked(bool new_use_second_file)
{
    dsp->dsp_params->read_params->use_second_file = new_use_second_file;
    global_update_interface();
}

// флаг записи в третий файл
void config_manager::on_RealSignalCB_clicked(bool new_use_third_file)
{
    dsp->dsp_params->read_params->use_third_file = new_use_third_file;
    global_update_interface();
}

// смена времени записи
void config_manager::on_RecordTimeCB_currentIndexChanged(int new_recording_time_idx)
{
    dsp->dsp_params->read_params->rec_time_idx = new_recording_time_idx;
    dsp->recalc_dsp_params();
}

// изменение кол-ва усреднений в секунду
void config_manager::on_ReadOutPerSecSB_valueChanged(int new_readout_per_sec)
{
    dsp->dsp_params->read_params->readout_per_seconds = new_readout_per_sec;
    dsp->recalc_dsp_params();
    emit global_update_interface();
}

// вывод информации на спектр
void config_manager::on_InfoCB_clicked(bool new_fft_info)
{
    dsp->dsp_params->fft_params->fft_info = new_fft_info;
    emit global_update_interface();
}

MouseType config_manager::checkResizableField(QMouseEvent *event)
{
    QPointF position = event->screenPos(); // Определяем позицию курсора на экране
    qreal x = this->x(); // координаты окна приложения, ...
    qreal y = this->y(); // ... то есть координату левого верхнего угла окна
    qreal width = this->width(); // А также ширину ...
    qreal height = this->height(); // А также ширину ...

    QRectF rectInterface(x, y, width, height);

    if(rectInterface.contains(position))
        return Move;
    else
        return None;
}

void config_manager::on_CloseButton_clicked()
{
    this->setVisible(!this->isVisible());
}
void config_manager::on_RecordButton_clicked()
{
    dsp->dsp_params->read_params->use_files = true;

    emit prepair_wav_recorder(); // подготовка wav_recorders к началу записи
}
// выбор выходной папки
void config_manager::on_CurrentPathButton_clicked()
{
    dsp->dsp_params->wav_params->directory = QFileDialog::getExistingDirectory(this, "Выходная директория", dsp->dsp_params->wav_params->directory) + "/";
}


/*
 первый этап работы АСКР
    производится перестройка на центральные частоты преселекторов и измеряется уровень калибровочного сигнала
   200  - 1000  : 600 кГц
   1000 - 1500  : 1250 кГц
   1500 - 2200  : 1850 кГц
   2200 - 3200  : 2700 кГц
   3200 - 4700  : 3950 кГц
   4700 - 6800  : 5750 кГц
   6800 - 9900  : 8350 кГц
   9900 - 14400 : 12150 кГц
  14400 - 20700 : 17550 кГц
  20700 - 32000 : 26350 кГц

  1) выставление четырехканального режима РПУ
  2) выставление центральной частоты канала
  3) настройка калибратора
  4) код начала калибровки - строка 4 в таблице кодограмм
  5) измерения
  6) код окончания калибровки - строка 5 в таблице кодограмм
*/
void config_manager::on_FirstValidatePB_clicked()
{
    if(dsp->dsp_params->read_params->is_recording)
        emit stop();

// настройки SDR
    sdr->sdr_params->central_freq = 5500000;
    sdr->sdr_params->direct_sampling_idx = 2;
    sdr->sdr_params->gain_idx = 1;
    sdr->sdr_params->sample_rate = 1400000;

// настройки тракта РПУ
    rpu->first_tract.set_central_freq(15000000);
    rpu->first_tract.set_if_band_idx(3);
    rpu->first_tract.set_in_att_idx(OFF);
    rpu->first_tract.set_hf_att_idx(0);
    rpu->first_tract.set_if_att_idx(0);

// настройки калибратора
    rpu->kalibrator.set_work_status(ON);
    rpu->kalibrator.set_att_idx(0);
    rpu->kalibrator.set_exit_type(INTERNAL);
    rpu->kalibrator.set_signal_type(SINUS);

    if(dsp->prepair_to_record(sdr)){
        emit bind_slots_signals();

        dsp->fft_shift_thread->start();
        dsp->fft_thread->start();

        dsp->reader_thread->start();
    }
    else{
        QMessageBox::critical(this, "Ошибка запуска", "Не удалось начать калибровку. Ошибка 1.");
        sdr->sdr_params->is_open = false;
        dsp->set_record_flags(false);
    }

    // на данном моменте на спектре должна быть видна синусоида калибратора

    int freqs[10] = {600000, 1250000, 1850000, 2700000, 3950000, 5750000, 8350000, 12150000, 17550000, 26350000}, i, j, n_read;

    // 10 обращений в секунду
    //dsp->dsp_params->read_params->readout_per_seconds = 10;
    //dsp->recalc_dsp_params();

    char *input_buffer = new char[dsp->dsp_params->read_params->read_rb_cell_size];

    /*
    // если подготовка к записи завершена успешно, то начинается прием
    if(dsp->prepair_to_record(sdr)){
        connect(dsp->reader,            &READER::update_ReadProgressBar,        this,                &Widget::update_ReadProgressBar);
        connect(dsp->reader,            &READER::end_of_recording,              this,                &Widget::end_of_recording);
        connect(dsp->fft,               &fft_calcer::paint_fft,                 this,                &Widget::paint_fft);
        connect(dsp->first_wav_rec,     &wav_recorder::end_of_recording,        this,                &Widget::end_of_first_file_rec);
        connect(dsp->second_wav_rec,    &wav_recorder::end_of_recording,        this,                &Widget::end_of_second_file_rec);
        connect(dsp->third_wav_rec,     &wav_recorder::end_of_recording,        this,                &Widget::end_of_third_file_rec);
        connect(dsp->first_wav_rec,     &wav_recorder::update_progr_bar,        config_manager_form, &config_manager::update_adc_bar);
        connect(dsp->second_wav_rec,    &wav_recorder::update_progr_bar,        config_manager_form, &config_manager::update_flt_bar);
        connect(dsp->third_wav_rec,     &wav_recorder::update_progr_bar,        config_manager_form, &config_manager::update_real_bar);

        dsp->wav_thread->start();
        dsp->fft_shift_thread->start();
        dsp->fft_thread->start();
        global_update_interface();


        for(i = 0; i < 10; i++){
            rpu->set_config(FOUR_CHANNEL);
            rpu->first_tract.set_central_freq(freqs[i]);

            // в начале накопить инфу о шуме
            // для статистики шума требуется DSP_NOISE_SIZE обращений к приемнику

            j = 0;
            // считывание просиходит до тех пор, пока либо не кончится время, либо не произойдет принудительная остановка
            while(j < DSP_NOISE_SIZE){
                rtlsdr_reset_buffer(sdr->sdr_params->sdr_ptr);

                // общение с железкой
                rtlsdr_read_sync(sdr->sdr_params->sdr_ptr,
                                 dsp->dsp_params->read_params->read_cell,
                                 dsp->dsp_params->read_params->read_rb_cell_size,
                                 &n_read);

                // конвертация 8u -> 32f
                ippsConvert_8u32f(dsp->dsp_params->read_params->read_cell,
                                 (Ipp32f*)dsp->dsp_params->read_params->read_rb[dsp->dsp_params->read_params->read_rb_cell_idx],
                                  dsp->dsp_params->read_params->read_rb_cell_size);


                // сопряжение
                if(dsp->dsp_params->fft_params->fft_inversion)
                    ippsConj_32fc_I(dsp->dsp_params->read_params->read_rb[dsp->dsp_params->read_params->read_rb_cell_idx],
                                    dsp->dsp_params->read_params->read_rb_cell_size / 2);

                // АКВАФОР
                // убирает вредное, сохраняя полезное
                //if(dsp_params->flt_params->use_filter)
                //    emit get_filtration_step(read_rb[dsp_params->flt_params->filtration_rb_cell_idx]);

                // инкремент итератора
                dsp->dsp_params->read_params->read_rb_cell_idx = (dsp_params->read_params->read_rb_cell_idx + 1) % DSP_READ_RB_SIZE;
            }

            // если завершилось штатно, то флаг emergency_end_recording == 0
            // если завершилось экстренно, то emergency_end_recording == 1
            emit end_of_recording(!dsp_params->read_params->emergency_end_recording);
        }

    }
    else{
        QMessageBox::critical(this, "Ошибка запуска", "Не удалось начать запись сигнала.\nЗаново подключите SDR приемник и перезапустите программу.");
        sdr->sdr_params->is_open = false;
        dsp->set_record_flags(false);
        global_update_interface();
    }
    */
}

void config_manager::on_comboBox_currentIndexChanged(int new_pres_idx)
{
    short  band_mask, preselektor_mask, InputAttenuator_mask;
    short  HFAttenuator_mask, IFAttenuator_mask, D4;

    rpu->set_config_to_RPU();

    switch(rpu->first_tract.get_if_band_idx()){
        case FOURTH_BAND: // 150 кГц
            band_mask = 0x0003;
            break;

        case THIRD_BAND: // 20 кГц
            band_mask = 0x0001;
            break;

        case SECOND_BAND: // 8 кГц
            band_mask = 0x0002;
            break;

        case FIRST_BAND: // 3 кГц
            band_mask = 0x0000;
            break;
    }

    preselektor_mask = 0x0009 - new_pres_idx;

    InputAttenuator_mask = 0;
    if(rpu->first_tract.get_in_att_idx())
       InputAttenuator_mask = 0x0001;
    else
       InputAttenuator_mask = 0x0000;

    HFAttenuator_mask = rpu->first_tract.get_hf_att_idx();
    IFAttenuator_mask = rpu->first_tract.get_if_att_idx();
    D4 = (short)((band_mask << 12) | (preselektor_mask << 8) | (InputAttenuator_mask << 7) | (HFAttenuator_mask << 4) | IFAttenuator_mask);
    D4 <<= 2;
    rpu->send_code(rpu->get_lpt_addres() + LPT_CONTROL_REG, 0xA);
    char ManageRPUKod = 0;
    for(int i = 0; i < 14; i++){
        ManageRPUKod  = 0x80;
        ManageRPUKod |= char( (D4 & 0x8000) >> 10 );
        rpu->send_code(rpu->get_lpt_addres() + LPT_DATA_REG, ManageRPUKod);
        rpu->send_code(rpu->get_lpt_addres() + LPT_CONTROL_REG, 0xB);
        rpu->send_code(rpu->get_lpt_addres() + LPT_CONTROL_REG, 0xA);
        ManageRPUKod ^= 0x40;
        rpu->send_code(rpu->get_lpt_addres() + LPT_DATA_REG, ManageRPUKod);
        rpu->send_code(rpu->get_lpt_addres() + LPT_CONTROL_REG, 0xB);
        rpu->send_code(rpu->get_lpt_addres() + LPT_CONTROL_REG, 0xA);

        D4 <<= 1;
    }
    ManageRPUKod = 0x00;
    rpu->send_code(rpu->get_lpt_addres() + LPT_DATA_REG, ManageRPUKod);
    rpu->send_code(rpu->get_lpt_addres() + LPT_CONTROL_REG, 0xB);
    rpu->send_code(rpu->get_lpt_addres() + LPT_CONTROL_REG, 0xA);
}
