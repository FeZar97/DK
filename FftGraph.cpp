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

#include "FftGraph.h"

FFT_GRAPH::FFT_GRAPH(QWidget *parent):
                     QWidget(parent),
                     dnRng{nullptr},
                     nsLvl{nullptr},
                     nBin{nullptr},
                     rFltFreq{nullptr},
                     shFr{nullptr},
                     cSdrFr{nullptr},
                     cRpuFr{nullptr},
                     sRt{nullptr},
                     stlIdx{nullptr},
                     frqGrdIdx{nullptr},
                     fltBwAlp{nullptr},
                     invFlg{nullptr},
                     w{DEFAULT_WIDTH},
                     h{DEFAULT_HEIGHT},
                     fftSz{DEFAULT_FFT_SIZE},
                     vRN{0},
                     hRN{0},
                     vLN{0},
                     hLN{0},
                     dx{0},
                     dy{0},
                     x{0},
                     y{0},
                     cFrRct{QRect(0,0,1,1)},
                     bckClr{Qt::black},
                     mPsX{0},
                     mPsY{0},
                     avgNLvl{0},// то же самое что и nsLvLn
                     nVCnt{0},
                     fcdMXUpd{true},
                     avgNlvlRd{false},
                     mBin{0},
                     fftRes{new float[DEFAULT_FFT_SIZE]},
                     vSc{w,h},
                     m_leftMouseButtonPressed{NONE_MT},
                     m_rightMouseButtonPressed{NONE_MT}
{
    setMouseTracking(true);

    setCursor(Qt::CrossCursor);

    clearFftRes();
}

FFT_GRAPH::~FFT_GRAPH()
{
    if(fftRes) delete[] fftRes;
}

void FFT_GRAPH::resize(const QSize &s)
{
    resize(s.width(), s.height());
}

void FFT_GRAPH::resize(int _w, int _h)
{
    if(_w > 0 && _h > 0){
        w = _w;
        h = _h;

        vSc.rect().setSize(QSize(w, h));

        cFrRct.setSize(QSize(w - NOISE_PANEL_WIDTH, h));

        // минимальное кол-во регионов == 2
        vRN = w/100 > 1 ? w/100 : 2;
        hRN = h/100 > 1 ? h/100 : 2;

        // условие четности регионов
        if(vRN % 2)
            vRN++;

        // кол-во линий на 1 меньше чем регионов
        vLN = vRN - 1;
        hLN = hRN - 1;

        // расстояние между линиями = ширина экрана / кол-во регионов
        dx = w/float(vRN);
        dy = h/float(hRN);

        paintScreen();
    }
}

void FFT_GRAPH::setFftSize(int _s)
{
    if(_s > 31 && !(_s & (_s - 1))){
        fftSz = _s;
        if(fftRes)
            delete[] fftRes;
        fftRes = new float[fftSz];
    }

    clearFftRes();
}

void FFT_GRAPH::connectParams(float *extDnRng, float *extNsLvl, bool *extNBinFlag, double *extRFltFreq,
                              double *extShFr, int *extCSdrFr, int *extCRpuFr, int *extSRt,
                              int *extStlIdx, int *extFrqGrdIdx, double *extFltBwAlp, bool *extInvFlg)
{
    dnRng = extDnRng;
    nsLvl = extNsLvl;
    nBin = extNBinFlag;
    rFltFreq = extRFltFreq;
    shFr = extShFr;
    cSdrFr = extCSdrFr;
    cRpuFr = extCRpuFr;
    sRt = extSRt;
    stlIdx = extStlIdx;
    frqGrdIdx = extFrqGrdIdx;
    fltBwAlp = extFltBwAlp;
    invFlg = extInvFlg;
}

void FFT_GRAPH::setPreviousPosition(QPoint previousPosition)
{
    if(m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
    emit previousPositionChanged(previousPosition);
}

// определние области нажатия
MOUSE_TYPE FFT_GRAPH::getMouseField(QMouseEvent *e)
{
    QPointF pos = e->localPos(); // Определяем позицию курсора на экране

    if(cFrRct.contains(pos))
        return FREQ_CHANGE;
    return NONE_MT;
}

// отрисовка виртуального экрана
void FFT_GRAPH::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter p(this);
    p.drawPixmap(QRectF(0,0,w,h), vSc, QRectF(0,0,w,h));
}

// отрисовка энергетического спектра
void FFT_GRAPH::paintSpectrum(QPainter &p)
{
    int i;
    QPointF p1, p2;
    p.setPen(QPen(Qt::white, 0.9));

    // При горизонтальном растяжении требуется интерполяция спектра
    // если width > DSP_FFT_SIZE, то

    // число, показывающее количество бинов БПФ, влияющих на каждый пиксель отрисовки спектра
    // если DSP_FFT_SIZE == 4, а width == 6, то interpoly_coef = 1,5 => каждый бин БПФ оказывает влияние на int(interpoly_coef) + 1 = 2 пикселя БПФ

    switch(*stlIdx){

        // ломаная
        case 0:{
            for(i = 0 ; i < fftSz - 1; ++i){
                if(i != fftSz/2 - 1){
                    p1.setX((i + fftSz/2) % fftSz);
                    p1.setY(int(h * (1 - (fftRes[i] - *nsLvl) / *dnRng)));

                    p2.setX((i + fftSz/2 + 1) % fftSz);
                    p2.setY(int(h * (1 - (fftRes[i + 1] - *nsLvl) / *dnRng)));

                    p.drawLine(p1, p2);
                }
            }
            break;
        }

        // бины
        case 1:{
            for(i = 0 ; i < fftSz ; i++){
                p1.setX((i + fftSz/2) % fftSz);
                p1.setY(int(h * (1 - (fftRes[i] - *nsLvl) / *dnRng)));

                p2 = QPointF(p1.x(), h);
                p.drawLine(p1, p2);
            }
            break;
        }

        // точки
        case 2:{
            for(i = 0 ; i < fftSz ; i++){
                p1.setX((i + fftSz/2) % fftSz);
                p1.setY(int(h * (1 - (fftRes[i] - *nsLvl) / *dnRng)));
                p.drawEllipse(p1, 1, 1);
            }
            break;
        }
    }
}

// отрисовка сетки
void FFT_GRAPH::paintGrid(QPainter &p)
{
    int i;

    // отрисовка vLN вертикальных линий с шагом dx
    for(i = 0; i < vLN; i++){
        if(i != vLN/2)
            p.setPen(QPen(Qt::gray, 0.4));
        else
            p.setPen(QPen(Qt::red, 1  ));

        // корректирующая поправка i/2
        x = dx + dx * i;
        // снизу остается 20px для подписей частот
        p.drawLine(QPointF(x, 0), QPointF(x, h - 20));
    }

    // отрисовка hLN горизонтальных линий с шагом dy
    p.setPen(QPen(Qt::gray, 0.4));
    for(i = 0; i < hLN; i++){
        y = dy + dy * i;
        // отступ слева 35px для подписей дБ
        p.drawLine(QPointF(35, y), QPointF(w, y));
    }
}

// подписи частот, уровней и отображении информации
void FFT_GRAPH::paintSignature(QPainter &p)
{
// полоса фильтрации
    p.setPen(Qt::NoPen);
    QColor clr = Qt::gray;
    clr.setAlphaF(*fltBwAlp);
    p.setBrush(clr);
    p.drawRect(QRectF(w/2 - int(w * *rFltFreq), 0, int(w * *rFltFreq * 2), h));

// частоты
    // поле frqGrdIdx определяет источник частот для подписей - частоты БПО (idx == 0) или частоты SDR (idx == 1)
    int srcFrq = *frqGrdIdx  ? *cSdrFr + *shFr : *cRpuFr;
    // fi - i-я частота сетки, dNb - рарядность частоты
    int i, fi, dNb;
    p.setPen(QPen(QColor("#ff8306"), 1));

    for(i = 0; i < vLN; i++){
        // с учетом инверсии
        // fi = int(srcFrq + (*sRt / vRN) * (*invFlg ? (vLN/2 - i) : (i - vLN/2)));
        fi = int(srcFrq + (*sRt / vRN) * (i - vLN/2));
        dNb = 1 + static_cast<int>(log10(fi));
        // середина числа должна располагаться точно под линией сетки
        // 7 разрядов ~ 22px, 8 ~ 24 px, 9 ~ 26 px
        // x_coord = (delta_x + delta_x * i + i/2) - (dig_num * 2 + 8)
        //p.drawText(int(dx + dx * i + i/2 - (dNb * 2 + 8)), h - 5, QString::number(fi));
        p.drawText(int(dx + dx * i + i/2 - (dNb * 2 + 8)), h - 5, separateFreq(fi));
    }

// дБ
    float vStep = *dnRng / hRN;
    // 4px - поправка для размера шрифта
    for(i = 0; i < hLN; i++)
        p.drawText(5,  int(dy + dy * i + 4), QString::number(double(*nsLvl - vStep * (i + 1)), 'f', 1));

// avgNLvl
    p.setPen(QPen(Qt::yellow, 1));
    int nLvlY = int(h * (1 - (avgNLvl - *nsLvl) / *dnRng));
    //p.drawLine(0, nLvlY, (*info ? w : 15), nLvlY);
    //if(*info) p.drawText(34, nLvlY - 4, QString::number(double(avgNLvl - *dnRng), 'f', 1));

// нулевой бин
    p.setPen(QPen(Qt::cyan));
    if(*nBin) p.drawEllipse(w/2 - 5 + (w * *shFr / *sRt), int(h * (1 - (fftRes[int(w * *shFr / *sRt)] - *nsLvl) / *dnRng) - 5), 10, 10);

// линия трекера
    // поле frqGrdIdx определяет источник частот для подписей - частоты БПО (idx == 0) или частоты SDR (idx == 1)
    srcFrq = *frqGrdIdx  ? *cSdrFr + *shFr : *cRpuFr;
    // #6f72ff
    p.setPen(QPen(QColor("#2d0fff"), 1));
    p.drawLine(mPsX, 0, mPsX, h - 15);
    p.setPen(QPen(QColor("#6f72ff"), 1));
    // значение частоты, соответствующей линии трекера
    p.drawText(mPsX + 4, h - 19, separateFreq(srcFrq + (mPsX/double(w) - 0.5) * *sRt));
}

// изменение размеров виджета
void FFT_GRAPH::resizeEvent(QResizeEvent *e)
{
    resize(e->size());
}

// отрисовка спектра
void FFT_GRAPH::paintScreen()
{
    if(w > 0 && h > 0){
        QPainter p(&vSc);
        p.setRenderHint(QPainter::HighQualityAntialiasing, true);

        p.setPen(Qt::NoPen);
        p.setBrush(Qt::black);
        p.drawRect(QRectF(0, 0, w, h));

        calcNoiseLevel();

        findMaxBin();

        paintSpectrum(p);
        paintSignature(p);
        paintGrid(p);

        update();
    }
}

void FFT_GRAPH::clearFftRes()
{
    for(int i = 0; i < fftSz; i++)
        fftRes[i] = 0;
}

/*
 в мгновенном спектре отыскиваются 10 минимальных значений, расположенных в полосе [-fд + fд/10; fd - fд/10]
 для этих 10 значений накапливается среднее значение на интервале readout_per_sec
    10 векторов длиной readout_per_sec, каждый вектор отслеживает среднее значение своего бина БПФ
 чем больше значние параметра readout_per_sec, тем точнее будет оценка этих 10 шумовых значений
 вектора обновляются на каждом такте приема
 скользящее среднее каждого из 10 векторов дает средний уровень шума каждого отслеживаемого бина БПФ
 усредняя 10 скользящих средний получаем оценку шума
 */
void FFT_GRAPH::calcNoiseLevel()
{
    int min_idx = 0, i, j, smpl_cnt = 0;

    // если надо сбросить всю шумовую матрицу,
    // то поиск 10-ти наимешьших значений потворяется
    if(fcdMXUpd){

        fcdMXUpd = false;
        avgNlvlRd = false;

        nVCnt = 0;

        // очищаем шумовую матрицу
        nMx.clear();

        // поиск 10 минимальных значений, расположенных в полосе [-fд + fд/10; fd - fд/10]
        for(i = 0; i < 10; i++){
            min_idx = 0;

            // если найдем маленький бин, то его индекс должен быть отличен от уже найденных индексов
            for(j = 1; j < fftSz/2 - fftSz/5; j++)
                if(fftRes[j] < fftRes[min_idx] && !nMx.contains(j)) min_idx = j;

            for(j = fftSz/2 + fftSz/5; j < fftSz; j++)
                if(fftRes[j] < fftRes[min_idx] && !nMx.contains(j)) min_idx = j;

            // при первом запуске минимальный индекс будет 0, и матрица инициализируется бинами 1..10
            if(min_idx == 0) nMx.insert(++smpl_cnt, QVector<float>(10, 0.));

            // добавляем в словарь вектор из readout_per_seconds = 10 элементов и инициализируем его нулями
            nMx.insert(min_idx, QVector<float>(10, 0.));

            // сразу же добавляем значение бина БПФ в вектор
            nMx[min_idx][nVCnt] = fftRes[min_idx];
        }
    }else{
        // на данном этапе существует словарь из 10 векторов, соответствующих 10-ти минимальным шумовым значениям

            // номера минимальных бинов
            QList<int> min_bins = nMx.keys();

            // 1)на каждой итерации приема в каждом векторе вместо самого старого шумового значения добавляется очередное шумовое значение бина
            // записываем в шумовой вектор, соответствующий бину с индексом min_bins[i], новое шумовое значение
            for(i = 0; i < 10; i++)
                nMx[min_bins[i]][nVCnt] = fftRes[min_bins[i]];

            float ten_avg_levels[10];

            // 2) для всех шумовых векторов считается скользящее среднее
            // оценка среднего значения шума суть есть среднее этих 10-ти скользящих средних
            float avg_of_one_noise_vector = 0., avg_of_all_noise_vectors = 0.;
            for(i = 0; i < 10; i++){

                // сброс аккумулятора
                avg_of_one_noise_vector = 0.;

                // сумма всех шумовых значений данного вектора
                for(j = 0; j < 10; j++)
                    avg_of_one_noise_vector += nMx[min_bins[i]][j];

                // усреднение, которое и дает скользящее среднее одного шумового вектора
                avg_of_one_noise_vector = avg_of_one_noise_vector / 10;

                // tesing
                ten_avg_levels[i] = avg_of_one_noise_vector;

                // добавляем скользящее среднее одного вектора в общий шумовой акуумулятор
                avg_of_all_noise_vectors += avg_of_one_noise_vector;
            }

            // 3) усреднение общего шумового вектора дает оценку среднего значения шума
            avg_of_all_noise_vectors /= 10;

            // tesing
            float max_delta = 0.0, delta;
            for(i = 0; i < 10; i++){
                delta = float(fabs(double(ten_avg_levels[i] - avg_of_all_noise_vectors)));
                if(delta > max_delta) max_delta = delta;
            }
            if(max_delta >= 1) fcdMXUpd = true;

            // если прошло 10 итераций с момента обновления шумовой матрицы, то можно перезаписывать среднее значение шума
            if(!avgNlvlRd && nVCnt == 9) avgNlvlRd = true;

            // если накопилась достаточная статистика о шуме, то записываем ее в поле среднего значения
            if(avgNlvlRd) avgNLvl = avg_of_all_noise_vectors;
    }

    // инкремент итератора по шумовым векторам
    nVCnt = (nVCnt + 1) % 10;
}

// поиск максимального бина БПФ
void FFT_GRAPH::findMaxBin()
{
    mBin = 0;
    for(int i = 0; i < fftSz; i++)
        if(fftRes[i] > fftRes[mBin]) mBin = i;
}

QString FFT_GRAPH::separateFreq(int freq)
{
    QString res = QString::number(freq);
    for (int i = res.size() - 3; i >= 1; i -= 3)
        res.insert(i, ' ');
    return res;
}

// итерация отрисовки
void FFT_GRAPH::paintStep(float *_fftRes)
{
    memcpy(fftRes, _fftRes, uint(fftSz)*sizeof(float));
    paintScreen();
}

void FFT_GRAPH::mouseMoveEvent(QMouseEvent *e)
{
    // трекер частоты, на которую наводит курсор
    mPsX = e->x();
    mPsY = e->y();

    switch(m_leftMouseButtonPressed){
        case FREQ_CHANGE:{
            *cSdrFr += (-1) * (e->x() - m_previousPosition.x()) * (*sRt / fftSz);
            emit changeSdrFreq(*cSdrFr);
            setPreviousPosition(e->pos());
            break;
        }

        default:
            getMouseField(e);
            break;
    }

    switch(m_rightMouseButtonPressed){
        case FREQ_CHANGE:{
            int newTractFreq = (int(*cRpuFr + int((-1) * (e->x() - m_previousPosition.x()) * (*sRt / float(w))))/1000)*1000;
            //int newTractFreq = int(*cRpuFr + int((-1) * (e->x() - m_previousPosition.x()) * 1000));
            //*cRpuFr += int((-1) * (e->x() - m_previousPosition.x()) * (*sRt / float(w)));
            emit changeRpuFreq(newTractFreq);
            setPreviousPosition(e->pos());
            break;
        }
        default:
            getMouseField(e);
            break;
    }

    // new
    paintScreen();

    return QWidget::mouseMoveEvent(e);
}

void FFT_GRAPH::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        m_leftMouseButtonPressed = getMouseField(e);
        setPreviousPosition(e->pos());
    }
    if(e->button() == Qt::RightButton){
        m_rightMouseButtonPressed = getMouseField(e);
        setPreviousPosition(e->pos());
    }
    return QWidget::mousePressEvent(e);
}

void FFT_GRAPH::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        m_leftMouseButtonPressed = NONE_MT;
    }
    if(e->button() == Qt::RightButton){
        m_rightMouseButtonPressed = NONE_MT;
    }

    return QWidget::mouseReleaseEvent(e);
}
