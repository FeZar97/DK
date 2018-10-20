#include "stylehelper.h"

QString StyleHelper::getWindowStyleSheet()
{
    return

/// QWidget
           "QWidget {"
               "background-color: #454545;"
               "border: 1px solid black;"
               "border-radius: 3px;"
           "}"

/// QLabel
           "QLabel {"
               "color: rgb(220, 220, 220);"
               "border: none;"
           "}"

/// QSpinBox
           "QSpinBox {"
               "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(51, 51, 51), stop: 0.4 rgb(39, 39, 39), stop: 0.5 rgb(32,32,32), stop: 1.0 rgb(38,38,38));"
               "border: 1px;"
               "color: #de8e37;"
           "}"

/// QDoubleSpinBox
           "QDoubleSpinBox {"
               "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(51, 51, 51), stop: 0.4 rgb(39, 39, 39), stop: 0.5 rgb(32,32,32), stop: 1.0 rgb(38,38,38));"
               "border: 1px;"
               "color: #de8e37;"
           "}"

/// QProgressBar
           "QProgressBar:horizontal {"
               "border: 1px solid gray;"
               "border-radius: 3px;"
               "background: #454545;"
               "padding: 1px;"
               "margin-right: 4ex;"
           "}"
           "QProgressBar::chunk:horizontal {"
               "background: #de8e37;"
               "margin-right: 2px;"
               "width: 10px;"
           "}"

/// QSlider
           "QSlider:vertical {"
               "border: none;"
               "background: transparent;"
               "selection-color: #de8e37;"
           "}"

/// QComboBox
            "QComboBox {"
                "border: 1px solid rgb(95, 95, 95);"
                "color: #de8e37;"
                "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(51, 51, 51), stop: 0.4 rgb(39, 39, 39), stop: 0.5 rgb(32,32,32), stop: 1.0 rgb(38,38,38));"
            "}"

/// QPushButton
            "QPushButton {                                 "
            "   background: #949494;                       "
            "}                                             "

            "QPushButton::hover {                          "
            "   background: #e7ad6d;                       "
            "}                                             "

            "QPushButton::pressed {                        "
            "   background: #de8e37;                       "
            "}                                             "

/// QCheckBox
            "QCheckBox {                                   "
            "   color: #de8e37;                            "
            "   border: 0px;                               "
            "   background: #454545;                       "
            "}                                             "

/// QTabWidget
            /* The tab widget frame */
            "QTabWidget::pane {                            "
            "    border: 0px;                              "
            "}                                             "

            "QTabWidget::tab-bar {                         "
            "    border: 0px;                              "
            "}                                             "

            "QTabBar::tab {                                "
            "    background: #949494;                      "
            "    border: none;                             "
            "    border-top-left-radius: 8px;              "
            "    border-top-right-radius: 8px;             "
            "    min-width: 70px;                          "
            "    min-height: 20px;                         "
            "}                                             "

            "QTabBar::tab:selected, QTabBar::tab:hover {   "
            "    background: #e1e1e1;                      "
            "}                                             "

            "QTabBar::tab:selected {                       "
            "    border: none;"
            "    background: #de8e37;                      "
            "    color: #454545;                           "
            "};                                            "
            ;
}

QString StyleHelper::getLabelStyleSheet()
{
    return "QLabel {"
                "color: #de8e37;"
                "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                                          "stop: 0 rgb(51, 51, 51), stop: 0.4 rgb(39, 39, 39),"
                                                          "stop: 0.5 rgb(32,32,32), stop: 1.0 rgb(38,38,38));"
                "border: none;"
           "}";
}

QString StyleHelper::getRecStyleSheet(bool is_enabled)
{
    if(is_enabled)
        return "QToolButton { "
                    "image: url(:/buttons/PLAY_GRAYGREEN.png);"
                    "background-color: #454545; "
                    "border: none; "
               "}"

               "QToolButton:hover {"
                    "image: url(:/buttons/PLAY_GREEN.png);"
                    "background-color: #303030; "
               "}"

               "QToolButton:pressed { "
                    "image: url(:/buttons/PLAY_GRAYGREEN.png);"
                    "background-color: #de8e37; "
               "}";
    else
        return "QToolButton { "
                    "image: url(:/buttons/PLAY_GRAY.png);"
                    "border: none; "
               "}";
}

QString StyleHelper::getStopStyleSheet(bool is_enabled)
{
    if(is_enabled)
        return "QToolButton { "
                    "image: url(:/buttons/STOP_GRAYRED.png);"
                    "background-color: #454545; "
                    "border: none; "
               "}"

               "QToolButton:hover {"
                    "image: url(:/buttons/STOP_RED.png);"
                    "background-color: #303030; "
               "}"

               "QToolButton:pressed { "
                    "image: url(:/buttons/STOP_GRAYRED.png);"
                    "background-color: #de8e37; "
               "}";
    else
        return "QToolButton { "
                    "image: url(:/buttons/STOP_GRAY.png);"
                    "border: none; "
               "}";
}

QString StyleHelper::getConfigStyleSheet()
{
    return "QToolButton { "
                "border-image: url(:/buttons/settings.png);"
                "background-color: #454545; "
                "border: none; "
           "}"

           "QToolButton:hover {"
                "background-color: #303030; "
           "}"

           "QToolButton:pressed { "
                "background-color: #de8e37; "
           "}";
}

QString StyleHelper::getRefreshStyleSheet()
{
    return "QPushButton { "
                "image: url(:/buttons/REFRESH_ORANGE.png);"
                "background-color: #454545; "
                "border: none;"
           "}"

           "QPushButton:hover {"
                "background-color: #303030; "
           "}"

           "QPushButton:pressed { "
                "background-color: #de8e37; "
           "}";
}

QString StyleHelper::getConfirmStyleSheet(bool is_enabled)
{
    if(is_enabled)
        return "QPushButton { "
                    "image: url(:/buttons/confirm.png);"
                    "background-color: #454545; "
                    "border: none;"
               "}"

               "QPushButton:hover {"
                    "background-color: #303030; "
               "}"

               "QPushButton:pressed { "
                    "background-color: #de8e37; "
               "}";
    else
        return "QPushButton { "
                    "image: none;"
                    "background-color: #454545;"
                    "border: none;"
               "}"
                ;
}

QString StyleHelper::getLineStyleSheet()
{
    return "QFrame{ "
                         "border: none;"
                         "color: #454545;"
                    "};";
}

QString StyleHelper::getCloseStyleSheet()
{
    return "QToolButton { "
                "border-image: url(:/buttons/CLOSE_GRAY.png);"
                "background-color: #292929; "
                "border-radius: 5px;"
           "}"

           "QToolButton:hover {"
                "border-image: url(:/buttons/CLOSE_WHITE.png); "
           "}"

           "QToolButton:pressed { "
                "border-image: url(:/buttons/CLOSE_WHITE.png);"
                "background-color: #de8e37; "
           "}";
}

QString StyleHelper::getMinimizeStyleSheet()
{
    return "QToolButton { "
                "border-image: url(:/buttons/MIN_GRAY.png);"
                "background-color: #292929;"
                "border-radius: 5px;"
           "}"

                "QToolButton:hover { "
                "border-image: url(:/buttons/MIN_WHITE.png); "
           "}"

           "QToolButton:pressed { "
                "border-image: url(:/buttons/MIN_WHITE.png);"
                "background-color: #de8e37; "
           "}";
}

