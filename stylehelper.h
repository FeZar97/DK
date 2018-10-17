#ifndef STYLEHELPER_H
#define STYLEHELPER_H

#include <QString>

class StyleHelper
{
public:
    static QString getWindowStyleSheet(); // весь виджет
    static QString getLabelStyleSheet(); // лейбл с названием
    static QString getMinimizeStyleSheet();
    static QString getCloseStyleSheet();

    static QString getRecStyleSheet(bool is_enabled); // кнопка записи
    static QString getStopStyleSheet(bool is_enabled);
    static QString getConfigStyleSheet();

    static QString getRefreshStyleSheet();
    static QString getConfirmStyleSheet(bool is_enabled);

    static QString getLineStyleSheet();
};

#endif // STYLEHELPER_H
