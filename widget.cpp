#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget), settings{"K722", "DigitalKalmar"}
{
    ui->setupUi(this);

    this->setMouseTracking(true);

    rpu                 = new RPU();
    sdr                 = new SDR();
    dsp                 = new DSP();
    config_manager_form = new config_manager(nullptr, rpu, sdr, dsp);

    ui->FftGraph->sdr_params = sdr->sdr_params;
    ui->FftGraph->dsp_params = dsp->dsp_params;

    connect(config_manager_form,    &config_manager::bind_slots_signals,        this,                   &Widget::bind_slots_signals);
    connect(config_manager_form,    &config_manager::stop,                      this,                   &Widget::on_StopButton_clicked);
    connect(config_manager_form,    &config_manager::global_update_interface,   this,                   &Widget::global_update_interface);
    connect(config_manager_form,    &config_manager::prepair_wav_recorder,      dsp,                    &DSP::prepair_wav_recorder);
    connect(sdr,                    &SDR::recalc_dsp_params,                    dsp,                    &DSP::recalc_dsp_params);
    connect(sdr,                    &SDR::global_update_interface,              this,                   &Widget::global_update_interface);
    connect(ui->MinimizeButton,     &QToolButton::clicked,                      this,                   &Widget::showMinimized);

    set_ui_style();

    restore_settings();
}

Widget::~Widget()
{
    delete config_manager_form;
    delete ui;
}

// --------------------------- public slots ---------------------------

void Widget::setPreviousPosition(QPoint previousPosition)
{
    if(m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
    emit previousPositionChanged(previousPosition);
}

// отрисовка спектра
void Widget::paint_fft()
{
    ui->FftGraph->repaintGraph();
}

// слот окончания записи
// принимает сигнал 1) при экстренном завершении записи (нажатие кнопки стоп)
//                  2) при штатном завершении записи
// при нажатии на кнопку стоп все воркеры сами вышлют сигналы окончания своей работы.
// как только придет сигнал от последнего воркера, все удалится и закроется
void Widget::end_of_recording()
{
    dsp->dsp_params->read_params->is_recording = false;

    if(dsp->first_wav_rec->params->file.isOpen())
        dsp->first_wav_rec->params->file.close();

    if(dsp->second_wav_rec->params->file.isOpen())
        dsp->second_wav_rec->params->file.close();

    if(dsp->third_wav_rec->params->file.isOpen())
        dsp->third_wav_rec->params->file.close();

    dsp->wav_thread->quit();
    dsp->fft_shift_thread->quit();
    dsp->fft_thread->quit();
    dsp->filtration_thread->quit();
    dsp->reader_thread->quit();

    global_update_interface();
}

// обновление интерфейса
void Widget::global_update_interface()
{
    // обновление конфигуратор
    config_manager_form->update_interface();

    // доступность кнопки записи и кнопки останова
    if(sdr->sdr_params->is_open){

        // if is_rec_active == true
        //      PLAY_BTN is disabled, STOP_BTN is enabled
        // else conversely
        bool is_rec_active = dsp->dsp_params->read_params->is_recording;

        ui->RecButton->setEnabled(!is_rec_active);
        ui->RecButton->setStyleSheet(StyleHelper::getRecStyleSheet(!is_rec_active));

        ui->StopButton->setEnabled(is_rec_active);
        ui->StopButton->setStyleSheet(StyleHelper::getStopStyleSheet(is_rec_active));

    }else{
        ui->RecButton->setEnabled(false);
        ui->RecButton->setStyleSheet(StyleHelper::getRecStyleSheet(false));

        ui->StopButton->setEnabled(false);
        ui->StopButton->setStyleSheet(StyleHelper::getStopStyleSheet(false));
    }

    ui->DynamicRangeSlider->setValue(int(dsp->dsp_params->fft_params->fft_dynamic_range));
    ui->NoiseLevelSlider->setValue(dsp->dsp_params->fft_params->fft_noise_level);

    ui->FftGraph->repaintGraph();
}

// управление стилем окна
void Widget::set_ui_style()
{
    this->setWindowFlags(Qt::FramelessWindowHint);      // отключение оформления окна
    this->setAttribute(Qt::WA_TranslucentBackground);   // фон главного виджета прозрачным

    this->setStyleSheet(StyleHelper::getWindowStyleSheet());    // стиль виджета
    ui->WindowTitleLabel->setStyleSheet(StyleHelper::getLabelStyleSheet()); // заголовок

    ui->RecButton->setStyleSheet(StyleHelper::getRecStyleSheet(true));
    ui->StopButton->setStyleSheet(StyleHelper::getStopStyleSheet(true));
    ui->ConfigButton->setStyleSheet(StyleHelper::getConfigStyleSheet());

    ui->MinimizeButton->setStyleSheet(StyleHelper::getMinimizeStyleSheet());
    ui->CloseButton->setStyleSheet(StyleHelper::getCloseStyleSheet());

    config_manager_form->set_ui_style();
}

void Widget::end_of_first_file_rec()
{
    dsp->dsp_params->read_params->use_first_file = false;
    global_update_interface();
}
void Widget::end_of_second_file_rec()
{
    dsp->dsp_params->read_params->use_second_file = false;
    global_update_interface();
}
void Widget::end_of_third_file_rec()
{
    dsp->dsp_params->read_params->use_third_file = false;
    global_update_interface();
}

// сигнально-слотовые соединения между объектами
void Widget::bind_slots_signals()
{
    connect(dsp->reader,            &READER::end_of_recording,                  this,                   &Widget::end_of_recording);
    connect(dsp->fft,               &fft_calcer::paint_fft,                     this,                   &Widget::paint_fft);
    connect(dsp->first_wav_rec,     &wav_recorder::end_of_recording,            this,                   &Widget::end_of_first_file_rec);
    connect(dsp->second_wav_rec,    &wav_recorder::end_of_recording,            this,                   &Widget::end_of_second_file_rec);
    connect(dsp->third_wav_rec,     &wav_recorder::end_of_recording,            this,                   &Widget::end_of_third_file_rec);
    connect(dsp->first_wav_rec,     &wav_recorder::update_progr_bar,            config_manager_form,    &config_manager::update_adc_bar);
    connect(dsp->second_wav_rec,    &wav_recorder::update_progr_bar,            config_manager_form,    &config_manager::update_flt_bar);
    connect(dsp->third_wav_rec,     &wav_recorder::update_progr_bar,            config_manager_form,    &config_manager::update_real_bar);
    //connect(dsp->reader,            &READER::get_sound_step,                    soun,                   &Widget::end_of_recording);
}

// --------------------------- private slots ---------------------------
// открытие/закрытие настройщика
void Widget::on_ConfigButton_clicked()
{
    bool new_visible = !config_manager_form->isVisible();
    config_manager_form->setVisible(new_visible);
    config_manager_form->is_visible = new_visible;
}

// начало записи
void Widget::on_RecButton_clicked()
{
    qDebug() << "on_RecButton_clicked";

    // если подготовка к записи завершена успешно, то начинается прием
    if(dsp->prepair_to_record(sdr)){

        sdr->sdr_params->is_open = true;
        bind_slots_signals();

        qDebug() << "success bind to sdr";

        dsp->start_threads();
    }
    else{
        QMessageBox::critical(this, "Ошибка запуска", "Не удалось начать запись сигнала.\nЗаново подключите SDR приемник и перезапустите программу.");
        sdr->sdr_params->is_open = false;
        dsp->set_record_flags(false);
    }

    global_update_interface();
}

// принудительная остановка записи
void Widget::on_StopButton_clicked()
{
    dsp->dsp_params->read_params->emergency_end_recording = true;
    end_of_recording();
}

// регулировка уровня шума
void Widget::on_NoiseLevelSlider_valueChanged(int new_noise_level)
{
    dsp->dsp_params->fft_params->fft_noise_level = new_noise_level;
    global_update_interface();
}

// регулировка динамического диапзона
void Widget::on_DynamicRangeSlider_valueChanged(int new_dynamic_range)
{
    dsp->dsp_params->fft_params->fft_dynamic_range = uint(new_dynamic_range);
    global_update_interface();
}

// закрытие программы
void Widget::on_CloseButton_clicked()
{
    save_settings();

    delete config_manager_form;

    // если приемник подключен - отключаем его
    if(sdr->sdr_params->is_open) rtlsdr_close(sdr->sdr_params->sdr_ptr);

    this->destroy();

    exit(0);
    qApp->quit();
}

// сохранение настроек
void Widget::save_settings()
{
/// формы
    // положения форм
    settings.setValue("MainGeometry",     saveGeometry());
    settings.setValue("ConfigGeometry",   config_manager_form->saveGeometry());
    settings.setValue("ConfigVisible",    config_manager_form->isVisible());

    // РПУ
    settings.setValue("RPU_lpt_idx",      rpu->get_lpt_idx());
    settings.setValue("RPU_config_idx",   rpu->get_config_idx());
    // калибратор
    settings.setValue("RPU_kalib_signal_type",      int(rpu->kalibrator.get_signal_type()));
    settings.setValue("RPU_kalib_modulation_freq",  rpu->kalibrator.get_modulation_freq() / 1000);
    settings.setValue("RPU_kalib_att_idx",          rpu->kalibrator.get_att_idx());
    settings.setValue("RPU_kalib_exit_type",        int(rpu->kalibrator.get_exit_type()));
    settings.setValue("RPU_kalib_work_status",      int(rpu->kalibrator.get_work_status()));
    // тракт 1
    settings.setValue("RPU_tract1_central_freq",    rpu->first_tract.get_central_freq());
    settings.setValue("RPU_tract1_in_att_idx",      int(rpu->first_tract.get_in_att_idx()));
    settings.setValue("RPU_tract1_hf_att_idx",      rpu->first_tract.get_hf_att_idx());
    settings.setValue("RPU_tract1_if_att_idx",      rpu->first_tract.get_if_att_idx());
    settings.setValue("RPU_tract1_if_band_idx",     int(rpu->first_tract.get_if_band_idx()));

/// SDR
    settings.setValue("SDR_central_freq",           sdr->sdr_params->central_freq);
    settings.setValue("SDR_gain_idx",               sdr->sdr_params->gain_idx);
    settings.setValue("SDR_sample_rate",            sdr->sdr_params->sample_rate);
    settings.setValue("SDR_direct_sampling_idx",    sdr->sdr_params->direct_sampling_idx);
    settings.setValue("SDR_index",                  sdr->sdr_params->index);

/// DSP
    settings.setValue("DSP_is_recording",           dsp->dsp_params->read_params->is_recording);
    settings.setValue("DSP_readout_per_second",     dsp->dsp_params->read_params->readout_per_seconds);
    settings.setValue("DSP_fft_dynamic_range",      dsp->dsp_params->fft_params->fft_dynamic_range);
    settings.setValue("DSP_fft_noise_level",        dsp->dsp_params->fft_params->fft_noise_level);
    settings.setValue("DSP_fft_info",               dsp->dsp_params->fft_params->fft_info);
    settings.setValue("DSP_fft_inversion",          dsp->dsp_params->fft_params->fft_inversion);
    settings.setValue("DSP_fft_mode",               dsp->dsp_params->fft_params->fft_mode);
    settings.setValue("DSP_fft_dc_offset_re",       dsp->dsp_params->fft_params->dc_offset.re);
    settings.setValue("DSP_fft_dc_offset_im",       dsp->dsp_params->fft_params->dc_offset.im);
    settings.setValue("DSP_fft_0_bin_circle",       dsp->dsp_params->fft_params->null_bin_circle);
    settings.setValue("DSP_fft_shift_val",          dsp->dsp_params->shift_params->shift_freq);
    settings.setValue("DSP_fft_shift_step",         dsp->dsp_params->shift_params->step);
    settings.setValue("DSP_wav_directory",          dsp->dsp_params->wav_params->directory);

    settings.sync();
}

// восстановление настроек
void Widget::restore_settings()
{
/// виджеты
    // положение главного окна
    restoreGeometry(settings.value("MainGeometry").toByteArray());
    if(!config_manager_form->restoreGeometry(settings.value("ConfigGeometry").toByteArray()))
        config_manager_form->setGeometry(this->pos().x() + this->width(), this->pos().y(), 1, 1);
    config_manager_form->setVisible(settings.value("ConfigVisible").toBool());

/// РПУ
    rpu->set_lpt_idx(settings.value("RPU_lpt_idx").toInt());
    rpu->set_config(RPU_CONFIG(settings.value("RPU_config_idx").toInt()));
    // калибратор
    rpu->kalibrator.set_signal_type(MOD_SIGNAL(settings.value("RPU_kalib_signal_type").toInt()));
    rpu->kalibrator.set_modulation_freq(ushort(settings.value("RPU_kalib_modulation_freq").toInt()));
    rpu->kalibrator.set_att_idx(ushort(settings.value("RPU_kalib_att_idx").toInt()));
    rpu->kalibrator.set_exit_type(KALIB_TYPE(settings.value("RPU_kalib_exit_type").toInt()));
    rpu->kalibrator.set_work_status(STATUS(settings.value("RPU_kalib_work_status").toInt()));
    // тракт 1
    rpu->first_tract.set_central_freq(uint(settings.value("RPU_tract1_central_freq").toInt()));
    rpu->first_tract.set_in_att_idx(short(settings.value("RPU_tract1_in_att_idx").toBool()));
    rpu->first_tract.set_hf_att_idx(short(settings.value("RPU_tract1_hf_att_idx").toInt()));
    rpu->first_tract.set_if_att_idx(short(settings.value("RPU_tract1_if_att_idx").toInt()));
    rpu->first_tract.set_if_band_idx(short(settings.value("RPU_tract1_if_band_idx").toInt()));

/// SDR
    sdr->sdr_params->central_freq                       = uint(settings.value("SDR_central_freq").toInt());
    sdr->sdr_params->gain_idx                           = uint(settings.value("SDR_gain_idx").toInt());
    sdr->sdr_params->sample_rate                        = uint(settings.value("SDR_sample_rate").toInt());
    sdr->sdr_params->direct_sampling_idx                = uint(settings.value("SDR_direct_sampling_idx").toInt());
    sdr->sdr_params->index                              = settings.value("SDR_index").toInt();

    // коннект к железу
    if(sdr->sdr_params->index != -1)
        sdr->sdr_params->is_open = !bool(rtlsdr_open(&(sdr->sdr_params->sdr_ptr), uint(sdr->sdr_params->index)));
    else
        sdr->sdr_params->is_open = false;

/// DSP
    dsp->dsp_params->read_params->readout_per_seconds   = uint(settings.value("DSP_readout_per_second").toInt());
    dsp->dsp_params->fft_params->fft_dynamic_range      = uint(settings.value("DSP_fft_dynamic_range").toInt());
    dsp->dsp_params->fft_params->fft_noise_level        = settings.value("DSP_fft_noise_level").toInt();
    dsp->dsp_params->fft_params->fft_info               = settings.value("DSP_fft_info").toBool();
    dsp->dsp_params->fft_params->fft_inversion          = settings.value("DSP_fft_inversion").toInt();
    dsp->dsp_params->fft_params->fft_mode               = FFT_MODE(settings.value("DSP_fft_mode").toInt());
    dsp->dsp_params->fft_params->dc_offset.re           = settings.value("DSP_fft_dc_offset_re").toFloat();
    dsp->dsp_params->fft_params->dc_offset.im           = settings.value("DSP_fft_dc_offset_im").toFloat();
    dsp->dsp_params->fft_params->null_bin_circle        = settings.value("DSP_fft_0_bin_circle").toBool();
    dsp->dsp_params->shift_params->shift_freq           = settings.value("DSP_fft_shift_val").toDouble();
    dsp->dsp_params->shift_params->step                 = settings.value("DSP_fft_shift_step").toBool();
    dsp->dsp_params->wav_params->directory              = settings.value("DSP_wav_directory").toString();

    global_update_interface();

    if(settings.value("DSP_is_recording").toBool()) on_RecButton_clicked();
}

// обработка события сворачивания главного окна
void Widget::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);

    if(config_manager_form->is_visible)
        config_manager_form->on_CloseButton_clicked();
}

// обработка события разворачивания главного окна
void Widget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    if(config_manager_form->is_visible)
        on_ConfigButton_clicked();
}

QPoint Widget::previousPosition() const
{
    return m_previousPosition;
}

// перенос окна при зажатой ЛКМ
void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_leftMouseButtonPressed = checkResizableField(event);
        setPreviousPosition(event->pos());
    }
    if(event->button() == Qt::RightButton){
        m_rightMouseButtonPressed = checkResizableField(event);
        setPreviousPosition(event->pos());
    }
    return QWidget::mousePressEvent(event);
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        m_leftMouseButtonPressed = None;

    return QWidget::mouseReleaseEvent(event);
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    switch(m_leftMouseButtonPressed){
        case Move:{
            int dx = event->x() - m_previousPosition.x();
            int dy = event->y() - m_previousPosition.y();
            setGeometry(x() + dx, y() + dy, width(), height());
            break;
        }
        case Spectr:{
            int dx = event->x() - m_previousPosition.x();
            sdr->set_central_freq(uint(sdr->sdr_params->central_freq + (-1) * dx * (sdr->sdr_params->sample_rate / 1024.)));
            setPreviousPosition(event->pos());
            break;
        }
        default:
            checkResizableField(event);
            break;
    }

    switch(m_rightMouseButtonPressed){
        case Move:{
            break;
        }
        case Spectr:{
            int dx = event->x() - m_previousPosition.x();
            rpu->first_tract.set_central_freq(uint(rpu->first_tract.get_central_freq() + (-1) * dx * (sdr->sdr_params->sample_rate / 1024.)));
            setPreviousPosition(event->pos());
            break;
        }
        default:
            checkResizableField(event);
            break;
    }

    return QWidget::mouseMoveEvent(event);
}

// определение области нажатия
MouseType Widget::checkResizableField(QMouseEvent *event)
{
    QPointF position = event->screenPos(); // Определяем позицию курсора на экране
    qreal x = this->x(); // координаты окна приложения, ...
    qreal y = this->y(); // ... то есть координату левого верхнего угла окна
    qreal width = this->width(); // А также ширину ...

    QRectF rectInterface(x, y, width, 24);
    QRectF rectSpectr(x + 4, y + 54, 1024, 711);

    if(rectInterface.contains(position))
        return Move;
    else
        if(rectSpectr.contains(position))
            return Spectr;
        else
            return None;
}
