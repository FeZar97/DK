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

    static QString getSoundVolumeHS(bool is_enabled);
    static QString getSoundButton(bool is_mute);

    static QString getHSStyleSheet();

    static QString getDemodRBStyleSheet();

    static QString getTabWidgetStyleSheet();

    static QString getPathButtonStyleSheet();
};

#endif // STYLEHELPER_H
