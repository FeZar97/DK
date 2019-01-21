#ifndef SIMPLEGRAPH_H
#define SIMPLEGRAPH_H

#include <QWidget>

#include <sdr.h>
#include <reader.h>
#include <fft_calcer.h>
#include <fft_shifter.h>

class simpleGraph : public QWidget
{
    Q_OBJECT
public:
    explicit            simpleGraph(QWidget *parent = 0);
                       ~simpleGraph();

    SDR_params          *sdr_params;
    DSP_params          *dsp_params;

private:
    QRect               vRect;          // размеры текущего экрана
    QPixmap             vScreen;        // виртуальный экран  - на нем рисовать быстрее, безопаснее и без ряби

    void                paintEvent(QPaintEvent *e);

    void                paint_spectrum(QPainter &painter); // отрисовка спектра
    void                paint_grid(QPainter &painter); // отрисовка сетки
    void                paint_signature(QPainter &painter); // подписи частот, уровней и отображении информации

public slots:
    void                repaintGraph(); // функция перерисовки графика
};

#endif // SIMPLEGRAPH_H
