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

#ifndef CONSTELLATIONDIAGRAM_H
#define CONSTELLATIONDIAGRAM_H

#include <QWidget>
#include <QPainter>
#include <QResizeEvent>
#include <QDebug>

#include <ippCustom.h>

#define     DEFAULT_CONSTEL_WIDTH         251
#define     DEFAULT_CONSTEL_HEIGHT        235

namespace Ui {
class ConstellationDiagram;
}

class ConstellationDiagram : public QWidget
{
    Q_OBJECT

public:
    explicit ConstellationDiagram(QWidget *parent = nullptr);
    ~ConstellationDiagram();

private:
    Ui::ConstellationDiagram *ui;

public slots:
    void                paintSignalPoints(QVector<Ipp32fc> &points, int symRate);
};

#endif // CONSTELLATIONDIAGRAM_H
