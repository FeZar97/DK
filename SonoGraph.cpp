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

#include "SonoGraph.h"

SONO_GRAPH::SONO_GRAPH(QWidget *parent):
                       QWidget{parent},
                       dnRng{nullptr},
                       nsLvl{nullptr},
                       fps{nullptr},
                       snStl{nullptr},
                       w{DEFAULT_SONO_WIDTH},
                       h{DEFAULT_SONO_HEIGHT},
                       fftSz{DEFAULT_FFT_SIZE},
                       sPtr{0},
                       lPtr{0},
                       lNb{0},
                       fftRes{new float[DEFAULT_FFT_SIZE]},
                       vSc{QImage(w,h,QImage::Format_Indexed8)},
                       vSc2{w,h}
{
    setMouseTracking(true);

    setCursor(Qt::CrossCursor);

    clearFftRes();
}

SONO_GRAPH::~SONO_GRAPH()
{
    if(fftRes)
        delete[] fftRes;
}

void SONO_GRAPH::resize(const QSize &s)
{
    resize(s.width(), s.height());
}

void SONO_GRAPH::changePalette()
{
    QImage tmpV = QImage(w, h, QImage::Format_Indexed8);

    switch(*snStl){

        case 0:
        {
            for(int i = 0; i <= 255; i++)
                if(i >= 225)
                    tmpV.setColor(i, QColor(255 - (i - 225), i - 225, i - 225).rgb());
                else
                    if(i != 0 && i < 225)
                        tmpV.setColor(i, QColor(255, 256 - i, 0).rgb());
                    else
                    tmpV.setColor(i, QColor(0,0,0).rgb());
            break;
        }

        case 1:
        {
            for(int i = 0; i <= 255; i++){
                // от черного до темно-синего 35 уровней
                // 10,5,30 --- 78,39,213 c шагом 1 px
                if(i < 35)
                    tmpV.setColor(i, QColor(10 + i*2, 5 + i, 30 + int(i*5.4)).rgb());

                // от темно-синего до фиолетового 40 уровней
                // 81,39,213 --- 198,39,213 с шагом 3 px
                if(i >= 35 && i < 75)
                    tmpV.setColor(i, QColor(78 + (i-35 + 1)*3, 39, 213).rgb());

                // от фиолетового до красного 35 уровней
                // 200,40,210 --- 224,16,18 с шагом 1/1/8 px
                if(i >= 75 && i < 100)
                    tmpV.setColor(i, QColor(200 + (i-75), 40 - (i-75), 210 - (i-75)*8).rgb());

                // от красного до желтого 50 уровней
                // 224,20,20 --- 234,220,0 с шагом (1/5)/(4)/(2/5) px
                if(i >= 100 && i <= 150)
                    tmpV.setColor(i, QColor(224 + (i-100)/5, 20 + (i-100)*4, 20 - (i-100)*2/5).rgb());

                // от желтого до белого 105 уровней
                // 234,220,0 --- 255,255,255 с шагом (31/105)/(35/105)/(235/105) px
                if(i >= 151 && i <= 255)
                    tmpV.setColor(i, QColor(224 + (i-151)*31/105, 220 + (i-151)*35/105, (i-151)*255/105).rgb());
            }
            break;
        }
    }

    tmpV.fill(0);

    vSc = tmpV;
}

void SONO_GRAPH::resize(int _w, int _h)
{
    if(_w > 0 && _h > 0){
        w = _w;
        h = _h;

        vSc.rect().setSize(QSize(w, h));
        vSc2.rect().setSize(QSize(w, h));

        changePalette();

        lNb = h;
        lPtr = lNb - 1;
    }
}

void SONO_GRAPH::setFftSize(int _s)
{
    if(_s > 31 && !(_s & (_s - 1))){
        fftSz = _s;
        if(fftRes)
            delete[] fftRes;
        fftRes = new float[fftSz];
    }

    clearFftRes();
}

void SONO_GRAPH::setPreviousPosition(QPoint previousPosition)
{
    if(m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
    emit previousPositionChanged(previousPosition);
}

void SONO_GRAPH::connectParams(float *extDnRng, float *extNsLvl, int *extFps, int *extSnStl)
{
    dnRng = extDnRng;
    nsLvl = extNsLvl;
    fps = extFps;
    snStl = extSnStl;
}

SONO_MOUSE_TYPE SONO_GRAPH::getMouseField(QMouseEvent *e)
{
    QPointF pos = e->localPos(); // Определяем позицию курсора на экране

    QRectF rct = vSc.rect();
    if(rct.contains(pos))
        return SONO_FREQ_CHANGE;
    return SONO_NONE_MT;
}

// отрисовка виртуального экрана
void SONO_GRAPH::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    //painter.drawImage(QRectF(0,0,fftSz,lNb), vSc);
    painter.drawPixmap(QRectF(0,0,w,h), vSc2, QRectF(0,0,w,h));
}

void SONO_GRAPH::resizeEvent(QResizeEvent *e)
{
    resize(e->size());
}

// отрисовка сонограммы
void SONO_GRAPH::paintScreen()
{
    if(w > 0 && h > 0){
        ippsSubC_32f(fftRes, *nsLvl, fftRes, fftSz);
        ippsMulC_32f_I(255./(*dnRng), fftRes, fftSz);

        // // градиентная заливка 256 пикселей
        // for(int testIdx = 0; testIdx < 256; testIdx++)
        //     fftRes[testIdx] = testIdx;

        if(vSc.width() == fftSz){
            ippsConvert_32f8u_Sfs(fftRes + fftSz/2, sn[sPtr], fftSz/2, ippRndZero, 0);
            ippsConvert_32f8u_Sfs(fftRes, sn[sPtr] + fftSz/2,  fftSz/2, ippRndZero, 0);
        }

        sPtr--;
        if(sPtr < 0) sPtr = DEFAULT_SONO_HEIGHT - 1;

        int counter = 0;
        while(counter < lNb){
            memcpy(vSc.scanLine(counter), sn[(sPtr + 1 + counter)%DEFAULT_SONO_HEIGHT], fftSz);
            counter++;
        }
        vSc2.convertFromImage(vSc);

        QPainter p(&vSc2);
        p.setRenderHint(QPainter::HighQualityAntialiasing, true);
        p.setPen(QPen(Qt::green, 1));
        // отрисовка временных меток
        // каждая строка сонограммы соответствует 1 / READ_OUT_PER_SEC секундам
        //      для READ_OUT_PER_SEC = 10, 10 строк сонограммы соответсвуют 1 секунду времени
        // отметки ставятся через каждые 100 px => время, эквивалентное данным 100 строкам
        // рассчитывается следующим образом: 100 / READ_OUT_PER_SEC
        int time_markers_number = (lNb - 1) / 100 + 1;
        for(int i = 1; i < time_markers_number; i++)
            p.drawText(5, i * 100 + 5, "-" + QString::number(i*100/(*fps)) + " сек");
    }
}

void SONO_GRAPH::clearFftRes()
{
    for(int i = 0; i < fftSz; i++)
        fftRes[i] = 0;
}

// итерация отрисовки
void SONO_GRAPH::paintStep(float *_fftRes)
{
    memcpy(fftRes, _fftRes, uint(fftSz)*sizeof(float));
    paintScreen();
    update();
}

void SONO_GRAPH::mouseMoveEvent(QMouseEvent *e)
{
    switch(m_leftMouseButtonPressed){
        case SONO_FREQ_CHANGE:{
            //cSdrFr += (-1) * (e->x() - m_previousPosition.x()) * (sRt / fftSz);
            //emit changeSdrFreq(cSdrFr);
            setPreviousPosition(e->pos());
            break;
        }
        default:
            getMouseField(e);
            break;
    }

    switch(m_rightMouseButtonPressed){
        case SONO_FREQ_CHANGE:{
            //cRpuFr += int((-1) * (e->x() - m_previousPosition.x()) * (sRt / float(w))) ;
            //emit changeRpuFreq(cRpuFr);
            setPreviousPosition(e->pos());
            break;
        }
        default:
            getMouseField(e);
            break;
    }

    return QWidget::mouseMoveEvent(e);
}

void SONO_GRAPH::mousePressEvent(QMouseEvent *e)
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

void SONO_GRAPH::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        m_leftMouseButtonPressed = SONO_NONE_MT;
    }

    return QWidget::mouseReleaseEvent(e);
}
