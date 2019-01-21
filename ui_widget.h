/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include <simplegraph.h>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *widgetInterface;
    QProgressBar *ReadProgressBar;
    QLabel *WindowTitleLabel;
    QToolButton *RecButton;
    QToolButton *StopButton;
    QToolButton *ConfigButton;
    QToolButton *CloseButton;
    QToolButton *MinimizeButton;
    simpleGraph *FftGraph;
    QSlider *DynamicRangeSlider;
    QSlider *NoiseLevelSlider;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(1026, 776);
        Widget->setMinimumSize(QSize(1026, 776));
        Widget->setMaximumSize(QSize(1026, 776));
        QIcon icon;
        icon.addFile(QStringLiteral(":/buttons/SquidLow.png"), QSize(), QIcon::Normal, QIcon::Off);
        Widget->setWindowIcon(icon);
        widgetInterface = new QWidget(Widget);
        widgetInterface->setObjectName(QStringLiteral("widgetInterface"));
        widgetInterface->setGeometry(QRect(0, 0, 1024, 762));
        widgetInterface->setMinimumSize(QSize(1024, 762));
        widgetInterface->setMaximumSize(QSize(1024, 762));
        ReadProgressBar = new QProgressBar(widgetInterface);
        ReadProgressBar->setObjectName(QStringLiteral("ReadProgressBar"));
        ReadProgressBar->setGeometry(QRect(83, 30, 928, 16));
        QFont font;
        font.setPointSize(10);
        font.setBold(false);
        font.setWeight(50);
        font.setKerning(false);
        ReadProgressBar->setFont(font);
        ReadProgressBar->setMaximum(100);
        ReadProgressBar->setValue(0);
        ReadProgressBar->setAlignment(Qt::AlignCenter);
        ReadProgressBar->setTextVisible(false);
        ReadProgressBar->setOrientation(Qt::Horizontal);
        ReadProgressBar->setTextDirection(QProgressBar::TopToBottom);
        WindowTitleLabel = new QLabel(widgetInterface);
        WindowTitleLabel->setObjectName(QStringLiteral("WindowTitleLabel"));
        WindowTitleLabel->setGeometry(QRect(0, 1, 1039, 24));
        QFont font1;
        font1.setFamily(QStringLiteral("Comic Sans MS"));
        font1.setPointSize(14);
        WindowTitleLabel->setFont(font1);
        WindowTitleLabel->setFrameShape(QFrame::NoFrame);
        WindowTitleLabel->setAlignment(Qt::AlignCenter);
        RecButton = new QToolButton(widgetInterface);
        RecButton->setObjectName(QStringLiteral("RecButton"));
        RecButton->setGeometry(QRect(6, 30, 16, 16));
        StopButton = new QToolButton(widgetInterface);
        StopButton->setObjectName(QStringLiteral("StopButton"));
        StopButton->setGeometry(QRect(30, 30, 16, 16));
        ConfigButton = new QToolButton(widgetInterface);
        ConfigButton->setObjectName(QStringLiteral("ConfigButton"));
        ConfigButton->setGeometry(QRect(54, 26, 23, 23));
        CloseButton = new QToolButton(widgetInterface);
        CloseButton->setObjectName(QStringLiteral("CloseButton"));
        CloseButton->setGeometry(QRect(996, 3, 23, 23));
        CloseButton->setMinimumSize(QSize(23, 23));
        CloseButton->setMaximumSize(QSize(23, 23));
        MinimizeButton = new QToolButton(widgetInterface);
        MinimizeButton->setObjectName(QStringLiteral("MinimizeButton"));
        MinimizeButton->setGeometry(QRect(972, 3, 23, 23));
        MinimizeButton->setMinimumSize(QSize(23, 23));
        MinimizeButton->setMaximumSize(QSize(23, 23));
        FftGraph = new simpleGraph(widgetInterface);
        FftGraph->setObjectName(QStringLiteral("FftGraph"));
        FftGraph->setGeometry(QRect(0, 51, 1024, 711));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FftGraph->sizePolicy().hasHeightForWidth());
        FftGraph->setSizePolicy(sizePolicy);
        FftGraph->setMinimumSize(QSize(1024, 711));
        FftGraph->setMaximumSize(QSize(1024, 711));
        DynamicRangeSlider = new QSlider(FftGraph);
        DynamicRangeSlider->setObjectName(QStringLiteral("DynamicRangeSlider"));
        DynamicRangeSlider->setGeometry(QRect(1002, 6, 20, 371));
        DynamicRangeSlider->setAutoFillBackground(false);
        DynamicRangeSlider->setInputMethodHints(Qt::ImhNone);
        DynamicRangeSlider->setMinimum(10);
        DynamicRangeSlider->setMaximum(150);
        DynamicRangeSlider->setSingleStep(5);
        NoiseLevelSlider = new QSlider(FftGraph);
        NoiseLevelSlider->setObjectName(QStringLiteral("NoiseLevelSlider"));
        NoiseLevelSlider->setGeometry(QRect(1002, 380, 20, 321));
        QFont font2;
        font2.setPointSize(12);
        NoiseLevelSlider->setFont(font2);
        NoiseLevelSlider->setAutoFillBackground(false);
        NoiseLevelSlider->setInputMethodHints(Qt::ImhNone);
        NoiseLevelSlider->setMinimum(-50);
        NoiseLevelSlider->setMaximum(0);
        NoiseLevelSlider->setValue(-10);
        WindowTitleLabel->raise();
        ReadProgressBar->raise();
        RecButton->raise();
        StopButton->raise();
        ConfigButton->raise();
        CloseButton->raise();
        MinimizeButton->raise();
        FftGraph->raise();

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "\320\246\320\270\321\204\321\200\320\276\320\262\320\276\320\271 \321\202\321\200\320\260\320\272\321\202 \320\240\320\237\320\243 \"\320\232\320\260\320\273\321\214\320\274\320\260\321\200\"", Q_NULLPTR));
        ReadProgressBar->setFormat(QApplication::translate("Widget", "%p%", Q_NULLPTR));
        WindowTitleLabel->setText(QApplication::translate("Widget", "\320\246\320\270\321\204\321\200\320\276\320\262\320\276\320\271 \321\202\321\200\320\260\320\272\321\202 \320\240\320\237\320\243 \"\320\232\320\260\320\273\321\214\320\274\320\260\321\200\"", Q_NULLPTR));
        RecButton->setText(QString());
        StopButton->setText(QString());
        ConfigButton->setText(QString());
        CloseButton->setText(QString());
        MinimizeButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
