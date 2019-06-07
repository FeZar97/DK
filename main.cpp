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

#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pix(":/buttons/SQUID 2.png");
    QSplashScreen splashScreen(pix);
    splashScreen.show();
    a.processEvents();

    QTime time;
    time.start();

    while(time.elapsed() < 1500){
        splashScreen.setPixmap(pix);
        a.processEvents();
    }

    Widget w;
    w.show();
    splashScreen.finish(&w);

    return a.exec();
}
