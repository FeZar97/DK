#include "simplegraph.h"

simpleGraph::simpleGraph(QWidget *parent) : QWidget(parent), vScreen( DSP_FFT_SIZE, DSP_FFT_SIZE )
{
    vRect.setTop(0);
    vRect.setLeft(0);
    vRect.setHeight( 711 );
    vRect.setWidth( DSP_FFT_SIZE );

    sdr_params = new SDR_params();
    dsp_params = new DSP_params();

    repaintGraph();
}

simpleGraph::~simpleGraph()
{
}

void simpleGraph::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    painter.drawPixmap(QPoint(0,0), vScreen, vRect);
}

// отрисовка энергетического спектра
void simpleGraph::paint_spectrum(QPainter &painter)
{
    QPointF p1, p2;
    painter.setPen( QPen(Qt::white, 0.9) );
    for(int i = 0 ; i < DSP_FFT_SIZE - 1; ++i){
        if(i != DSP_FFT_SIZE / 2 - 1){

            p1.setX((i + DSP_FFT_SIZE/2) % DSP_FFT_SIZE);
            p1.setY(int(vRect.height() * (1. - (dsp_params->fft_params->fft_res[i] - dsp_params->fft_params->fft_noise_level) / dsp_params->fft_params->fft_dynamic_range)));

            p2.setX((i + DSP_FFT_SIZE/2 + 1) % DSP_FFT_SIZE);
            p2.setY(int(vRect.height() * (1. - (dsp_params->fft_params->fft_res[i + 1] - dsp_params->fft_params->fft_noise_level) / dsp_params->fft_params->fft_dynamic_range)));

            painter.drawLine(p1, p2);
        }
    }

    /* биновая отрисовка
    for( int i = 0 ; i < size ; ++i ) {
        cur.setX(int((1024 * i) / double(size) + 512) % 1024 );

        cur.setY(vRect.height() * (1. - (data[i] - gShift) / gSize) );
        old = QPointF( cur.x(), vRect.bottom() );
        painter.drawLine( old, cur );
    }*/
}

// отрисовка сетки
void simpleGraph::paint_grid(QPainter &painter)
{
    // вертикальные линии
    painter.setPen( QPen(Qt::red, 0.4) );
    painter.drawLine(   0, 0,    0, vRect.height() - 20);
    painter.drawLine( 102, 0,  102, vRect.height() - 20);
    painter.drawLine( 205, 0,  205, vRect.height() - 20);
    painter.drawLine( 307, 0,  307, vRect.height() - 20);
    painter.drawLine( 409, 0,  409, vRect.height() - 20);
    painter.drawLine( 614, 0,  614, vRect.height() - 20);
    painter.drawLine( 716, 0,  716, vRect.height() - 20);
    painter.drawLine( 818, 0,  818, vRect.height() - 20);
    painter.drawLine( 921, 0,  921, vRect.height() - 20);
    painter.drawLine(1023, 0, 1023, vRect.height() - 20);
    painter.setPen(QPen(Qt::red, 1) );
    painter.drawLine( 512, 0,  512, vRect.height() - 20);


    // горизонтальные линии
    // от 70 до 630 с шагом 70 px
    painter.setPen( QPen(Qt::green, 0.4) );
    painter.drawLine(25, 70, vRect.width(), 70);
    painter.drawLine(25, 140, vRect.width(), 140);
    painter.drawLine(25, 210, vRect.width(), 210);
    painter.drawLine(25, 280, vRect.width(), 280);
    painter.drawLine(25, 350, vRect.width(), 350);
    painter.drawLine(25, 420, vRect.width(), 420);
    painter.drawLine(25, 490, vRect.width(), 490);
    painter.drawLine(25, 560, vRect.width(), 560);
    painter.drawLine(25, 630, vRect.width(), 630);
}

// подписи частот, уровней и отображении информации
void simpleGraph::paint_signature(QPainter &painter)
{
    // подписи к вертикальным слайдерам
    painter.setPen(QPen(QColor(21,21,21), 0));

    QRect dyn_range_rect(984, 80, 40, 84);
    QRect shift_rect(984, 500, 40, 70);
    painter.drawRect(dyn_range_rect);
    painter.drawRect(shift_rect);

    painter.save();

    painter.setPen(QPen(Qt::red, 1));
    painter.setFont(QFont("calibri", 15));

    painter.translate(dyn_range_rect.bottomLeft());
    painter.rotate(-90);
    painter.drawText(QRect(0, 0, dyn_range_rect.height(), dyn_range_rect.width()), QTextOption::WrapAnywhere, "диапазон");

    painter.restore();

    painter.save();

    painter.setPen(QPen(Qt::red, 1));
    painter.setFont(QFont("calibri", 15));
    painter.translate(shift_rect.bottomLeft());
    painter.rotate(-90);
    painter.drawText(QRect(0, 0, shift_rect.height(), shift_rect.width()), QTextOption::WrapAnywhere, "уровень");

    painter.restore();

    // подписи частот
    painter.setPen(QPen(Qt::red, 1));

    switch(dsp_params->fft_params->fft_mode){
        case READER_FFT:{
            int new_central_freq = sdr_params->central_freq + dsp_params->shift_params->shift_freq;
            painter.drawText( 75, vRect.height() - 5, QString::number(new_central_freq - sdr_params->sample_rate * 4/10));
            painter.drawText(178, vRect.height() - 5, QString::number(new_central_freq - sdr_params->sample_rate * 3/10));
            painter.drawText(280, vRect.height() - 5, QString::number(new_central_freq - sdr_params->sample_rate * 2/10));
            painter.drawText(382, vRect.height() - 5, QString::number(new_central_freq - sdr_params->sample_rate * 1/10));
            painter.drawText(485, vRect.height() - 5, QString::number(new_central_freq));
            painter.drawText(587, vRect.height() - 5, QString::number(new_central_freq + sdr_params->sample_rate * 1/10));
            painter.drawText(689, vRect.height() - 5, QString::number(new_central_freq + sdr_params->sample_rate * 2/10));
            painter.drawText(791, vRect.height() - 5, QString::number(new_central_freq + sdr_params->sample_rate * 3/10));
            painter.drawText(894, vRect.height() - 5, QString::number(new_central_freq + sdr_params->sample_rate * 4/10));
            break;
        }
        case FLT_FFT:{
            painter.drawText( 75, vRect.height() - 5, QString::number(sdr_params->central_freq - sdr_params->sample_rate * 4/10));
            painter.drawText(178, vRect.height() - 5, QString::number(sdr_params->central_freq - sdr_params->sample_rate * 3/10));
            painter.drawText(280, vRect.height() - 5, QString::number(sdr_params->central_freq - sdr_params->sample_rate * 2/10));
            painter.drawText(382, vRect.height() - 5, QString::number(sdr_params->central_freq - sdr_params->sample_rate * 1/10));
            painter.drawText(485, vRect.height() - 5, QString::number(sdr_params->central_freq));
            painter.drawText(587, vRect.height() - 5, QString::number(sdr_params->central_freq + sdr_params->sample_rate * 1/10));
            painter.drawText(689, vRect.height() - 5, QString::number(sdr_params->central_freq + sdr_params->sample_rate * 2/10));
            painter.drawText(791, vRect.height() - 5, QString::number(sdr_params->central_freq + sdr_params->sample_rate * 3/10));
            painter.drawText(894, vRect.height() - 5, QString::number(sdr_params->central_freq + sdr_params->sample_rate * 4/10));
            break;
        }
        default:
            break;
    }

    // подписи дБ
    int top_value = dsp_params->fft_params->fft_noise_level;
    float step = dsp_params->fft_params->fft_dynamic_range / 10.;
    painter.setPen(QPen(Qt::green, 1));
    painter.drawText(5,  10, QString::number(    top_value             ));
    painter.drawText(5,  74, QString::number(int(top_value - step * 1 )));
    painter.drawText(5, 144, QString::number(int(top_value - step * 2 )));
    painter.drawText(5, 214, QString::number(int(top_value - step * 3 )));
    painter.drawText(5, 284, QString::number(int(top_value - step * 4 )));
    painter.drawText(5, 354, QString::number(int(top_value - step * 5 )));
    painter.drawText(5, 424, QString::number(int(top_value - step * 6 )));
    painter.drawText(5, 494, QString::number(int(top_value - step * 7 )));
    painter.drawText(5, 564, QString::number(int(top_value - step * 8 )));
    painter.drawText(5, 634, QString::number(int(top_value - step * 9 )));
    painter.drawText(5, 702, QString::number(int(top_value - step * 10)));

    // инфо
    if(dsp_params->fft_params->fft_info){
        painter.setPen(QPen(Qt::green, 1) );
        painter.drawText(930, 12, "FPS: "    + QString::number(dsp_params->read_params->readout_per_seconds));
        painter.drawText(929, 24, "Avg: "    + QString::number(dsp_params->fft_params->fft_averages_number));
        painter.drawText(929, 36, "Shft: "   + QString::number(dsp_params->fft_params->fft_noise_level) + " дБ");
        painter.drawText(930, 48, "DR: "     + QString::number(dsp_params->fft_params->fft_dynamic_range) + " дБ");
        painter.drawText(930, 60, "Fд: "     + QString::number(int(sdr_params->sample_rate / 1000.)) + " кГц");
        painter.drawText(930, 72, "DSidx: "  + QString::number(sdr_params->direct_sampling_idx));
        painter.drawText(930, 84, "КУ: "     + (sdr_params->gain_idx ? QString::number(gains[sdr_params->gain_idx - 1]/10.) + " дБ" : "авто"));
        painter.drawText(930, 96, "dfreq: "  + QString::number(dsp_params->shift_params->shift_freq));
        painter.drawText(930, 108, "max: "   + QString::number(dsp_params->fft_params->fft_res[dsp_params->fft_params->max_level_idx]
                                                             - dsp_params->fft_params->fft_dynamic_range - 1, 'f', 1));
        painter.setPen(QPen(Qt::magenta, 1));
        painter.drawEllipse((dsp_params->fft_params->max_level_idx + DSP_FFT_SIZE/2) % DSP_FFT_SIZE - 7,
                            vRect.height() * (1. - (dsp_params->fft_params->fft_res[dsp_params->fft_params->max_level_idx] - dsp_params->fft_params->fft_noise_level) / dsp_params->fft_params->fft_dynamic_range) - 7,
                            14,
                            14);
    }


    painter.setPen(QPen(Qt::yellow, 1));
    int height = vRect.height() * (1. - (dsp_params->fft_params->noise_level - dsp_params->fft_params->fft_noise_level) / dsp_params->fft_params->fft_dynamic_range);
    painter.drawLine(0, height, (dsp_params->fft_params->fft_info ? 1023 : 15), height);
    painter.drawText(5, height - 3, QString::number(dsp_params->fft_params->noise_level - dsp_params->fft_params->fft_dynamic_range - 1, 'f', 1));
}

// отрисовка всего графа
void simpleGraph::repaintGraph()
{
    QPainter painter(&vScreen);

    painter.setRenderHint(QPainter::Antialiasing,            true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);

    // экран
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(21,21,21));
    painter.drawRect(vRect);

    // подписи частот, уровней и отображении информации
    paint_signature(painter);

    // отрисовка энергетического спектра
    paint_spectrum(painter);

    // отрисовка сетки
    paint_grid(painter);

    repaint();
}
