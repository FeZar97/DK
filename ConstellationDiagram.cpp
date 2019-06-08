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

#include "ConstellationDiagram.h"
#include "ui_ConstellationDiagram.h"

ConstellationDiagram::ConstellationDiagram(QWidget *parent) :   QWidget(parent),
                                                                ui(new Ui::ConstellationDiagram)
{
    ui->setupUi(this);
}

ConstellationDiagram::~ConstellationDiagram()
{
    delete ui;
}

// отрисовка сигнальных точек points со скоростью symRate
void ConstellationDiagram::paintSignalPoints(QVector<Ipp32fc> &points, int symRate)
{

}
