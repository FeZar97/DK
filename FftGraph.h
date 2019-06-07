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

#ifndef FFTGRAPH_H
#define FFTGRAPH_H

#include <QWidget>
#include <QPainter>
#include <QResizeEvent>
#include <QDebug>
#include <QtMath>

#define     DEFAULT_WIDTH              1024
#define     DEFAULT_HEIGHT             706
#define     NOISE_PANEL_WIDTH          35
#define     DEFAULT_FFT_SIZE           1024

// тип лкм/пкм
enum MOUSE_TYPE{FREQ_CHANGE,
                NONE_MT};

class FFT_GRAPH : public QWidget
{
    Q_OBJECT

public:
                        FFT_GRAPH(QWidget *parent = nullptr);
                       ~FFT_GRAPH();

    // экспортируемое
    float               *dnRng;    // динамиечский диапазон
    float               *nsLvl;    // уровень шума
    bool                *nBin;     // флаг отображения нулевого бина бпф
    double              *rFltFreq; // полоса фильтрации
    double              *shFr;     // смещение нулевой частоты на спектре
    int                 *cSdrFr;   // центральная частота SDR
    int                 *cRpuFr;   // центральная частота РПУ
    int                 *sRt;      // частота дискретизации
    int                 *stlIdx;   // стиль отображения спектра
    int                 *frqGrdIdx;// источник подписей для сетки частот
    double              *fltBwAlp; // прозрачность полосы фильтрации
    bool                *invFlg;   // инверсия спектра

    // внутренние поля
    int                 w;         // ширина виджета
    int                 h;         // высота виджета

    int                 fftSz;     // размерность БПФ

    int                 vRN;       // количество вертикальных регионов
    int                 hRN;       // количество горизонтальных регионов
    int                 vLN;       // количество вертикальных линий сетки
    int                 hLN;       // количество горизонтальных линий сетки

    float               dx;        // ширина горизонтального региона
    float               dy;        // ширина вертикального региона
    float               x;         // абсцисса
    float               y;         // ордината
    QRectF              cFrRct;    // прямоугольник соответствующий области изменения частоты

    QColor              bckClr;    // цвет подложки

    int                 mPsX;      // абсцисса указтеля мыши
    int                 mPsY;      // ордината указтеля мыши

    float               avgNLvl;   // скользящее среднее шума
    QMap<int, QVector<float>>
                        nMx;       // шумовая мтарица
    int                 nVCnt;     // итератор
    bool                fcdMXUpd;  // флаг принудительного обновления шумовой матрицы
    bool                avgNlvlRd; // флаг готовности среднего значения

    int                 mBin;      // максимальный бин БПФ

    float              *fftRes;    // буфер для хранения энергетического спектра

    QPixmap             vSc;       // виртуальный экран

    void                setPreviousPosition(QPoint previousPosition);

    void                resize(const QSize &s);
    void                resize(int _w, int _h);

    void                setFftSize(int _s);

    void                connectParams(float *extDnRng, float *extNsLvl, bool *extNBinFlag, double *extRFltFreq,
                                      double *extShFr, int *extCSdrFr, int *extCRpuFr, int *extSRt,
                                      int *extStlIdx, int *extFrqGrdIdx, double *extFltBwAlp,
                                      bool *extInvFlg);

private:
    MOUSE_TYPE          m_leftMouseButtonPressed;
    MOUSE_TYPE          m_rightMouseButtonPressed;
    QPoint              m_previousPosition;
    MOUSE_TYPE          getMouseField(QMouseEvent *e); // определение области нажатия

    virtual void        resizeEvent(QResizeEvent *e);

    void                paintEvent(QPaintEvent *e);

    void                paintSpectrum(QPainter &p);  // отрисовка спектра
    void                paintGrid(QPainter &p);      // отрисовка сетки
    void                paintSignature(QPainter &p); // подписи частот, уровней и отображении информации

    void                clearFftRes();

    void                calcNoiseLevel();
    void                findMaxBin();
    QString             separateFreq(int freq);

public slots:
    void                paintStep(float *_fftRes);
    void                paintScreen();

protected:
    void                mouseMoveEvent(QMouseEvent *e);
    void                mousePressEvent(QMouseEvent *e);
    void                mouseReleaseEvent(QMouseEvent *e);

signals:
    void                changeEvent(QEvent *e);
    void                previousPositionChanged(QPoint previousPosition);
    void                changeSdrFreq(int _sdrFr);
    void                changeRpuFreq(int _rpuFr);
    void                changeNoiseLevel(float _nsLvl);
};

#endif // FFTGRAPH_H
