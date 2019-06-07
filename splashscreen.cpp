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

#include "splashscreen.h"
#include "ui_splashscreen.h"

SplashScreen::SplashScreen(QWidget *parent) : QWidget(parent), ui(new Ui::SplashScreen)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);    // отключение оформления окна
    this->setAttribute(Qt::WA_TranslucentBackground); // фон главного виджета прозрачным
    ui->InfoLabel->setStyleSheet("QLabel { color: #ca6c4f; border: none; }");
    ui->PixLabel->setPixmap(QPixmap(":/buttons/SQUID 2.png"));
}

SplashScreen::~SplashScreen()
{
    delete ui;
}

void SplashScreen::setInfoLabelText(QString _text)
{
    ui->InfoLabel->setText(_text);
}
