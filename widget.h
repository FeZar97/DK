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
    QPoint              m_previousPosition;
    MouseType           checkResizableField(QMouseEvent *event); // определение области нажатия

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    QPoint              previousPosition() const;

    RPU                 *rpu;
    SDR                 *sdr;
    DSP                 *dsp;
    config_manager      *config_manager_form;

public slots:
    void setPreviousPosition(QPoint previousPosition);

    void update_ReadProgressBar(int val);

    // отрисовка
    void paint_fft();

    // слот окончания записи
    // принимает сигнал и при экстренном завершении записи (нажатие кнопки стоп)
    //                  и при штатном завершении записи
    void end_of_recording();

    // обновление окон
    void global_update_interface();

    // изменение интерфейса
    void set_ui_style();

    void end_of_first_file_rec();
    void end_of_second_file_rec();
    void end_of_third_file_rec();

private slots:
    // открытие/закрытие настройщика
    void on_ConfigButton_clicked();

    // начало записи
    void on_RecButton_clicked();

    // принудительная остановка записи
    void on_StopButton_clicked();

    // изменение уровня шума
    void on_NoiseLevelSlider_valueChanged(int new_noise_level);

    // регулировка динамического диапзона
    void on_DynamicRangeSlider_valueChanged(int new_dynamic_range);

    // закрытие окна
    void on_CloseButton_clicked();

    // сохранение настроек
    void save_settings();

    // восстановление настроек
    void restore_settings();

signals:
    void previousPositionChanged(QPoint previousPosition);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // WIDGET_H
