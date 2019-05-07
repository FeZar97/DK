#ifndef SONOGRAPH_H
#define SONOGRAPH_H

#include <QWidget>
#include <QPainter>
#include <QResizeEvent>
#include <QDebug>

#include <ippCustom.h>

#define     DEFAULT_SONO_WIDTH         1024
#define     DEFAULT_SONO_HEIGHT        706
#define     DEFAULT_FFT_SIZE           1024

enum SONO_STYLE{
         SHARP,
         TIRADA
};

enum SONO_MOUSE_TYPE{SONO_FREQ_CHANGE,
                     SONO_NONE_MT};

class SONO_GRAPH : public QWidget
{
    Q_OBJECT

public:
                        SONO_GRAPH(QWidget *parent = nullptr);
                       ~SONO_GRAPH();

     // экспортируемое
     float              *dnRng; // динамиечский диапазон
     float              *nsLvl; // уровень шума
     int                *fps;   // частота обновления сонограммы

     // внутренние поля
     int                w;     // ширина виджета
     int                h;     // высота виджета

     int                fftSz; // размерность БПФ

     int                sPtr;  // глобальный указатель
     uchar              sn[DEFAULT_SONO_HEIGHT][DEFAULT_FFT_SIZE];

     int                lPtr;  // локальный указатель
     int                lNb;   // количество линий сонограммы

     SONO_STYLE         snStl; // стиль сонограммы

     float              *fftRes;// буфер для хранения энергетического спектра

     QImage             vSc;   // виртуальный экран
     QPixmap            vSc2;   // виртуальный экран

     void               setPreviousPosition(QPoint previousPosition);

     void               connectParams(float *extDnRng, float *extNsLvl, int *extFps);

     void               resize(const QSize &s);
     void               resize(int _w, int _h);

     void               setFftSize(int _s);
private:
    SONO_MOUSE_TYPE     m_leftMouseButtonPressed;
    SONO_MOUSE_TYPE     m_rightMouseButtonPressed;
    QPoint              m_previousPosition;
    SONO_MOUSE_TYPE     getMouseField(QMouseEvent *e);

    virtual void        resizeEvent(QResizeEvent *e);

    void                paintEvent(QPaintEvent *e);

    void                set_new_size(const QSize &s);

    void                paintScreen();

    void                clearFftRes();

public slots:
    void                paintStep(float *_fftRes);

protected:
    void                mouseMoveEvent(QMouseEvent *e);
    void                mousePressEvent(QMouseEvent *e);
    void                mouseReleaseEvent(QMouseEvent *e);

signals:
    void                previousPositionChanged(QPoint previousPosition);
};

#endif // SONOGRAPH_H
