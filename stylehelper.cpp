#include "stylehelper.h"

QString StyleHelper::getWindowStyleSheet()
{
    return
/// QWidget
            "QWidget{"
            "    background-color: #454545;"
            "    border: 1px solid black;"
            "    border-radius: 3px;"
            "}"
/// QLabel
            "QLabel{"
            "    color: #DCDCDC;"
            "    border: none;"
            "}"
/// QSpinBox
           "QSpinBox {"
           "     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(51, 51, 51), stop: 0.4 rgb(39, 39, 39), stop: 0.5 rgb(32,32,32), stop: 1.0 rgb(38,38,38));"
           "     border: 1px;"
           "     color: #de8e37;"
           "}"
/// QDoubleSpinBox
           "QDoubleSpinBox{"
           "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(51, 51, 51), stop: 0.4 rgb(39, 39, 39), stop: 0.5 rgb(32,32,32), stop: 1.0 rgb(38,38,38));"
           "border: 1px;"
           "color: #de8e37;"
           "}"
/// QProgressBar
           "QProgressBar:horizontal{"
           "    border: 1px solid gray;"
           "    border-radius: 3px;"
           "    background: #454545;"
           "    padding: 1px;"
           "    margin-right: 4ex;"
           "}"
           "QProgressBar::chunk:horizontal{"
           "    background: #de8e37;"
           "    margin-right: 2px;"
           "    width: 10px;"
           "}"
/// QSlider
           "QSlider:vertical{"
           "    border: none;"
           "    background: transparent;"
           "    selection-color: #de8e37;"
           "}"
/// QComboBox
            "QComboBox{"
            "   border: 1px solid rgb(95, 95, 95);"
            "   color: #de8e37;"
            "   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(51, 51, 51), stop: 0.4 rgb(39, 39, 39), stop: 0.5 rgb(32,32,32), stop: 1.0 rgb(38,38,38));"
            "}"
/// QPushButton
            "QPushButton{"
            "   background: #949494;"
            "}"
            "QPushButton::hover{"
            "   background: #e7ad6d;"
            "}"
            "QPushButton::pressed{"
            "   background: #de8e37;"
            "}"
/// QCheckBox
            "QCheckBox{"
            "   color: #de8e37;"
            "   border: 0px;"
            "   background: #454545;"
            "}"
            ;
}

QString StyleHelper::getLabelStyleSheet()
{
    return "QLabel{"
           "    color: #de8e37;"
           "    background: qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0,"
                                           "stop: 0 #454545, stop: 1.0 #000000);"
           "    border: none;"
           "}";
}

QString StyleHelper::getRecStyleSheet(bool is_enabled)
{
    if(is_enabled)
        return "QToolButton{"
               "    image: url(:/buttons/PLAY_GRAYGREEN.png);"
               "    background-color: #454545;"
               "    border: none;"
               "}"
               "QToolButton:hover{"
               "    image: url(:/buttons/PLAY_GREEN.png);"
               "}"
               "QToolButton:pressed{"
               "    image: url(:/buttons/PLAY_GRAYGREEN.png);"
               "}";
    else
        return "QToolButton{"
               "    image: url(:/buttons/PLAY_GRAY.png);"
               "    background-color: #454545; "
               "    border: none;"
               "}";
}

QString StyleHelper::getStopStyleSheet(bool is_enabled)
{
    if(is_enabled)
        return "QToolButton{"
               "    image: url(:/buttons/STOP_GRAYRED.png);"
               "    background-color: #454545;"
               "    border: none;"
               "}"
               "QToolButton:hover{"
               "    image: url(:/buttons/STOP_RED.png);"
               "}"
               "QToolButton:pressed{"
               "    image: url(:/buttons/STOP_GRAYRED.png);"
               "}";
    else
        return "QToolButton{"
               "    image: url(:/buttons/STOP_GRAY.png);"
               "    background-color: #454545;"
               "    border: none; "
               "}";
}

QString StyleHelper::getConfigStyleSheet()
{
    return "QToolButton{"
           "    border-image: url(:/buttons/settings.png);"
           "    background-color: #454545; "
           "    border: none; "
           "}"
           "QToolButton:pressed{"
           "    background-color: #de8e37;"
           "}";
}

QString StyleHelper::getRefreshStyleSheet()
{
    return "QPushButton{"
           "    image: url(:/buttons/REFRESH_ORANGE.png);"
           "    background-color: #454545;"
           "    border: none;"
           "}"
           "QPushButton:pressed{"
           "    background-color: #de8e37;"
           "}";
}

QString StyleHelper::getConfirmStyleSheet(bool is_enabled)
{
    if(is_enabled)
        return "QPushButton{"
               "    image: url(:/buttons/confirm.png);"
               "    background-color: #454545;"
               "    border: none;"
               "}"
               "QPushButton:pressed{"
               "    background-color: #de8e37;"
               "}";
    else
        return "QPushButton{"
               "    image: none;"
               "    background-color: #454545;"
               "    border: none;"
               "}";
}

QString StyleHelper::getLineStyleSheet()
{
    return "QFrame{"
           "    border: none;"
           "};";
}

QString StyleHelper::getCloseStyleSheet()
{
    return "QToolButton{"
           "    border-image: url(:/buttons/CLOSE_GRAY.png);"
           "    background-color: transparent;"
           "    border-radius: 5px;"
           "}"
           "QToolButton:hover{"
           "    border-image: url(:/buttons/CLOSE_WHITE.png);"
           "}"
           "QToolButton:pressed{"
           "    border-image: url(:/buttons/CLOSE_WHITE.png);"
           "    background-color: #de8e37;"
           "}";
}

QString StyleHelper::getMinimizeStyleSheet()
{
    return "QToolButton{"
           "    border-image: url(:/buttons/MIN_GRAY.png);"
           "    background-color: transparent;"
           "    border-radius: 5px;"
           "}"
           "QToolButton:hover{"
           "    border-image: url(:/buttons/MIN_WHITE.png);"
           "}"
           "QToolButton:pressed{"
           "    border-image: url(:/buttons/MIN_WHITE.png);"
           "    background-color: #de8e37; "
           "}";
}

QString StyleHelper::getSoundVolumeHS(bool is_enabled)
{
    if(is_enabled)
        return "QSlider{"
               "    border: 0px; solid #606060;"
               "}"
               "QSlider::groove:horizontal {"
               "    border: 0px; solid #606060;"
               "    height: 14px;"
               "    background: qlineargradient(x1:0, y1:0.4, x2:0, y2:0.6, stop:0 #454545, x3:0, y3:1, stop:0.5 #de8e37, stop:1 #454545);"
               "}"
               "QSlider::handle:horizontal {"
               "    background: #de8e37;"
               "    width: 12px;"
               "    border-radius: 6px;"
               "}";
    else
        return "QSlider{"
               "    border: 0px; solid #606060;"
               "}"
               "QSlider::groove:horizontal {"
               "    border: 0px; solid #606060;"
               "    height: 14px;"
               "    background: qlineargradient(x1:0, y1:0.4, x2:0, y2:0.6, stop:0 #454545, x3:0, y3:1, stop:0.5 #B8B8B8, stop:1 #454545);"
               "}"
               "QSlider::handle:horizontal {"
               "    background: #B8B8B8;"
               "    width: 12px;"
               "    border-radius: 6px;"
               "}";
}

QString StyleHelper::getSoundButton(bool is_mute)
{
    if(is_mute)
        return "QToolButton{"
               "    image: url(:/buttons/SOUND_OFF.png);"
               "    border: none;"
               "}"
               "QToolButton:pressed{"
               "    background-color: #de8e37;"
               "}";
    else
        return "QToolButton{"
               "    image: url(:/buttons/SOUND_ON.png);"
               "    border: none; "
               "}"
               "QToolButton:pressed{"
               "    background-color: #de8e37;"
               "}";
}

QString StyleHelper::getHSStyleSheet()
{
    return "QSlider{"
           "    border: 0px; solid #606060;"
           "}"
           "QSlider::groove:horizontal {"
           "    height: 14px;"
           "    background: qlineargradient(x1:0, y1:0.4, x2:0, y2:0.6, stop:0 #454545, x3:0, y3:1, stop:0.5 #de8e37, stop:1 #454545);"
           "}"
           "QSlider::handle:horizontal {"
           "    background: #de8e37;"
           "    width: 12px;"
           "    border-radius: 6px;"
           "}";
}

QString StyleHelper::getDemodRBStyleSheet()
{
    return
        "QRadioButton{"
        "    border: none;"
        "    color: #DCDCDC;"
        "}"
        "QRadioButton::indicator{"
        "    width: 16px;"
        "    height: 16px;"
        "}"
        "QRadioButton::indicator::unchecked{"
        "    image: url(:/buttons/DISABLED_RB_C.png);"
        "}"
        "QRadioButton::indicator::checked{"
        "    image: url(:/buttons/ENABLED_RB_C.png);"
        "}";
}

QString StyleHelper::getTabWidgetStyleSheet()
{
    return
           "QTabWidget{"
           "    border: 0px; solid #26FF26;"
           "}"
            "QTabWidget::pane{"
            "    border: 0;"
            "}"
            "QTabWidget::tab-bar{"
            "    border: 0px;"
            "    border-color: #26FF26;"
            "}"
            "QTabBar::tab{"
            "    background: #949494;"
            "    border-color: #26FF26;"
            "    border: 0px; solid #26FF26;"
            "    border-top-left-radius: 8px;"
            "    border-top-right-radius: 8px;"
            "    min-width: 50px;"
            "    min-height: 20px;"
            "}"
            "QTabBar::tab:selected, QTabBar::tab:hover{"
            "    background: #e1e1e1;"
            "    border-color: #26FF26;"
            "}"
            "QTabBar::tab:selected{"
            "    background: #de8e37;"
            "    color: #454545;"
            "    border-color: #26FF26;"
           "};";
}

QString StyleHelper::getPathButtonStyleSheet()
{
    return "QPushButton{"
           "    image: url(:/buttons/FOLDER.png);"
           "    background-color: #454545;"
           "    border: none;"
           "}"
           "QPushButton:pressed{"
           "    background-color: #de8e37;"
           "}";
}
