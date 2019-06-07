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

#include "config_manager.h"
#include "ui_config_manager.h"

config_manager::config_manager(QWidget *parent, RPU *new_rpu, SDR *new_sdr, DSP *new_dsp)
  : QDialog{parent}, ui{new Ui::config_manager}, rpu{new_rpu}, sdr{new_sdr}, dsp{new_dsp}
{
    ui->setupUi(this);

    set_ui_style();
    on_ReloadButton_clicked();
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
    if(m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
}

// обновление интерфейса
void config_manager::update_interface()
{
    update_rpu_tab();
    update_sdr_tab();
    update_dsp_tab();
}

// обновление вкладки РПУ
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
    bool enabled_flag = (rpu->kalibrator.get_work_status() == OFF ? false : true);

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

    // частота дискретизации
    int idx;
    switch(sdr->sdr_params->sample_rate){
        case 240000:
            idx = 0;
            break;
        case 2496000:
            idx = 1;
            break;
    }
    // частота дискретизации
    ui->SdrSampleRateCB->blockSignals(true);
    ui->SdrSampleRateCB->setCurrentIndex(idx);
    ui->SdrSampleRateCB->blockSignals(false);

    // усиление
    ui->GainValBox->setCurrentIndex(sdr->sdr_params->gain_idx);
    ui->RtlAgcCB->setChecked(sdr->sdr_params->rtl_agc);
    ui->DirectSamplingCB->setCurrentIndex(sdr->sdr_params->direct_sampling_idx);

    // строка выбора приемника по номеру
    // если приемник еще не добавлен, но есть доступные приемники
    ui->CurrentDevNumLabel->setEnabled(!sdr->sdr_params->is_open && sdr->get_sdr_count());
    ui->CurrentDevSpinBox->setEnabled(!sdr->sdr_params->is_open && sdr->get_sdr_count());

    // кнопка выбора индекса используемого приемника также доступна, пока примник не подключен
    // и есть свободные приемники
    ui->ConnectButton->setEnabled(!sdr->sdr_params->is_open && sdr->get_sdr_count());
    ui->DisconnectButton->setEnabled(sdr->sdr_params->is_open);

    // обновление инфы о приемниках допускается только при отключенном приемнике
    ui->ReloadButton->setEnabled(!sdr->sdr_params->is_open);

    // частота дискретизации может меняться только при условии подключенного приемника и пока не идет запись
    ui->SampleRateLabel->setEnabled(sdr->sdr_params->is_open && !dsp->dsp_params->read_params.is_receiving);
    ui->SdrSampleRateCB->setEnabled(sdr->sdr_params->is_open && !dsp->dsp_params->read_params.is_receiving);

    // КУ может меняться с момента подключения приемника
    ui->GainLabel->setEnabled(sdr->sdr_params->is_open);
    ui->GainValBox->setEnabled(sdr->sdr_params->is_open);

    // режим оцифровки может меняться только при условии подключенного приемника и пока не идет запись
    ui->DirectSamplingLabel->setEnabled(sdr->sdr_params->is_open && !dsp->dsp_params->read_params.is_receiving);
    ui->DirectSamplingCB->setEnabled(sdr->sdr_params->is_open && !dsp->dsp_params->read_params.is_receiving);
}

// обновление вкладки "ЦОС"
void config_manager::update_dsp_tab()
{
    bool enabled_flag = !dsp->dsp_params->read_params.is_receiving;

    // значения
    ui->ReadOutPerSecSB->blockSignals(true);
    ui->ReadOutPerSecSB->setValue(dsp->dsp_params->read_params.readout_per_seconds);
    ui->ReadOutPerSecSB->blockSignals(false);

    ui->FltBwSB->setMaximum(sdr->sdr_params->sample_rate);
    ui->FltBwSB->setValue(dsp->dsp_params->flt_params.fltBw);
    ui->TransparencySB->setValue(dsp->dsp_params->fft_params.fft_fltBwAlpha);

    ui->ShiftFreqSB->setValue(dsp->dsp_params->shift_params.r_frec);
    ui->ShiftFreqSB->setMaximum(sdr->sdr_params->sample_rate / 2);
    ui->ShiftFreqSB->setMinimum((-1) * double(sdr->sdr_params->sample_rate / 2));

    ui->NullBinCircleCB->setChecked(dsp->dsp_params->fft_params.null_bin);
    ui->InversionCB->setChecked(dsp->dsp_params->fft_params.fft_inversion);
    ui->FftModeCB->setCurrentIndex(dsp->dsp_params->fft_params.fft_mode);
    ui->MaxAverageNumber->setText(QString::number(dsp->dsp_params->fft_params.fft_max_averages_number));
    ui->AverageNumSB->setMaximum((dsp->dsp_params->fft_params.fft_max_averages_number ? dsp->dsp_params->fft_params.fft_max_averages_number : 1));
    ui->AverageNumSB->setValue(dsp->dsp_params->fft_params.fft_averages_number);
    ui->DCOffsetGain->setValue(double(dsp->dsp_params->fft_params.dc_offset_gain));

    ui->WindowCB->setCurrentIndex(dsp->dsp_params->fft_params.fft_current_window);
    // выбор коэф-та доступен только для окна Блэкмана
    bool alpha_enabled = (dsp->dsp_params->fft_params.fft_current_window == BLACKMANN ? true : false);
    ui->WindowAlphaLabel->setEnabled(alpha_enabled);
    ui->WindowAlphaSB->setEnabled(alpha_enabled);

    ui->FreqGridCB->setCurrentIndex(dsp->dsp_params->fft_params.fft_frqGrdIdx);
    ui->FftStyleCB->setCurrentIndex(dsp->dsp_params->fft_params.fft_stlIdx);
    ui->FftSonoPaletteCB->setCurrentIndex(dsp->dsp_params->fft_params.fft_sonoPltIdx);

    // доступность изменения частоты дискретизации
    ui->ReadOutPerSecLabel->setEnabled(enabled_flag);
    ui->ReadOutPerSecSB->setEnabled(enabled_flag);

    // чек-боксы записи
    ui->ADCSignalCB->setChecked(dsp->dsp_params->read_params.write_to_file);
    ui->FltSignalCB->setChecked(dsp->dsp_params->flt_params.write_to_file);
    ui->ShiftSignalCB->setChecked(dsp->dsp_params->shift_params.write_to_file);
    ui->SoundSignalCB->setChecked(dsp->dsp_params->sound_params.write_to_file);

    if(dsp->dsp_params->read_params.write_to_file)
        ui->ReaderKbLabel->setText(QString::number(dsp->reader_rec->params->file.pos() / 1024) + " кБ");
    if(dsp->dsp_params->flt_params.write_to_file)
        ui->FltKbLabel->setText(QString::number(dsp->flt_rec->params->file.pos() / 1024) + " кБ");
    if(dsp->dsp_params->shift_params.write_to_file)
        ui->ShiftKbLabel->setText(QString::number(dsp->shift_rec->params->file.pos() / 1024) + " кБ");
    if(dsp->dsp_params->sound_params.write_to_file)
        ui->SoundKbLabel->setText(QString::number(dsp->sound_rec->params->file.pos() / 1024) + " кБ");

    // если флаг кратности == true, то шаг = (частота дискретизации) / (DSP_FFT_SIZE)
    // если флаг кратности == false, то шаг = 1
    if(dsp->dsp_params->shift_params.step)
        ui->ShiftFreqSB->setSingleStep(sdr->sdr_params->sample_rate / double(DSP_FFT_SIZE));
    else
        ui->ShiftFreqSB->setSingleStep(500);

    // демодуляция
    switch(dsp->dsp_params->demod_params.demod_mode){
        case AM:
            ui->AMDemodRB->setChecked(true);
            break;
        case USB:
            ui->USBDemodRB->setChecked(true);
            break;
        case LSB:
            ui->LSBDemodRB->setChecked(true);
            break;
        case FM:
            ui->FMDemodRB->setChecked(true);
            break;
        case BPSK:
            break;
        case QPSK:
            break;
    }

    // звуковая карта
    ui->ScaleFactorSB->setValue(dsp->dsp_params->sound_params.scale_facor);
}

// применение стиля
void config_manager::set_ui_style()
{
    this->setWindowFlags(Qt::FramelessWindowHint); // отключение оформления окна
    this->setAttribute(Qt::WA_TranslucentBackground); // фон главного виджета прозрачный
    this->setStyleSheet(StyleHelper::getWindowStyleSheet()); // стиль виджета

    // Установка стилей для всех элементов
    ui->WindowTitleLabel->setStyleSheet(StyleHelper::getLabelStyleSheet());
    ui->CloseButton->setStyleSheet(StyleHelper::getCloseStyleSheet());
    ui->CloseButton->setStyleSheet(StyleHelper::getCloseStyleSheet());
    ui->RPUMainTopLine->setStyleSheet(StyleHelper::getLineStyleSheet());
    ui->ConfigTab->setStyleSheet(StyleHelper::getTabWidgetStyleSheet());
    ui->RPUTabs->setStyleSheet(StyleHelper::getTabWidgetStyleSheet());

    // демодуляция
    ui->AMDemodRB->setStyleSheet(StyleHelper::getDemodRBStyleSheet());
    ui->USBDemodRB->setStyleSheet(StyleHelper::getDemodRBStyleSheet());
    ui->LSBDemodRB->setStyleSheet(StyleHelper::getDemodRBStyleSheet());
    ui->FMDemodRB->setStyleSheet(StyleHelper::getDemodRBStyleSheet());

    // запись
    ui->AdcPathButton->setStyleSheet(StyleHelper::getPathButtonStyleSheet());
    ui->FltPathButton->setStyleSheet(StyleHelper::getPathButtonStyleSheet());
    ui->ShiftPathButton->setStyleSheet(StyleHelper::getPathButtonStyleSheet());
    ui->SoundPathButton->setStyleSheet(StyleHelper::getPathButtonStyleSheet());
}

void config_manager::appendAskrTextBrowser(QString additionalText)
{
    ui->AskrResTextBrowser->setPlainText(additionalText);
    ui->AskrResTextBrowser->moveCursor(QTextCursor::End);
}

void config_manager::updateAskrProgressBar(int val)
{
    ui->AskrProgressBar->setValue(val);
}

/// --------------------------------РПУ----------------------------------------
// смена адреса LPT порта
void config_manager::on_LPTPortCB_currentIndexChanged(int new_lpt_port_idx)
{
    rpu->set_lpt_idx(new_lpt_port_idx);
}

// смена конфигурации трактов
void config_manager::on_TractConfigCB_currentIndexChanged(int new_rpu_config_idx)
{
    rpu->set_config(RPU_CONFIG(new_rpu_config_idx));
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
    emit global_update_interface();
}

// изменение выхода калибратора
void config_manager::on_KalibExitTypeBox_currentIndexChanged(int new_exit_type_idx)
{
    rpu->kalibrator.set_exit_type(KALIB_TYPE(new_exit_type_idx));
    emit global_update_interface();
}

// изменение частоты модуляции
void config_manager::on_KalibModulationFrequencyBox_valueChanged(int new_modulation_freq_in_kHz)
{
    rpu->kalibrator.set_modulation_freq(new_modulation_freq_in_kHz);
    emit global_update_interface();
}

// изменение типа модулирующего сигнала
void config_manager::on_KalibSignalTypeBox_currentIndexChanged(int new_signal_type_idx)
{
    rpu->kalibrator.set_signal_type(MOD_SIGNAL(new_signal_type_idx));
    emit global_update_interface();
}

// АСКР
void config_manager::on_AskrStartButton_clicked()
{
    emit startAskr();
}

void config_manager::on_Tract1_CentralFreqSB_valueChanged(int new_central_freq)
{
    rpu->first_tract.set_central_freq(new_central_freq * 1000);
    emit repaintFftGraph();
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


// обновление инфомрации о подключенных приемниках
void config_manager::on_ReloadButton_clicked()
{
    sdr->read_sdr_info();
    ui->FindedSdrTE->setText(sdr->sdr_params->sdrInfo);
    update_sdr_tab();
}

// очистка информационного поля
void config_manager::on_ClearButton_clicked()
{
    ui->FindedSdrTE->clear();
}

void config_manager::on_ConnectButton_clicked()
{
    // если приемник еще не занят
    if(!sdr->sdr_params->is_open || (sdr->sdr_params->is_open && sdr->sdr_params->is_already_open)){
        int result = rtlsdr_open(&(sdr->sdr_params->sdr_ptr), uint(ui->CurrentDevSpinBox->value()));

        if(!result || (result && sdr->sdr_params->is_already_open)){
            sdr->sdr_params->index = ui->CurrentDevSpinBox->value();
            sdr->sdr_params->is_open = true;
        }else{
            sdr->sdr_params->is_open = false;
        }
    }else{
        sdr->sdr_params->is_open = false;
    }

    emit global_update_interface();
}

// отключение от приемника
void config_manager::on_DisconnectButton_clicked()
{
    if(sdr->sdr_params->is_open){
        if(!dsp->dsp_params->read_params.is_receiving){
            int result = rtlsdr_close(sdr->sdr_params->sdr_ptr);

            if(!result){
                sdr->sdr_params->is_open = false;
            }else{
                sdr->sdr_params->is_open = true;
            }
            emit global_update_interface();
        }else{
            QMessageBox(QMessageBox::Information, "Ошибка", "Для отключения СПО от приемника необходимо остановить работу СПО.").exec();
        }
    }
}

// изменение частоты дисркетизации
void config_manager::on_SdrSampleRateCB_currentIndexChanged(int new_sample_rate_idx)
{
    sdr->set_sample_rate(sample_rates[new_sample_rate_idx]);
    ui->FltBwSB->setMaximum(sdr->sdr_params->sample_rate);

    dsp->dsp_params->flt_params.fltRFrec = dsp->dsp_params->flt_params.fltBw / double(2 * sdr->sdr_params->sample_rate);
    dsp->dsp_params->flt_params.recalc_flt_coeffs();

    emit global_update_interface();
    emit repaintFftGraph();
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

// управление усиление на чипе realtek
void config_manager::on_RtlAgcCB_clicked(bool new_agc_state)
{
    sdr->set_rtl_agc(new_agc_state);
    emit global_update_interface();
}
/// --------------------------------DSP----------------------------------------

// выбор окна
void config_manager::on_WindowCB_currentIndexChanged(int new_win_idx)
{
    dsp->dsp_params->fft_params.fft_current_window = WINDOW(new_win_idx);
    emit global_update_interface();
}

// изменение коэффициента alpha
void config_manager::on_WindowAlphaSB_valueChanged(double new_aplha)
{
    dsp->dsp_params->fft_params.fft_win_alpha = float(new_aplha);
}

void config_manager::on_AverageNumSB_valueChanged(int new_fft_averages_number)
{
    dsp->dsp_params->fft_params.fft_averages_number = new_fft_averages_number;
    emit global_update_interface();
}

void config_manager::on_FftModeCB_currentIndexChanged(int new_fft_mode)
{
    dsp->dsp_params->fft_params.fft_mode = FFT_MODE(new_fft_mode);
}

void config_manager::on_InversionCB_clicked(bool new_fft_inversion)
{
    dsp->dsp_params->fft_params.fft_inversion = new_fft_inversion;
    emit repaintFftGraph();
    emit repaintSonoGraph();
}

void config_manager::on_ShiftFreqSB_valueChanged(double new_r_frec)
{
    dsp->dsp_params->shift_params.r_frec = new_r_frec;
}

void config_manager::on_DCOffsetGain_valueChanged(double new_dc_gain)
{
    dsp->dsp_params->fft_params.dc_offset_gain = float(new_dc_gain);
}

void config_manager::on_NullBinCircleCB_clicked(bool new_state)
{
    dsp->dsp_params->fft_params.null_bin = new_state;
}

void config_manager::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_leftMouseButtonPressed = checkResizableField(event);
        setPreviousPosition(event->pos());
    }
    return QWidget::mousePressEvent(event);
}

void config_manager::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        m_leftMouseButtonPressed = None;
    }
    return QWidget::mouseReleaseEvent(event);
}

void config_manager::mouseMoveEvent(QMouseEvent *event)
{
    switch(m_leftMouseButtonPressed) {
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
void config_manager::on_ADCSignalCB_clicked(bool new_state)
{
    dsp->dsp_params->read_params.write_to_file = new_state;

    // создание файла
    if(new_state){
        dsp->reader_rec->prepair_memory(dsp->dsp_params->read_params.rb.cell_size);
        dsp->create_file_names();
        dsp->reader_rec->open_file();
    }else{
        dsp->reader_rec->close_file(sdr->sdr_params->sample_rate, 2);
    }

    emit global_update_interface();
}

// выбор выходной папки
void config_manager::on_AdcPathButton_clicked()
{
    QString res = QFileDialog::getExistingDirectory(this, "Директория записи сигнала с АЦП", dsp->reader_rec->params->path) + "/";
    if(res != "/")
        dsp->reader_rec->params->path = res;
}

// флаг записи во второй файл
void config_manager::on_FltSignalCB_clicked(bool new_state)
{
    dsp->dsp_params->flt_params.write_to_file = new_state;

    // создание файла
    if(new_state){
        dsp->flt_rec->prepair_memory(dsp->dsp_params->flt_params.rb.cell_size);
        dsp->create_file_names();
        dsp->flt_rec->open_file();
    }else{
        dsp->flt_rec->close_file(sdr->sdr_params->sample_rate, 2);
    }

    emit global_update_interface();
}

// выбор выходной папки
void config_manager::on_FltPathButton_clicked()
{
    QString res = QFileDialog::getExistingDirectory(this, "Директория записи отфильтрованного сигнала", dsp->flt_rec->params->path) + "/";
    if(res != "/")
        dsp->flt_rec->params->path = res;

}

// флаг записи в третий файл
void config_manager::on_ShiftSignalCB_clicked(bool new_state)
{
    dsp->dsp_params->shift_params.write_to_file = new_state;

    // создание файла
    if(new_state){
        dsp->shift_rec->prepair_memory(dsp->dsp_params->shift_params.rb.cell_size);
        dsp->create_file_names();
        dsp->shift_rec->open_file();
    }else{
        dsp->shift_rec->close_file(sdr->sdr_params->sample_rate, 2);
    }

    emit global_update_interface();
}

// выбор выходной папки
void config_manager::on_ShiftPathButton_clicked()
{
    QString res = dsp->shift_rec->params->path = QFileDialog::getExistingDirectory(this, "Директория записи перенесенного сигнала", dsp->shift_rec->params->path) + "/";
    if(res != "/")
        dsp->shift_rec->params->path = res;
}

// флаг записи в звуковой файл
void config_manager::on_SoundSignalCB_clicked(bool new_state)
{
    dsp->dsp_params->sound_params.write_to_file = new_state;

    // создание файла
    if(new_state){
        dsp->sound_rec->prepair_memory(dsp->dsp_params->demod_params.rb.cell_size);
        dsp->create_file_names();
        dsp->sound_rec->open_file();
    }else{
        dsp->sound_rec->close_file(DSP_SOUND_SAMPLE_RATE, 1);
    }

    emit global_update_interface();
}

// выбор выходной папки
void config_manager::on_SoundPathButton_clicked()
{
    QString res = QFileDialog::getExistingDirectory(this, "Директория записи аудио сигнала", dsp->sound_rec->params->path) + "/";
    if(res != "/")
        dsp->sound_rec->params->path = res;
}

// изменение полосы фильтрации
void config_manager::on_FltBwSB_valueChanged(int _fltBw)
{
    dsp->dsp_params->flt_params.fltBw    = _fltBw;
    dsp->dsp_params->flt_params.fltRFrec = dsp->dsp_params->flt_params.fltBw / double(2 * sdr->sdr_params->sample_rate);
    dsp->dsp_params->flt_params.recalc_flt_coeffs();

    emit global_update_interface();
    emit repaintFftGraph();
}

// изменение кол-ва усреднений в секунду
void config_manager::on_ReadOutPerSecSB_valueChanged(int new_readout_per_sec)
{
    bool is_receiving = dsp->dsp_params->read_params.is_receiving;
    if(is_receiving)
        emit stop_receiving();

    dsp->dsp_params->read_params.readout_per_seconds = new_readout_per_sec;

    if(is_receiving)
        emit start_receiving();
    else
        dsp->prepair_to_record();
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
    this->setVisible(false);
}

void config_manager::update_reader_rec_label()
{
    ui->ReaderKbLabel->setText(dsp->reader_rec->size_to_str());
}

void config_manager::update_flt_rec_label()
{
    ui->FltKbLabel->setText(dsp->flt_rec->size_to_str());
}

void config_manager::update_shift_rec_label()
{
    ui->ShiftKbLabel->setText(dsp->shift_rec->size_to_str());
}

void config_manager::update_sound_rec_label()
{
    ui->SoundKbLabel->setText(dsp->sound_rec->size_to_str());
}

void config_manager::on_AMDemodRB_clicked()
{
    dsp->dsp_params->demod_params.demod_mode = AM;
}

void config_manager::on_USBDemodRB_clicked()
{
    dsp->dsp_params->demod_params.demod_mode = USB;
}

void config_manager::on_LSBDemodRB_clicked()
{
    dsp->dsp_params->demod_params.demod_mode = LSB;
}

void config_manager::on_FMDemodRB_clicked()
{
    dsp->dsp_params->demod_params.demod_mode = FM;
}

void config_manager::on_ScaleFactorSB_valueChanged(int _scFct)
{
    dsp->dsp_params->sound_params.scale_facor = _scFct;
}

void config_manager::on_FreqGridCB_currentIndexChanged(int _freq_grid)
{
    dsp->dsp_params->fft_params.fft_frqGrdIdx = _freq_grid;
    emit repaintFftGraph();
}

void config_manager::on_FftStyleCB_currentIndexChanged(int _style_idx)
{
    dsp->dsp_params->fft_params.fft_stlIdx = _style_idx;
    emit repaintFftGraph();
}

void config_manager::on_FftSonoPaletteCB_currentIndexChanged(int _palette_idx)
{
    dsp->dsp_params->fft_params.fft_sonoPltIdx = _palette_idx;
    emit repaintSonoGraph();
}

void config_manager::on_TransparencySB_valueChanged(double _fltBwAlpha)
{
    dsp->dsp_params->fft_params.fft_fltBwAlpha = _fltBwAlpha;
    emit repaintFftGraph();
}
