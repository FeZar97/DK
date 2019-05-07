#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pix(":/buttons/SQUID.png");
    QSplashScreen splashScreen(pix);
    splashScreen.show();
    a.processEvents();

    QTime time;
    time.start();
    while(time.elapsed() < 1000){
        splashScreen.setPixmap(pix);
    }

    Widget w;
    w.show();
    splashScreen.finish(&w);

    return a.exec();
}
