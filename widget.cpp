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

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget), settings{"K722", "DigitalKalmar"}
{
    ui->setupUi(this);
    this->setMouseTracking(true);

    rpu = new RPU();
    sdr = new SDR();
    dsp = new DSP(sdr);
    config_manager_form = new config_manager(parent, rpu, sdr, dsp);
    askr = new ASKR(rpu, sdr, dsp);
    askrThread.start();
    askr->moveToThread(&askrThread);
    fft_graph = ui->FftGraph;
    sono_graph = ui->SonoGraph;

    set_ui_style();
    bind_slots_signals();
    restore_settings();

    fft_graph->paintStep(dsp->dsp_params->fft_params.fft_res);
    sono_graph->paintStep(dsp->dsp_params->fft_params.fft_res);
}

Widget::~Widget()
{
    askrThread.quit();
    askrThread.wait();

    delete rpu;
    delete sdr;
    delete dsp;
    delete config_manager_form;

    delete fft_graph;
    delete sono_graph;

    delete ui;
}

void Widget::setPreviousPosition(QPoint previousPosition)
{
    if(m_previousPosition == previousPosition)
        return;
    m_previousPosition = previousPosition;
    emit previousPositionChanged(previousPosition);
}

void Widget::global_update_interface()
{
    config_manager_form->update_interface();

// кнопка записи и кнопка останова
    if(sdr->sdr_params->is_open){

        bool is_receiving = dsp->dsp_params->read_params.is_receiving;

        ui->RecButton->setEnabled(!is_receiving);
        ui->RecButton->setStyleSheet(StyleHelper::getRecStyleSheet(!is_receiving));

        ui->StopButton->setEnabled(is_receiving);
        ui->StopButton->setStyleSheet(StyleHelper::getStopStyleSheet(is_receiving));

    }else{
        ui->RecButton->setEnabled(false);
        ui->RecButton->setStyleSheet(StyleHelper::getRecStyleSheet(false));

        ui->StopButton->setEnabled(false);
        ui->StopButton->setStyleSheet(StyleHelper::getStopStyleSheet(false));
    }

// слайдеры
    ui->DynamicRangeHS->setValue(int(dsp->dsp_params->fft_params.fft_dynamic_range));
    ui->NoiseLevelHS->setValue(int(dsp->dsp_params->fft_params.fft_noise_level));
    ui->SoundButton->setStyleSheet(StyleHelper::getSoundButton(dsp->dsp_params->sound_params.is_mute));

    ui->SoundVolumeHS->setValue(dsp->dsp_params->sound_params.sound_volume);
    ui->SoundVolumeHS->setEnabled(!dsp->dsp_params->sound_params.is_mute);
    ui->SoundVolumeHS->setStyleSheet(StyleHelper::getSoundVolumeHS(!dsp->dsp_params->sound_params.is_mute));

// подписи громкости звука, динамического диапазона и уровня шума
    ui->SoundLabel->setText("Громкость ( " + (dsp->dsp_params->sound_params.is_mute ? "0 % )" : QString::number(ui->SoundVolumeHS->value()) + " % )"));
    ui->DynRangeLabel->setText("Динамический диапазон ( " + QString::number(int(dsp->dsp_params->fft_params.fft_dynamic_range)) + " дБ )");
    ui->NoiseLevelLabel->setText("Смещение ( " + QString::number(int(dsp->dsp_params->fft_params.fft_noise_level)) + " дБ )");

// частота настройки
    ui->CurrentFreqSB->blockSignals(true);
    ui->CurrentFreqSB->setValue(sdr->sdr_params->central_freq);
    ui->CurrentFreqSB->blockSignals(false);
}

// управление стилем окна
void Widget::set_ui_style()
{
    this->setWindowFlags(Qt::FramelessWindowHint);    // отключение оформления окна
    this->setAttribute(Qt::WA_TranslucentBackground); // фон главного виджета прозрачным

    this->setStyleSheet(StyleHelper::getWindowStyleSheet());    // стиль виджета
    ui->WindowTitleLabel->setStyleSheet(StyleHelper::getLabelStyleSheet()); // заголовок

    ui->RecButton->setStyleSheet(StyleHelper::getRecStyleSheet(true));
    ui->StopButton->setStyleSheet(StyleHelper::getStopStyleSheet(true));
    ui->ConfigButton->setStyleSheet(StyleHelper::getConfigStyleSheet());
    ui->SoundVolumeHS->setStyleSheet(StyleHelper::getSoundVolumeHS(dsp->dsp_params->sound_params.is_mute));
    ui->DynamicRangeHS->setStyleSheet(StyleHelper::getHSStyleSheet());
    ui->NoiseLevelHS->setStyleSheet(StyleHelper::getHSStyleSheet());

    ui->MinimizeButton->setStyleSheet(StyleHelper::getMinimizeStyleSheet());
    ui->CloseButton->setStyleSheet(StyleHelper::getCloseStyleSheet());

    config_manager_form->set_ui_style();
}

// сигнально-слотовые соединения между объектами
void Widget::bind_slots_signals()
{
    connect(config_manager_form, &config_manager::startAskr,               askr,                &ASKR::testRpu);

    connect(askr,                &ASKR::updateAskrResTextBrowser,          config_manager_form, &config_manager::appendAskrTextBrowser);
    connect(askr,                &ASKR::updateAskrProgressBar,             config_manager_form, &config_manager::updateAskrProgressBar);
    connect(askr,                &ASKR::globalUpdateInterface,             this,                &Widget::global_update_interface);
    connect(&askrThread,         &QThread::finished,                       askr,                &QObject::deleteLater);

    connect(config_manager_form, &config_manager::global_update_interface, this,                &Widget::global_update_interface);
    connect(config_manager_form, &config_manager::repaintFftGraph,         ui->FftGraph,        &FFT_GRAPH::paintScreen);
    connect(config_manager_form, &config_manager::repaintSonoGraph,        ui->SonoGraph,       &SONO_GRAPH::changePalette);
    connect(sdr,                 &SDR::recalc_dsp_params,                  dsp,                 &DSP::recalc_dsp_params);
    connect(dsp,                 &DSP::global_update_interface,            this,                &Widget::global_update_interface);
    connect(ui->MinimizeButton,  &QToolButton::clicked,                    this,                &Widget::showMinimized);
    connect(&rpu->first_tract,   &RPU_tract::global_update_interface,      this,                &Widget::global_update_interface);

    // отрисовка спектра
    connect(this,                &Widget::start_reading,                   dsp->reader,         &READER::start_reading);
    connect(this,                &Widget::repaintFftGraph,                 ui->FftGraph,        &FFT_GRAPH::paintScreen);
    connect(dsp->fft,            &FFT_CALCER::paint_fft,                   ui->FftGraph,        &FFT_GRAPH::paintStep);
    connect(dsp->fft,            &FFT_CALCER::paint_fft,                   ui->SonoGraph,       &SONO_GRAPH::paintStep);
    connect(this,                &Widget::repaintSonoGraph,                ui->SonoGraph,       &SONO_GRAPH::changePalette);

    // старт-стоп
    connect(config_manager_form, &config_manager::stop_receiving,          this,                &Widget::on_StopButton_clicked);
    connect(config_manager_form, &config_manager::start_receiving,         this,                &Widget::on_RecButton_clicked);

    // размеры записываемых файлов
    connect(dsp->reader_rec,     &WAV_RECORDER::update_size_label,         config_manager_form, &config_manager::update_reader_rec_label);
    connect(dsp->flt_rec,        &WAV_RECORDER::update_size_label,         config_manager_form, &config_manager::update_flt_rec_label);
    connect(dsp->shift_rec,      &WAV_RECORDER::update_size_label,         config_manager_form, &config_manager::update_shift_rec_label);
    connect(dsp->sound_rec,      &WAV_RECORDER::update_size_label,         config_manager_form, &config_manager::update_sound_rec_label);

    // от спектрограммы;
    connect(ui->FftGraph,        &FFT_GRAPH::changeSdrFreq,                sdr,                 &SDR::set_central_freq);
    connect(ui->FftGraph,        &FFT_GRAPH::changeRpuFreq,                &rpu->first_tract,   &RPU_tract::set_central_freq);

    // от главной формы
    connect(ui->DynamicRangeHS, &QSlider::valueChanged,                    dsp,                 &DSP::setDynRng);

    // от sdr
    connect(sdr,                &SDR::update_interface,                    this,                &Widget::global_update_interface);

    // передача в fft_graph валидных указателей
    fft_graph->connectParams(&dsp->dsp_params->fft_params.fft_dynamic_range,
                             &dsp->dsp_params->fft_params.fft_noise_level,
                             &dsp->dsp_params->fft_params.null_bin,
                             &dsp->dsp_params->flt_params.fltRFrec,
                             &dsp->dsp_params->shift_params.r_frec,
                             &sdr->sdr_params->central_freq,
                             &rpu->first_tract.central_freq,
                             &sdr->sdr_params->sample_rate,
                             &dsp->dsp_params->fft_params.fft_stlIdx,
                             &dsp->dsp_params->fft_params.fft_frqGrdIdx,
                             &dsp->dsp_params->fft_params.fft_fltBwAlpha,
                             &dsp->dsp_params->fft_params.fft_inversion);

    // передача в sono_graph валидных указателей
    sono_graph->connectParams(&dsp->dsp_params->fft_params.fft_dynamic_range,
                              &dsp->dsp_params->fft_params.fft_noise_level,
                              &dsp->dsp_params->read_params.readout_per_seconds,
                              &dsp->dsp_params->fft_params.fft_sonoPltIdx);
}

void Widget::startAskr()
{
    askr->testRpu();
}

void Widget::on_ConfigButton_clicked()
{
    bool new_visible = !config_manager_form->isVisible();
    config_manager_form->setVisible(new_visible);
    config_manager_form->is_visible = new_visible;
}

void Widget::on_RecButton_clicked()
{
    if(dsp->prepair_to_record()){
        QMessageBox::critical(this, "Ошибка запуска", "Не удалось начать запись сигнала.\nЗаново подключите SDR приемник и перезапустите программу.");
        sdr->sdr_params->is_open = false;
    }else{
        sdr->sdr_params->is_open = true;
        dsp->dsp_params->read_params.is_receiving = true;

        emit start_reading();
    }

    global_update_interface();
}

void Widget::on_StopButton_clicked()
{
    dsp->dsp_params->read_params.is_receiving = false;

    dsp->close_files();

    global_update_interface();
}

void Widget::on_CloseButton_clicked()
{
    save_settings();
    if(dsp->dsp_params->read_params.is_receiving) on_StopButton_clicked();
    close();
}


// изменение громкости воспроизводимого звука
void Widget::on_SoundVolumeHS_valueChanged(int new_sound_volume)
{
    dsp->dsp_params->sound_params.sound_volume = new_sound_volume;
    if(dsp->dsp_params->sound_params.audio_out)
        dsp->dsp_params->sound_params.audio_out->setVolume(dsp->dsp_params->sound_params.sound_volume / 100.);

    global_update_interface();
}

// приглушение
void Widget::on_SoundButton_clicked()
{
    dsp->dsp_params->sound_params.is_mute = !dsp->dsp_params->sound_params.is_mute;

    if(dsp->dsp_params->sound_params.audio_out){
        if(dsp->dsp_params->sound_params.is_mute)
            dsp->dsp_params->sound_params.audio_out->setVolume(0);
        else
            dsp->dsp_params->sound_params.audio_out->setVolume(dsp->dsp_params->sound_params.sound_volume / 100.);
    }

    global_update_interface();
}

void Widget::on_DynamicRangeHS_valueChanged(int new_dnRng)
{
    dsp->dsp_params->fft_params.fft_dynamic_range = float(new_dnRng);
    global_update_interface();
    emit repaintFftGraph();
    emit repaintSonoGraph();
}

void Widget::on_NoiseLevelHS_valueChanged(int new_noiseLvl)
{
    dsp->dsp_params->fft_params.fft_noise_level = float(new_noiseLvl);
    global_update_interface();
    emit repaintFftGraph();
    emit repaintSonoGraph();
}


void Widget::save_settings()
{
/// формы
    // положения форм
    settings.setValue("MainFormGeometry",   saveGeometry());
    settings.setValue("ConfigFormGeometry", config_manager_form->saveGeometry());
    settings.setValue("ConfigFormVisible",  config_manager_form->isVisible());

    // высота и ширина спектрограммы и сонограммы
    settings.setValue("GraphsSplitter",     ui->GraphsSlpitter->saveState());

/// РПУ
    settings.beginGroup("/RPU");

    settings.setValue("lpt_idx",            rpu->get_lpt_idx());
    settings.setValue("config_idx",         rpu->get_config_idx());

    // калибратор
    settings.beginGroup("/kalibrator");
    settings.setValue("signal_type",        int(rpu->kalibrator.get_signal_type()));
    settings.setValue("modulation_freq",    rpu->kalibrator.get_modulation_freq() / 1000);
    settings.setValue("att_idx",            rpu->kalibrator.get_att_idx());
    settings.setValue("exit_type",          int(rpu->kalibrator.get_exit_type()));
    settings.setValue("work_status",        int(rpu->kalibrator.get_work_status()));
    settings.endGroup();

    // тракт 1
    settings.beginGroup("/tract1");
    settings.setValue("central_freq",       rpu->first_tract.get_central_freq());
    settings.setValue("in_att_idx",         int(rpu->first_tract.get_in_att_idx()));
    settings.setValue("hf_att_idx",         rpu->first_tract.get_hf_att_idx());
    settings.setValue("if_att_idx",         rpu->first_tract.get_if_att_idx());
    settings.setValue("if_band_idx",        int(rpu->first_tract.get_if_band_idx()));
    settings.endGroup();

    settings.endGroup();

/// SDR
    settings.beginGroup("/SDR");
    settings.setValue("index",              sdr->sdr_params->index);
    settings.setValue("central_freq",       sdr->sdr_params->central_freq);
    settings.setValue("gain_idx",           sdr->sdr_params->gain_idx);
    settings.setValue("rtl_agc",            sdr->sdr_params->rtl_agc);
    settings.setValue("sample_rate",        sdr->sdr_params->sample_rate);
    settings.setValue("direct_sampling_idx",sdr->sdr_params->direct_sampling_idx);
    settings.endGroup();

/// DSP
    settings.beginGroup("/DSP");

    settings.beginGroup("/read");
    settings.setValue("is_receiving",       dsp->dsp_params->read_params.is_receiving);
    settings.setValue("readout_per_second", dsp->dsp_params->read_params.readout_per_seconds);
    settings.endGroup();

    settings.beginGroup("/flt");
    settings.setValue("fltBw",              dsp->dsp_params->flt_params.fltBw);
    settings.endGroup();

    settings.beginGroup("/shift");
    settings.setValue("r_frec",             dsp->dsp_params->shift_params.r_frec);
    settings.setValue("step",               dsp->dsp_params->shift_params.step);
    settings.endGroup();

    settings.beginGroup("/fft");
    settings.setValue("fft_dynamic_range",  dsp->dsp_params->fft_params.fft_dynamic_range);
    settings.setValue("fft_noise_level",    dsp->dsp_params->fft_params.fft_noise_level);
    settings.setValue("null_bin",           dsp->dsp_params->fft_params.null_bin);
    settings.setValue("dc_offset_gain",     dsp->dsp_params->fft_params.dc_offset_gain);
    settings.setValue("mode",               dsp->dsp_params->fft_params.fft_mode);
    settings.setValue("averages_number",    dsp->dsp_params->fft_params.fft_averages_number);
    settings.setValue("inversion",          dsp->dsp_params->fft_params.fft_inversion);
    settings.setValue("current_window",     dsp->dsp_params->fft_params.fft_current_window);
    settings.setValue("win_alpha",          dsp->dsp_params->fft_params.fft_win_alpha);
    settings.setValue("win_alpha",          dsp->dsp_params->fft_params.fft_win_alpha);
    settings.setValue("fftStl",             dsp->dsp_params->fft_params.fft_stlIdx);
    settings.setValue("frqGrdIdx",          dsp->dsp_params->fft_params.fft_frqGrdIdx);
    settings.setValue("sonoPaletteIdx",     dsp->dsp_params->fft_params.fft_sonoPltIdx);
    settings.setValue("fltBwAlpha",         dsp->dsp_params->fft_params.fft_fltBwAlpha);
    settings.endGroup();

    settings.beginGroup("/wav_rec");
    settings.setValue("adc_path",           dsp->reader_rec->params->path);
    settings.setValue("flt_path",           dsp->flt_rec->params->path);
    settings.setValue("shift_path",         dsp->shift_rec->params->path);
    settings.setValue("sound_path",         dsp->sound_rec->params->path);
    settings.endGroup();

    settings.beginGroup("/demod");
    settings.setValue("mode",               dsp->dsp_params->demod_params.demod_mode);
    settings.endGroup();

    settings.beginGroup("/sound");
    settings.setValue("volume",             dsp->dsp_params->sound_params.sound_volume);
    settings.setValue("mute",               dsp->dsp_params->sound_params.is_mute);
    settings.setValue("scale_facor",        dsp->dsp_params->sound_params.scale_facor);
    settings.endGroup();

    settings.sync();
}

void Widget::restore_settings()
{
/// виджеты
    // положение главного окна
    restoreGeometry(settings.value("MainGeometry").toByteArray());
    config_manager_form->restoreGeometry(settings.value("ConfigFormGeometry").toByteArray());
    config_manager_form->setVisible(settings.value("ConfigFormVisible", false).toBool());

    // сплиттер
    ui->GraphsSlpitter->restoreState(settings.value("GraphsSplitter").toByteArray());

/// РПУ
    settings.beginGroup("/RPU");

    rpu->set_lpt_idx(settings.value("lpt_idx", 0).toInt());
    rpu->set_config(RPU_CONFIG(settings.value("config_idx", RPU_DEFAULT_CONFIG).toInt()));

    // калибратор
    settings.beginGroup("/kalibrator");
    rpu->kalibrator.set_signal_type( MOD_SIGNAL(settings.value("signal_type",     RPU_DEFAULT_KALIB_SIGNAL_TYPE).toInt()));
    rpu->kalibrator.set_modulation_freq( ushort(settings.value("modulation_freq", RPU_DEFAULT_KALIB_MOD_FREQ).toInt()));
    rpu->kalibrator.set_att_idx(         ushort(settings.value("att_idx",         RPU_DEFAULT_KALIB_ATT_IDX).toInt()));
    rpu->kalibrator.set_exit_type(   KALIB_TYPE(settings.value("exit_type",       RPU_DEFAULT_KALIB_EXIT).toInt()));
    rpu->kalibrator.set_work_status(     STATUS(settings.value("work_status",     RPU_DEFAULT_KALIB_STATUS).toInt()));
    settings.endGroup();

    // тракт 1
    settings.beginGroup("/tract1");
    rpu->first_tract.set_central_freq(     settings.value("central_freq", RPU_DEFAULT_TRACT_FREQ).toInt());
    rpu->first_tract.set_in_att_idx( short(settings.value("in_att_idx",   RPU_DEFAULT_IN_ATT_IDX).toBool()));
    rpu->first_tract.set_hf_att_idx( short(settings.value("hf_att_idx",   RPU_DEFAULT_HF_ATT_IDX).toInt()));
    rpu->first_tract.set_if_att_idx( short(settings.value("if_att_idx",   RPU_DEFAULT_IF_ATT_IDX).toInt()));
    rpu->first_tract.set_if_band_idx(short(settings.value("if_band_idx",  RPU_DEFAULT_IF_BAND_IDX).toInt()));
    settings.endGroup();

    settings.endGroup();

/// SDR
    settings.beginGroup("/SDR");
    sdr->sdr_params->central_freq         = settings.value("central_freq",        SDR_DEFAULT_CENTRAL_FREQ).toInt();
    sdr->sdr_params->gain_idx             = settings.value("gain_idx",            SDR_DEFAULT_GAIN_IDX).toInt();
    sdr->sdr_params->rtl_agc              = settings.value("rtl_agc",             SDR_DEFAULT_RTL_AGC).toBool();
    sdr->sdr_params->sample_rate          = settings.value("sample_rate",         SDR_DEFAULT_SAMPLE_RATE).toInt();
    sdr->sdr_params->direct_sampling_idx  = settings.value("direct_sampling_idx", SDR_DEFAULT_DIRECT_SAMPLING_IDX).toInt();
    sdr->sdr_params->index                = settings.value("index",               SDR_DEFAULT_INDEX).toInt();
    settings.endGroup();

    // коннект к железу
    if(sdr->sdr_params->index != -1)
        sdr->sdr_params->is_open = !bool(rtlsdr_open(&(sdr->sdr_params->sdr_ptr), uint(sdr->sdr_params->index)));
    else
        sdr->sdr_params->is_open = false;

/// DSP
    settings.beginGroup("/DSP");

    settings.beginGroup("/read");
    bool is_receiving                                  = settings.value("is_receiving",       false).toBool();
    dsp->dsp_params->read_params.readout_per_seconds   = settings.value("readout_per_second", DSP_READOUT_PER_SECONDS).toInt();
    settings.endGroup();

    settings.beginGroup("/flt");
    dsp->dsp_params->flt_params.fltBw                  = settings.value("fltBw", DSP_FLT_DEFAULT_BW).toInt();
    dsp->dsp_params->flt_params.fltRFrec               = dsp->dsp_params->flt_params.fltBw / (2 * sdr->sdr_params->sample_rate);
    dsp->dsp_params->flt_params.recalc_flt_coeffs();
    settings.endGroup();

    settings.beginGroup("/shift");
    dsp->dsp_params->shift_params.r_frec               = settings.value("r_frec", 0.).toDouble();
    dsp->dsp_params->shift_params.step                 = settings.value("step",   false).toBool();
    settings.endGroup();

    settings.beginGroup("/fft");
    dsp->dsp_params->fft_params.fft_dynamic_range      =          settings.value("fft_dynamic_range", DSP_FFT_DEFAULT_DYNAMIC_RANGE).toFloat();
    dsp->dsp_params->fft_params.fft_noise_level        =          settings.value("fft_noise_level", DSP_FFT_DEFAULT_NOISE_LEVEL).toFloat();
    dsp->dsp_params->fft_params.null_bin               =          settings.value("null_bin", false).toBool();
    dsp->dsp_params->fft_params.dc_offset_gain         =          settings.value("dc_offset_gain",  0).toFloat();
    dsp->dsp_params->fft_params.fft_mode               = FFT_MODE(settings.value("mode",            DSP_FFT_DEFAULT_MODE).toInt());
    dsp->dsp_params->fft_params.fft_averages_number    =          settings.value("averages_number", DSP_FFT_DEFAULT_AVERAGE_NUMBER).toInt();
    dsp->dsp_params->fft_params.fft_inversion          =          settings.value("inversion",       DSP_FFT_DEFAULT_INVERSION).toBool();
    dsp->dsp_params->fft_params.fft_current_window     =   WINDOW(settings.value("current_window",  DSP_DEFAULT_FFT_WINDOW).toInt());
    dsp->dsp_params->fft_params.fft_win_alpha          =          settings.value("win_alpha",       DSP_DEFAULT_WIN_ALPHA).toFloat();
    dsp->dsp_params->fft_params.fft_stlIdx             =          settings.value("fftStl",          DSP_DEFAULT_FFT_STYLE_IDX).toInt();
    dsp->dsp_params->fft_params.fft_frqGrdIdx          =          settings.value("frqGrdIdx",       DSP_DEFAULT_FFT_GRID_IDX).toInt();
    dsp->dsp_params->fft_params.fft_sonoPltIdx         =          settings.value("sonoPaletteIdx",  DSP_DEFAULT_FFT_SONO_PALETTE_IDX).toInt();
    dsp->dsp_params->fft_params.fft_fltBwAlpha         =          settings.value("fltBwAlpha",      DSP_DEFAULT_FFT_FLT_ALPHA).toDouble();
    settings.endGroup();

    settings.beginGroup("/wav_rec");
    dsp->reader_rec->params->path                      = settings.value("adc_path").toString();
    dsp->flt_rec->params->path                         = settings.value("flt_path").toString();
    dsp->shift_rec->params->path                       = settings.value("shift_path").toString();
    dsp->sound_rec->params->path                       = settings.value("sound_path").toString();
    settings.endGroup();

    settings.beginGroup("/demod");
    dsp->dsp_params->demod_params.demod_mode           = DEMOD_MODE(settings.value("mode", DSP_DEMOD_DEFAULT_MODE).toInt());
    settings.endGroup();

    settings.beginGroup("/sound");
    dsp->dsp_params->sound_params.sound_volume         = settings.value("volume", DSP_SOUND_DEFAULT_VOLUME).toInt();
    dsp->dsp_params->sound_params.is_mute              = settings.value("mute", false).toBool();
    dsp->dsp_params->sound_params.scale_facor          = settings.value("scale_facor", 1).toInt();
    settings.endGroup();

    settings.endGroup();

    if(dsp->dsp_params->sound_params.audio_out){
        dsp->dsp_params->sound_params.audio_out->setVolume(dsp->dsp_params->sound_params.sound_volume / 100.);

        if(dsp->dsp_params->sound_params.is_mute) dsp->dsp_params->sound_params.audio_out->setVolume(0);
        else dsp->dsp_params->sound_params.audio_out->setVolume(dsp->dsp_params->sound_params.sound_volume / 100.);
    }

    dsp->recalc_dsp_params();
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
        case Move:
        {
            int dx = event->x() - m_previousPosition.x();
            int dy = event->y() - m_previousPosition.y();
            setGeometry(x() + dx, y() + dy, width(), height());
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
        default:
            checkResizableField(event);
            break;
    }

    return QWidget::mouseMoveEvent(event);
}

// определение области нажатия
MouseType Widget::checkResizableField(QMouseEvent *event)
{
    QPointF position = event->localPos();
    QRectF rectInterface(0, 0, this->width(), UI_TOP_PANEL_HEIGHT_PX);

    if(rectInterface.contains(position))
        return Move;
    return None;
}

void Widget::on_CurrentFreqSB_valueChanged(int newFreq)
{
    sdr->set_central_freq(newFreq);
    emit global_update_interface();
}
