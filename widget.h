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

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include <QSplashScreen>
#include <QSplitter>
#include <QStyle>
#include <QFileDialog>
#include <QDir>
#include <QToolButton>
#include <QMouseEvent>
#include <QTextEdit>

#include <rpu.h>
#include <sdr.h>
#include <dsp.h>
#include <config_manager.h>
#include <askr.h>
#include <FftGraph.h>
#include <SonoGraph.h>
#include <stylehelper.h>
#include <splashscreen.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

private:
    Ui::Widget *ui;

    Q_PROPERTY(QPoint previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

    QSettings       settings;

    MouseType       m_leftMouseButtonPressed;
    MouseType       m_rightMouseButtonPressed;
    QPoint          m_previousPosition;
    MouseType       checkResizableField(QMouseEvent *event); // определение области нажатия

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    QPoint          previousPosition() const;

    RPU             *rpu;
    SDR             *sdr;
    DSP             *dsp;
    config_manager  *config_manager_form;
    ASKR            *askr;
    SplashScreen    *spl;

    FFT_GRAPH       *fft_graph;
    SONO_GRAPH      *sono_graph;

    QThread         askrThread;

public slots:
    void setPreviousPosition(QPoint previousPosition);
    void global_update_interface(); // обновление интерфейса
    void set_ui_style(); // изменение интерфейса
    void bind_slots_signals(); // сигнально-слотовые соединения между объектами
    void startAskr();

private slots:
    void on_ConfigButton_clicked(); // открытие/закрытие настройщика
    void on_RecButton_clicked(); // начало записи
    void on_StopButton_clicked(); // остановка записи
    void on_CloseButton_clicked(); // закрытие программы
    void on_SoundVolumeHS_valueChanged(int new_sound_volume); // изменение громкости выходного аудиопотока
    void on_SoundButton_clicked(); // приглушение
    void on_DynamicRangeHS_valueChanged(int new_dnRng);
    void on_NoiseLevelHS_valueChanged(int new_noiseLvl);

    void save_settings(); // сохранение настроек
    void restore_settings(); // восстановление настроек

    void hideEvent(QHideEvent *event); // обработка события сворачивания главного окна
    void showEvent(QShowEvent *event); // обработка события разворачивания главного окна

    void on_CurrentFreqSB_valueChanged(int newFreq);

signals:
    void changeEvent(QEvent *e);
    void previousPositionChanged(QPoint previousPosition);
    void start_reading();
    void repaintFftGraph();
    void repaintSonoGraph();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // WIDGET_H
