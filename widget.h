#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include <QSplashScreen>
#include <QStyle>
#include <QFileDialog>
#include <QDir>
#include <QToolButton>
#include <QMouseEvent>

#include <rpu.h>
#include <sdr.h>
#include <dsp.h>
#include <config_manager.h>
#include <simplegraph.h>
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

    QSettings           settings;

    MouseType           m_leftMouseButtonPressed;
    MouseType           m_rightMouseButtonPressed;
    QPoint              m_previousPosition;
    MouseType           checkResizableField(QMouseEvent *event); // определение области нажатия

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    QPoint              previousPosition() const;

    RPU                 *rpu;
    SDR                 *sdr;
    DSP                 *dsp;
    config_manager      *config_manager_form;

public slots:
    void setPreviousPosition(QPoint previousPosition);

    void paint_fft(); // отрисовка спектра

    void end_of_recording(); // окончание записи

    void global_update_interface(); // обновление интерфейса

    void set_ui_style(); // изменение интерфейса

    void end_of_first_file_rec();
    void end_of_second_file_rec();
    void end_of_third_file_rec();

    void bind_slots_signals(); // сигнально-слотовые соединения между объектами

private slots:
    void on_ConfigButton_clicked(); // открытие/закрытие настройщика

    void on_RecButton_clicked(); // начало записи !!!!!!!!!!!!!!!!!

    void on_StopButton_clicked(); // принудительная остановка записи

    void on_NoiseLevelSlider_valueChanged(int new_noise_level); // изменение уровня шума

    void on_DynamicRangeSlider_valueChanged(int new_dynamic_range); // регулировка динамического диапзона

    void on_CloseButton_clicked(); // закрытие программы

    void save_settings(); // сохранение настроек

    void restore_settings(); // восстановление настроек

    void test_kalibrator(); // тестировка калибратора

    void hideEvent(QHideEvent *event); // обработка события сворачивания главного окна

    void showEvent(QShowEvent *event); // обработка события разворачивания главного окна

signals:
    void changeEvent(QEvent *e);
    void previousPositionChanged(QPoint previousPosition);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // WIDGET_H
