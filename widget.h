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

    FFT_GRAPH       *fft_graph;
    SONO_GRAPH      *sono_graph;

public slots:
    void setPreviousPosition(QPoint previousPosition);
    void global_update_interface(); // обновление интерфейса
    void set_ui_style(); // изменение интерфейса
    void bind_slots_signals(); // сигнально-слотовые соединения между объектами
    void first_rpu_test(); // первый тест РПУ

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

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // WIDGET_H
